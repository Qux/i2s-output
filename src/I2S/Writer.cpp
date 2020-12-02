#include "Writer.h"

void I2S::audioWriteTask(void* param) {
    // std::cout << "Writetask has begun" << std::endl; 
    I2S::Writer* writer = static_cast<I2S::Writer*>(param);
    // const std::size_t range = Math::ipow(2, 23) * 0.9 - 1 ;  // * 1.9

    // static Types::audiobuf_t tmpbuf;
    // const std::size_t count = tmpbuf.size() / 2;    
    
    Types::fifobuffer_t* fifo = writer->getAudioBufferPtr();

    while(true) {
        // std::cout << "Writetask loop has begun" << std::endl;
        i2s_event_t event;
        if(xQueueReceive(writer->queue, &event, portMAX_DELAY) == pdPASS) {
            // std::cout << "event on" << std::endl;
            if(event.type == I2S_EVENT_TX_DONE) {   // if transmission is done
                // std::cout << "TX Event" << std::endl;
                std::size_t i2s_bytes_write = 0;
                // i2s_write(Config::DAC::I2S_NUM, writer->buf->pop().data(), Config::DAC::DMA::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);
                i2s_write(Config::DAC::I2S_NUM, fifo->pop().data(), Config::DAC::DMA::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);
                std::cout << "Write : " << i2s_bytes_write << std::endl;
            }
        }
    }
}



void I2S::Writer::begin()  {
    std::cout << "Setting up I2S output..." << std::endl;    
    
    i2s_config_t i2s_config = {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX),                                   // Only TX
        .sample_rate = Config::Sampling_Rate,
        .bits_per_sample = Config::Bit_Rate,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = static_cast<i2s_comm_format_t>(I2S_COMM_FORMAT_I2S),  //I2S_COMM_FORMAT_STAND_I2S - probably version thing
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                               //Interrupt level 1
        .dma_buf_count = Config::DAC::DMA::Buffer_Count,
        .dma_buf_len = Config::DAC::DMA::Buffer_Length,//64,   samples
        .use_apll = true,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0,
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = Config::DAC::Pins::BCK,
        .ws_io_num = Config::DAC::Pins::WS,
        .data_out_num = Config::DAC::Pins::DO,
        .data_in_num = Config::DAC::Pins::DI              //Not used
    };
    i2s_driver_install(Config::DAC::I2S_NUM, &i2s_config, Config::DAC::DMA::Buffer_Count, &this->queue);
    i2s_set_pin(Config::DAC::I2S_NUM, &pin_config);
    i2s_set_clk(Config::DAC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
    i2s_set_sample_rates(Config::DAC::I2S_NUM, Config::Sampling_Rate);
    i2s_zero_dma_buffer(Config::DAC::I2S_NUM);

    
    // Run Task    
    std::cout << "Begining Audio Writer Task..." << std::endl;

    TaskHandle_t tmpWriteTaskHandle; // Could be NULL?
    xTaskCreate(I2S::audioWriteTask, "I2S Writer Task", 8192, this, 1, &tmpWriteTaskHandle);
}