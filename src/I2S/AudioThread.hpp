#pragma once

#include <functional>

/* FREERTOS */ 
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2s.h"
#include "esp_system.h"


#include "Config.hpp"
#include "Types.hpp"
#include "App/ListeningApp.hpp"

namespace I2S {
    TaskHandle_t taskHandle;
    QueueHandle_t queue;

    inline std::size_t read(Types::audiobuf_t& buf) noexcept {
        static std::size_t i2s_bytes_read = 0;
        i2s_read(Config::ADC::I2S_NUM, buf.data(), Config::DMA::I2S_Buffer_Size, &i2s_bytes_read, portMAX_DELAY);     // read from i2s      
        return i2s_bytes_read;       
    }

    inline std::size_t write(const Types::audiobuf_t& buf) noexcept {        
        // static Types::audiobuf_t tmpbuf;
        // tmpbuf.fill(0);        
        std::size_t i2s_bytes_write = 0; // Could be nullptr   
        i2s_write(Config::DAC::I2S_NUM, buf.data(), Config::DMA::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);                
        return i2s_bytes_write;
    }

    template<Stream_State stream_state>
    void audioThread(void* param) noexcept {
        std::cout << "Audio Thread has been provoked" << std::endl;
        ListeningApp* app = static_cast<ListeningApp*>(param);
        static Types::audiobuf_t buf;
        buf.fill(0);

        while(true) {
            i2s_event_t event;
            if(xQueueReceive(queue, &event, portMAX_DELAY) == pdPASS) {
                // audioEvent(event, app, buf);
                if constexpr (stream_state == Stream_State::InOut) {                    
                    if(event.type == I2S_EVENT_TX_DONE) {                        
                        read(buf);                                                                                   
                        app->runDSP(buf);
                        write(buf);
                    }
                }           

                if constexpr (stream_state == Stream_State::Output_Only) {                            
                    if(event.type == I2S_EVENT_TX_DONE) {                                                                    
                        app->runDSP(buf);
                        write(buf);                        
                    }
                }

                if constexpr (stream_state == Stream_State::Input_Only) {                    
                    if(event.type == I2S_EVENT_RX_DONE) {
                        read(buf);
                        app->runDSP(buf);
                    }
                }            

                if constexpr (stream_state == Stream_State::No_Audio) {
                    app->runDSP(buf);
                }                    
            }
        }
    }
    
    void beginThread(ListeningApp& app) {
        // using task = audioThread<Config::Stream>(void*);        
        xTaskCreatePinnedToCore(audioThread<Config::Stream>, "AudioThread", 8192, &app, 0, &taskHandle, APP_CPU_NUM);
    }
    

    void setupWriter() {
        const i2s_config_t i2s_config = {
            .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX),  // Only TX
            .sample_rate = Config::Sampling_Rate,
            .bits_per_sample = Config::Bit_Rate,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
            .communication_format = static_cast<i2s_comm_format_t>(I2S_COMM_FORMAT_I2S),  //I2S_COMM_FORMAT_STAND_I2S - probably version thing
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                               //Interrupt level 1
            .dma_buf_count = Config::DMA::Buffer_Count,
            .dma_buf_len = Config::DMA::Buffer_Length,//64,   samples
            .use_apll = false,
            .tx_desc_auto_clear = true,
            // .fixed_mclk = Config::MCLK_Freq,        
            .fixed_mclk = 0,
        };

        const i2s_pin_config_t pin_config = {
            .bck_io_num = Config::DAC::Pins::BCK,
            .ws_io_num = Config::DAC::Pins::WS,
            .data_out_num = Config::DAC::Pins::DO,
            .data_in_num = Config::DAC::Pins::DI    //Not used
        };
        i2s_driver_install(Config::DAC::I2S_NUM, &i2s_config, Config::DMA::Buffer_Count, &queue);
        i2s_set_pin(Config::DAC::I2S_NUM, &pin_config);
        // i2s_set_clk(Config::DAC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
        // i2s_set_sample_rates(Config::DAC::I2S_NUM, Config::Sampling_Rate);
        i2s_zero_dma_buffer(Config::DAC::I2S_NUM);
    };

    void setupReader() {
        const i2s_config_t i2s_config {
            .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_RX),  // Only TX
            .sample_rate = Config::Sampling_Rate,
            .bits_per_sample = Config::Bit_Rate,
            .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,  //2-channels
            .communication_format = static_cast<i2s_comm_format_t>(I2S_COMM_FORMAT_I2S),  //I2S_COMM_FORMAT_STAND_I2S - probably version thing
            .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,   //Interrupt level 1
            .dma_buf_count = Config::DMA::Buffer_Count,
            .dma_buf_len = Config::DMA::Buffer_Length, //64,
            .use_apll = false,
            .tx_desc_auto_clear = true,
            // .fixed_mclk = Config::MCLK_Freq,
            .fixed_mclk = 0,        
        };

        const i2s_pin_config_t pin_config = {
            .bck_io_num = Config::ADC::Pins::BCK,
            .ws_io_num = Config::ADC::Pins::WS,
            .data_out_num = Config::ADC::Pins::DO,  //Not used
            .data_in_num = Config::ADC::Pins::DI                       
        };

        i2s_driver_install(Config::ADC::I2S_NUM, &i2s_config, Config::DMA::Buffer_Count, &queue);    
        i2s_set_pin(Config::ADC::I2S_NUM, &pin_config);
        // i2s_set_sample_rates(Config::ADC::I2S_NUM, Config::Sampling_Rate);
        // i2s_set_clk(Config::ADC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
        i2s_zero_dma_buffer(Config::ADC::I2S_NUM);
    };
}
