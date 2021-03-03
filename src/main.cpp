/*
- Helpful repo: https://github.com/atomic14/esp32_audio

*/

#include <Arduino.h>

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
#include "esp32-hal-ledc.c"
#include "esp_system.h"
// #include "esp32/spiram.h"
// #include "esp32-hal-psram.h"

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


void updateControl() {}


void setup() {    
    std::cout << "PSRAM Found: " << psramFound() << std::endl;
    
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

void loop() {

}
