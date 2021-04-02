#include "Writer.h"

void I2S::audioWriteTask(void* param) {
    std::cout << "audio write task started." << std::endl;
    I2S::Writer* writer = static_cast<I2S::Writer*>(param);

    while(true) {
        i2s_event_t event;
        if(xQueueReceive(writer->queue, &event, portMAX_DELAY) == pdPASS) {
            if(event.type == I2S_EVENT_TX_DONE) {   // if transmission is done
                        
                if(Config::Stream == Stream_State::Output_Only) {               
                    
                    // static StereoSample out;
                    static Types::audiobuf_t tmpbuf;  
                    // constexpr std::size_t loop_count = tmpbuf.size() / 2;
                    // for(std::size_t i = 0; i < loop_count; i++) {
                    //     writer->app->dsp(StereoSample(0.0, 0.0), out);

                    //     tmpbuf[2*i] = static_cast<int>(out.R * Config::Bit_Range);
                    //     tmpbuf[2*i + 1] = static_cast<int>(out.L * Config::Bit_Range);  
                    // }

                    writer->app->runDSP(tmpbuf);

                    std::size_t i2s_bytes_write = 0; // Could be nullptr   
                    i2s_write(Config::DAC::I2S_NUM, tmpbuf.data(), Config::DMA::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);
                } else {            
                    std::size_t i2s_bytes_write = 0; // Could be nullptr           
                    i2s_write(Config::DAC::I2S_NUM, writer->buf->pop().data(), Config::DMA::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);
                }                
            }
        }
    }
}


I2S::Writer::Writer(ListeningApp* _app) : app{_app} {}

void I2S::Writer::begin()  {
    i2s_config_t i2s_config = {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX),                                   // Only TX
        .sample_rate = Config::Sampling_Rate,
        .bits_per_sample = Config::Bit_Rate,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = static_cast<i2s_comm_format_t>(I2S_COMM_FORMAT_I2S),  //I2S_COMM_FORMAT_STAND_I2S - probably version thing
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                               //Interrupt level 1
        .dma_buf_count = Config::DMA::Buffer_Count,
        .dma_buf_len = Config::DMA::Buffer_Length,//64,   samples
        .use_apll = true,
        .tx_desc_auto_clear = false,
        // .fixed_mclk = Config::Sampling_Rate * 256,
        .fixed_mclk = Config::MCLK_Freq,
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = Config::DAC::Pins::BCK,
        .ws_io_num = Config::DAC::Pins::WS,
        .data_out_num = Config::DAC::Pins::DO,
        .data_in_num = Config::DAC::Pins::DI    //Not used
    };
    i2s_driver_install(Config::DAC::I2S_NUM, &i2s_config, Config::DMA::Buffer_Count, &this->queue);
    i2s_set_pin(Config::DAC::I2S_NUM, &pin_config);
    // i2s_set_clk(Config::DAC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
    // i2s_set_sample_rates(Config::DAC::I2S_NUM, Config::Sampling_Rate);
    i2s_zero_dma_buffer(Config::DAC::I2S_NUM);

    // Run the Task    
    xTaskCreate(I2S::audioWriteTask, "I2S Writer Task", 8192, this, 1, &writerTaskHandle);
}