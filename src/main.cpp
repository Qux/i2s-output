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

/* Original */
#include "Config.h"
#include "Types.h"
#include "I2S/Writer.h"
#include "I2S/Reader.h"

#include "DSP/DelayLine.h"
#include "DSP/Oscillator.h"
#include "UnitTest.h"
#include "Util/RangedCounter.h"
#include "Util/Static_FIFO.h"



void setup_i2s();

void unit_test() {
    std::cout << "Bit Range:" << Config::Bit_Range << ", INT32_Max: "<< INT32_MAX << std::endl;
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

    typedef std::array<int, 3> tmptype;
    Static_FIFO<tmptype, 5> fifo2;
    tmptype t = {0, 1, 2};
    fifo2.push(t);
    const auto result = fifo2.pop();
    assert_equal(result[0], 0);
    assert_equal(result[1], 1);
    assert_equal(result[2], 2);

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

/*
void vAudioReadThread(void* param) {
    using namespace Types;
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

*/
void setup_adc() {}


void setup_dac() {}


void setup_i2s() {
    std::cout << "Initializing ADC..." << std::endl;
    setup_adc();
    std::cout << "Initializing DAC..." << std::endl;
    setup_dac();
}

/* Generate dummy master clock from pin 18 */
void setup_clock() {
    const ledc_channel_t channel = LEDC_CHANNEL_0;
    constexpr std::size_t freq = Config::Sampling_Rate * 128;
    constexpr std::array<int, 2> gpio_pins = {32, 33}; 

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

void updateControl() {

}



extern "C" void app_main(void) {
    unit_test();

    // setup_clock();

    using namespace Types;
    fifobuffer_t* buf = new fifobuffer_t;

    std::cout << "Reader init" << std::endl;
    I2S::Reader reader;
    reader.setBufferPtr(buf);

    std::cout << "Reader begin" << std::endl;
    reader.begin();
    
    std::cout << "Writer init" << std::endl;    
    // I2S::Writer writer(*buf);
    I2S::Writer writer;

    std::cout << "Writer begin" << std::endl;
    writer.setAudioBufferPtr(buf);
    writer.begin();
    
    constexpr TickType_t interval = 2 / portTICK_PERIOD_MS;
    while (true) {                
        updateControl();
        vTaskDelay(interval);
    }
}