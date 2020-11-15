/*
- Helpful repo: https://github.com/atomic14/esp32_audio

*/


/** C++ **/
#include <array>
#include <chrono>
#include <cstddef>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <functional>
#include <limits>
#include <iostream>
#include <thread>

/* ESP32 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_system.h"
#include "esp32/spiram.h"

// #include <audio_pipeline>

/* Original */
#include "Config.h"

#include "DSP/DelayLine.h"
#include "DSP/Oscillator.h"
#include "UnitTest.h"
#include "Util/RangedCounter.h"
#include "Util/Static_FIFO.h"

typedef std::array<int, Config::ADC::DMA::Buffer_Length> audiobuf_t;
typedef Static_FIFO<audiobuf_t, Config::ADC::DMA::Buffer_Count> fifobuffer_t;

Oscillator osc;

TaskHandle_t m_i2sWriterTaskHandle; // I2S write task
QueueHandle_t m_i2sQueue;   // i2s writer queue
TaskHandle_t writerTaskHandle;


struct buffer_container_t {
    Static_FIFO<int, 8192> DA;
    Static_FIFO<int, 8192> AD;
};

void setup_i2s();

void unit_test() {
    std::cout << "Int Size: " << sizeof(int) << std::endl;
    constexpr auto tick_dur_ms = 1000 / portTICK_PERIOD_MS;
    std::cout << "Tick Size: " << tick_dur_ms << std::endl;

    std::cout << "Beginning unit test...\n" << std::endl;

    #ifdef BOARD_HAS_PSRAM
    // printf("SPIRAM FOUND - %d\n", esp_spiram_get_size());
    printf("SPIRAM FOUND\n");
    #endif // DEBUG
    #ifdef CONFIG_SPIRAM_USE
    printf("SPIRAM ENABLED\n");
    #endif

    using namespace UnitTest;
    
    DelayLine<int, 10> idelay;    
    assert_equal(idelay.get(0), 0);
    idelay.add(1);    
    assert_equal(idelay.get(0), 1);
    idelay.add(2);
    assert_equal(idelay.get(0), 2);
    assert_equal(idelay.get(1), 1);

    std::cout << "Float Delay test beginning..." << std::endl;

    DelayLine<float, 10> fdelay;    
    assert_equal(fdelay.get(0), 0.0, true);
    fdelay.add(1.0);
    assert_equal(fdelay.get(0), 1.0);
    fdelay.add(2.0);
    assert_equal(fdelay.get(0), 2.0);
    assert_equal(fdelay.get(1), 1.0);
    assert_equal(fdelay.get(2), 0.0);
    assert_equal(fdelay.get(3), 0.0);
    assert_equal(fdelay.get(4), 0.0);

    std::cout << "Static FIFO test beginning..." << std::endl;
    Static_FIFO<int, 5> fifo;
    assert_equal(fifo.has_queue(), 0, true);
    fifo.push(0);    
    assert_equal(fifo.has_queue(), 1);    
    assert_equal(fifo.pop(), 0);
    assert_equal(fifo.has_queue(), 0);

    fifo.push(1);
    fifo.push(2);
    fifo.push(3);
    assert_equal(fifo.has_queue(), 3);    
    assert_equal(fifo.pop(), 1);
    assert_equal(fifo.pop(), 2);
    assert_equal(fifo.pop(), 3);

    std::cout << "RangedCounter test beginning..." << std::endl;
    constexpr std::size_t Count_Max = 100;
    RangedCounter<Count_Max> counter;
    assert_equal(counter, 0, true);     
    assert_equal(counter++, 0);    
    assert_equal(counter, 1);
    assert_equal(--counter, 0);
    assert_equal(--counter, Count_Max - 1);    
    assert_equal(++counter, 0);
    assert_equal(++counter, 1);
    counter = 50;
    assert_equal(counter, 50);

    std::cout << "finished Unit test " << std::endl;
}

void DSP(float& Lch, float& Rch) {
    const float vol = osc.getNext();

    // Lch *= -1;
    // Rch *= -1;

    // Lch >>= 8;
    // Rch = 8;

    // int output = static_cast<int>(vol * range_max);
    Lch = vol;
    Rch = vol;
}

void vAudioReadThread(void* param) {
    fifobuffer_t* buf = (fifobuffer_t*)param;
    static audiobuf_t tmpbuf;
    static std::size_t i2s_bytes_read = 0;
    
    while(true) {
        if(buf->has_space()) {
            i2s_read(Config::ADC::I2S_NUM, tmpbuf.data(), Config::ADC::DMA::I2S_Buffer_Size, &i2s_bytes_read, portMAX_DELAY); 

            constexpr std::size_t count = tmpbuf.size() / 2;
            for(std::size_t i = 0; i < count; i++) {
                // DSP(tmpbuf[2 * i], tmpbuf[2 * i + 1]);
            }

            buf->push(tmpbuf);

        }   
        vTaskDelay(1);   
    }
}

void vAudioWriteTask(void* param) {       
    fifobuffer_t* buf = (fifobuffer_t*)param;    
    static audiobuf_t tmpbuf;

    // std::cout << "Begin write task" << std::endl;
    
    // std::array<int, 2> bulk_data = {0, 0};
    // i2s_write(Config::DAC::I2S_NUM, bulk_data.data(), sizeof(int) * bulk_data.size(), NULL, portMAX_DELAY);

    std::cout << "Going into loop" << std::endl;

    while(true) {
        std::cout << "Loop begin" << std::endl;
        // i2s_event_t evt;
        // if (xQueueReceive(m_i2sQueue, &evt, portMAX_DELAY) == pdPASS && evt.type == I2S_EVENT_TX_DONE)   {
            /*
            Something is wrong with writing - there're some glitch
        */
        // if(buf->has_queue() != 0 ) {   
            // audiobuf_t& samples = buf->pop();
            // DSP HERE
            
            // constexpr std::size_t range = std::pow(2, 30) * 1.8;  // * 1.9
            constexpr std::size_t range = std::pow(2, 23) - 1 ;  // * 1.9
            constexpr std::size_t count = tmpbuf.size() / 2;
            for(std::size_t i = 0; i < count; i++) {
                // float lch = static_cast<float>(tmpbuf[2 * i] >> 8) * Config::Bit_Range_Reciprocal;
                // float rch = static_cast<float>(tmpbuf[2 * i + 1] >> 8) * Config::Bit_Range_Reciprocal;
                static float lch, rch;    
                DSP(lch, rch);
                
                tmpbuf[2*i] = static_cast<int>(lch * range) << 7;
                tmpbuf[2*i + 1] = static_cast<int>(rch * range) << 7;
            }

            std::size_t i2s_bytes_write = 0;
            // i2s_write(Config::DAC::I2S_NUM, samples.data(), Config::DAC::DMA::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);
            i2s_write(Config::DAC::I2S_NUM, tmpbuf.data(), Config::DAC::DMA::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);

            vTaskDelay(1);
        // }
        // }
                std::cout << "Loop end" << std::endl;
    }
}


void setup_adc() {
    i2s_config_t i2s_config {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_SLAVE | I2S_MODE_RX),                                   // Only TX
        .sample_rate = Config::Sampling_Rate,
        .bits_per_sample = Config::Bit_Rate,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = static_cast<i2s_comm_format_t>(I2S_COMM_FORMAT_I2S),  //I2S_COMM_FORMAT_STAND_I2S - probably version thing
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                               //Interrupt level 1
        .dma_buf_count = Config::ADC::DMA::Buffer_Count,
        .dma_buf_len = Config::ADC::DMA::Buffer_Length, //64,
        .use_apll = true,
        .tx_desc_auto_clear = false,
        .fixed_mclk = Config::Sampling_Rate * 256,
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = Config::ADC::Pins::BCK,
        .ws_io_num = Config::ADC::Pins::WS,
        .data_out_num = Config::ADC::Pins::DO,  //Not used
        .data_in_num = Config::ADC::Pins::DI               
    };
    i2s_driver_install(Config::ADC::I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(Config::ADC::I2S_NUM, &pin_config);
    i2s_set_sample_rates(Config::ADC::I2S_NUM, Config::Sampling_Rate);
    i2s_zero_dma_buffer(Config::ADC::I2S_NUM);
    i2s_set_clk(Config::ADC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
}


void setup_dac() {
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
    i2s_driver_install(Config::DAC::I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(Config::DAC::I2S_NUM, &pin_config);
    i2s_set_sample_rates(Config::DAC::I2S_NUM, Config::Sampling_Rate);
    i2s_zero_dma_buffer(Config::DAC::I2S_NUM);
    i2s_set_clk(Config::DAC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
}


void setup_i2s() {
    std::cout << "Initializing ADC..." << std::endl;
    setup_adc();
    std::cout << "Initializing DAC..." << std::endl;
    setup_dac();
}

/* Generate dummy master clock from pin 18 */
void setup_clock() {
    const ledc_channel_t channel = LEDC_CHANNEL_0;
    constexpr std::size_t freq = Config::Sampling_Rate * 256;
    constexpr std::array<int, 2> gpio_pins = {18, 19}; 

    ledc_timer_config_t ledc_timer {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_1_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = freq,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    ledc_timer_config(&ledc_timer);

    for(const auto gpio_pin : gpio_pins) {
        ledc_channel_config_t ledc_channel = {
            .gpio_num = gpio_pin,
            .speed_mode = LEDC_HIGH_SPEED_MODE,
            .channel = channel,
            .intr_type = LEDC_INTR_DISABLE,
            .timer_sel = LEDC_TIMER_0,        
            .duty = 1,
            .hpoint = 0,
        };
        ledc_channel_config(&ledc_channel);    
        ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 1);
        ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);  
    }
}

void vControlLoop(void* param) {
    constexpr TickType_t interval = 1000 / portTICK_PERIOD_MS;
    while (true)    {
        
        // static std::size_t freq = 10;
        // freq *= 2;

        // if(440 < freq) {
        //     freq = 10;
        // }

        // osc.setFreq(freq);

        vTaskDelay(interval);        
    }
}



extern "C" void app_main(void) {
    unit_test();

    setup_clock();
    setup_i2s();
    
    osc.setFreq(440);

    fifobuffer_t* buf = new fifobuffer_t;
    
    std::cout << "Buffer space::" << buf->has_space() << std::endl;
    std::cout << "Bit Range:" << Config::Bit_Range << ", INT32_Max: "<< INT32_MAX << std::endl;

    xTaskCreatePinnedToCore(vControlLoop, "ControlLoop", 4096, NULL, 0, NULL, tskNO_AFFINITY);

    xTaskCreatePinnedToCore(vAudioWriteTask, "AudioWriteLoop", 8192, buf, configMAX_PRIORITIES - 1, &writerTaskHandle, tskNO_AFFINITY); //tskNO_AFFINITY   
    // xTaskCreatePinnedToCore(vAudioReadThread, "AudioReedLoop", 8192, buf, configMAX_PRIORITIES - 2, NULL, tskNO_AFFINITY); //tskNO_AFFINITY   

    constexpr TickType_t interval = 100000 / portTICK_PERIOD_MS;
    while (true) {                
        vTaskDelay(interval);
    }
}