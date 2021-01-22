#include "Reader.h"

void I2S::audioReadTask(void* param) {
    std::cout << "audio read task started." << std::endl;
    I2S::Reader* reader = static_cast<I2S::Reader*>(param);
    
    
    // constexpr std::size_t count = tmpbuf.size() / 2;
    // printf("AD fifo adr: %p\n", reader->buffer); 

    Types::audiobuf_t tmpbuf;
    // tmpbuf.fill(0);

    // Types::audiobuf_t* b = new Types::audiobuf_t;

    while(true) {
        // wait for some data to arrive on the queue
        i2s_event_t event;
        if (xQueueReceive(reader->queue, &event, portMAX_DELAY) == pdPASS)  {            
            if (event.type == I2S_EVENT_RX_DONE)  {                    
                
                // printf("tmpbuf adr: %p\n", &tmpbuf); 

                // placeholder to count incoming data bytes
                std::size_t i2s_bytes_read = 0;
                // read from i2s                
                i2s_read(Config::ADC::I2S_NUM, tmpbuf.data(), Config::ADC::DMA::I2S_Buffer_Size, &i2s_bytes_read, portMAX_DELAY);                       

                // std::cout << "Read : " << i2s_bytes_read << ":";
                
                // for(const auto sample: tmpbuf) {
                //     std::cout << sample << ":";
                // }

                // std::cout << std::endl;

                for(std::size_t i = 0; i < 512; i++) {
                    float lch = static_cast<float>(tmpbuf[2 * i]) * Config::Bit_Range_Reciprocal;
                    float rch = static_cast<float>(tmpbuf[2 * i + 1]) * Config::Bit_Range_Reciprocal;
                    
                    DSP(lch, rch);

                    tmpbuf[2*i] = static_cast<int>(lch * Config::Bit_Range);
                    tmpbuf[2*i + 1] = static_cast<int>(rch * Config::Bit_Range);
                }
                
                
                reader->buffer->push(tmpbuf);                               
            }
        }
    }
}

void I2S::Reader::begin() {
    const i2s_config_t i2s_config {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_SLAVE | I2S_MODE_RX),                                   // Only TX
        .sample_rate = Config::Sampling_Rate,
        .bits_per_sample = Config::Bit_Rate,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = static_cast<i2s_comm_format_t>(I2S_COMM_FORMAT_I2S),  //I2S_COMM_FORMAT_STAND_I2S - probably version thing
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,   //Interrupt level 1
        .dma_buf_count = Config::ADC::DMA::Buffer_Count,
        .dma_buf_len = Config::ADC::DMA::Buffer_Length, //64,
        .use_apll = false,
        .tx_desc_auto_clear = false,
        .fixed_mclk = Config::Sampling_Rate * 256,
    };

    const i2s_pin_config_t pin_config = {
        .bck_io_num = Config::ADC::Pins::BCK,
        .ws_io_num = Config::ADC::Pins::WS,
        .data_out_num = Config::ADC::Pins::DO,  //Not used
        .data_in_num = Config::ADC::Pins::DI                       
    };

    i2s_driver_install(Config::ADC::I2S_NUM, &i2s_config, 4, &queue);    
    i2s_set_pin(I2S_NUM_0, &pin_config);
    // i2s_set_sample_rates(Config::ADC::I2S_NUM, Config::Sampling_Rate);
    // i2s_set_clk(Config::ADC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
    i2s_zero_dma_buffer(Config::ADC::I2S_NUM);
    
    // readTaskHandle; // Could be NULL?    
    xTaskCreate(audioReadTask, "I2S Reader Task", 8192, this, 1, &readerTaskHandle);
}