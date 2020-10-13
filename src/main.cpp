/** C++ **/
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <array>
#include <limits>
#include <iostream>
#include <functional>
#include <thread>

/* ESP32 */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "esp_system.h"
#include "esp32/spiram.h"

/* Original */
#include "Config.h"
#include "DSP/DelayLine.h"
#include "DSP/Oscillator.h"
#include "UnitTest.h"
#include "Util/Static_FIFO.h"

typedef std::array<int32_t, Config::Buffer_Length * Config::Channels> audiobuf_t;
typedef Static_FIFO<audiobuf_t, Config::Buffer_Count> fifobuffer_t;

void unit_test() {
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

    std::cout << "Float Delay testing..." << std::endl;

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

    std::cout << "Static FIFO testing..." << std::endl;
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

    std::cout << "finished Unit test " << std::endl;
}


// void process_audio() {
//     static std::array<int32_t, Config::Buffer_Length * Config::Channels> buf;
//     std::size_t i2s_bytes_read = 0;
//     i2s_read(Config::ADC::I2S_NUM, buf.data(), Config::I2S_Buffer_Size, &i2s_bytes_read, portMAX_DELAY);

//     for(std::size_t i = 0; i < Config::Buffer_Length; i++) {
//         dsp(static_cast<float>(buf[2 * i]) * Config::Bit_Range_Reciprocal, static_cast<float>(buf[2 * i + 1]) * Config::Bit_Range_Reciprocal);
//     }
    
//     size_t i2s_bytes_write = 0;        
//     i2s_write(Config::DAC::I2S_NUM, buf.data(), Config::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);
// }


void vAudioReadThread(void* param) {
    fifobuffer_t* buf = (fifobuffer_t*)param;
    while(true) {
        if(buf->has_space()) {
            // static audiobuf_t tmpbuf;
            std::size_t i2s_bytes_read = 0;
            i2s_read(Config::ADC::I2S_NUM, buf->push(), Config::I2S_Buffer_Size, &i2s_bytes_read, 100);  
        }      
    }
}


void vAudioWriteThread(void* param) {       
    fifobuffer_t* buf = (fifobuffer_t*)param;
    printf("IN THREAD: %p\n", buf);
    while(true) {
        if(buf->has_queue() != 0 ) {   
            audiobuf_t& samples = buf->pop();
            // DSP HERE
            size_t i2s_bytes_write = 0;
            i2s_write(Config::DAC::I2S_NUM, samples.data(), Config::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);
        }
        vTaskDelay(1);
    }
}

void vAudioLoop(void* param) {
    // static fifobuffer_t buf;      
    // // xTaskCreatePinnedToCore(vAudioReadThread, "Audio Read Loop", 4096, &buf, 0, NULL, 0); //tskNO_AFFINITY   
    // xTaskCreatePinnedToCore(vAudioWriteThread, "Audio Write Loop", 4096, &buf, 0, NULL, 0); //tskNO_AFFINITY   
    // printf("OUT THREAD: %p\n", &buf);
    // while(true) {        
    //     if(buf.has_space()) {
    //         // static audiobuf_t tmpbuf;
    //         std::size_t i2s_bytes_read = 0;
    //         i2s_read(Config::ADC::I2S_NUM, buf.push(), Config::I2S_Buffer_Size, &i2s_bytes_read, portMAX_DELAY);  
    //     }   
    //     vTaskDelay(1);    
    // }

    static audiobuf_t buf;
    while(true) {
        std::size_t i2s_bytes_read = 0;
        i2s_read(Config::ADC::I2S_NUM, buf.data(), Config::I2S_Buffer_Size, &i2s_bytes_read, portMAX_DELAY);
        
        size_t i2s_bytes_write = 0;        
        i2s_write(Config::DAC::I2S_NUM, buf.data(), Config::I2S_Buffer_Size, &i2s_bytes_write, portMAX_DELAY);
    }
}

void setup_adc() {
    i2s_config_t i2s_config {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_SLAVE | I2S_MODE_RX),                                   // Only TX
        .sample_rate = Config::Sampling_Rate,
        .bits_per_sample = Config::Bit_Rate,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = I2S_COMM_FORMAT_I2S_MSB,  //I2S_COMM_FORMAT_STAND_MSB - probably version thing
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                               //Interrupt level 1
        .dma_buf_count = Config::Buffer_Count,
        .dma_buf_len = Config::Buffer_Length * Config::Channels, //64,
        .use_apll = true,
        .tx_desc_auto_clear = false,
        .fixed_mclk = 0, //Config::Sampling_Rate * 256,
    };
    i2s_pin_config_t pin_config = {
        .bck_io_num = Config::ADC::Pins::BCK,
        .ws_io_num = Config::ADC::Pins::WS,
        .data_out_num = Config::ADC::Pins::DO,  //Not used
        .data_in_num = Config::ADC::Pins::DI               
    };
    i2s_driver_install(Config::ADC::I2S_NUM, &i2s_config, 0, NULL);
    i2s_set_pin(Config::ADC::I2S_NUM, &pin_config);
    i2s_set_clk(Config::ADC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
}

void setup_dac() {
    //for 36Khz sample rates, we create 100Hz sine wave, every cycle need 36000/100 = 360 samples (4-bytes or 8-bytes each sample)
    //depend on bits_per_sample
    //using 6 buffers, we need 60-samples per buffer
    //if 2-channels, 16-bit each channel, total buffer is 360*4 = 1440 bytes    16bit (= 2byte) * 2ch = 4  
    //if 2-channels, 24/32-bit each channel, total buffer is 360*8 = 2880 bytes
    i2s_config_t i2s_config = {
        .mode = static_cast<i2s_mode_t>(I2S_MODE_MASTER | I2S_MODE_TX),                                   // Only TX
        .sample_rate = Config::Sampling_Rate,
        .bits_per_sample = Config::Bit_Rate,
        .channel_format = I2S_CHANNEL_FMT_RIGHT_LEFT,                           //2-channels
        .communication_format = I2S_COMM_FORMAT_I2S,  //I2S_COMM_FORMAT_STAND_MSB - probably version thing
        .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,                               //Interrupt level 1
        .dma_buf_count = Config::Buffer_Count,
        .dma_buf_len = Config::Buffer_Length * Config::Channels,//64,   samples
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
    i2s_set_clk(Config::DAC::I2S_NUM, Config::Sampling_Rate, Config::Bit_Rate, I2S_CHANNEL_STEREO);
}

void setup_i2s() {
    std::cout << "Initializing ADC..." << std::endl;
    setup_adc();
    std::cout << "Initializing DAC..." << std::endl;
    setup_dac();
}

void setup_clock() {
    const ledc_channel_t channel = LEDC_CHANNEL_0;
    const std::size_t freq = 12288000;
    const int gpio_pin = 18; 

    ledc_timer_config_t ledc_timer {
        .speed_mode = LEDC_HIGH_SPEED_MODE,
        .duty_resolution = LEDC_TIMER_1_BIT,
        .timer_num = LEDC_TIMER_0,
        .freq_hz = freq,
        .clk_cfg = LEDC_AUTO_CLK,
    };

    ledc_timer_config(&ledc_timer);

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
    // ledc_fade_func_install(0);
    // ledc_fade_start(ledc_channel.speed_mode, ledc_channel.channel, LEDC_FADE_NO_WAIT);
    ledc_set_duty(ledc_channel.speed_mode, ledc_channel.channel, 1);
    ledc_update_duty(ledc_channel.speed_mode, ledc_channel.channel);   
}

void vControlLoop(void* param) {
    constexpr TickType_t interval = 1000 / portTICK_PERIOD_MS;
    while (true)    {
        vTaskDelay(interval);        
    }
}



extern "C" void app_main(void) {
    unit_test();
    // vTaskDelay(100 / portTICK_PERIOD_MS);

    setup_clock();
    setup_i2s();

    // vTaskDelay(100 / portTICK_PERIOD_MS);

    xTaskCreatePinnedToCore(vControlLoop, "ControlLoop", 4096, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(vAudioLoop, "AudioLoop", 4096, NULL, 0, NULL, 0); //tskNO_AFFINITY   

    constexpr TickType_t interval = 100000 / portTICK_PERIOD_MS;
    while (true) {                
        vTaskDelay(interval);
    }
}