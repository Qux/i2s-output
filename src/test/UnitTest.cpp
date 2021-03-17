#include <Arduino.h>
#include <iostream>

#include "UnitTest.h"
#include "Config.h"
#include "DSP/DelayLine.h"
#include "Util/Static_FIFO.h"
#include "Util/RangedCounter.h"
#include "App/ListeningApp.hpp"

#include "StereoSampleTest.hpp"

void UnitTest::run() {
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
    
    DelayLine<int> idelay(10);    
    assert_equal(idelay.get(0), 0);
    idelay.add(1);    
    assert_equal(idelay.get(0), 1);
    idelay.add(2);
    assert_equal(idelay.get(0), 2);
    assert_equal(idelay.get(1), 1);

    std::cout << "Float Delay test beginning..." << std::endl;

    DelayLine<float> fdelay(10);    
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

    std::cout << "mstosamps test..." << std::endl;
    assert_equal(ListeningApp::mstosamps(1000), Config::Sampling_Rate, true);
    std::cout << "sampstoms test..." << std::endl;
    assert_equal(ListeningApp::sampstoms(48000), 1000.0);

    StereoSampleTest::run();

    std::cout << "finished Unit test " << std::endl;
}