/*
- Helpful repo: https://github.com/atomic14/esp32_audio
*/

#include <Arduino.h>
#include <memory>


/* Original */
#include "App/ListeningApp.hpp"
#include "App/AppRunner.hpp"

#include "Hardware/Audio/DAC/PCM5242.hpp"
#include "Hardware/Audio/DAC/PCM5142.hpp"

#include "Hardware/Audio/ADC/PCM1863.hpp"

/* Tests */
#include "test/UnitTest.h"

#include <bitset>

Hardware::DAC::PCM5242 dac;
Hardware::ADC::PCM1863 adc;

void setup() {    
    // std::cout << "PSRAM Found: " << psramFound() << std::endl
    UnitTest::run();
    
    /*
    std::cout << "DA setup begins" << std::endl;
    dac.setup();
    dac.enable_PLL(true);
    dac.setClockMode(Hardware::Audio::Clock_Mode::Master);
    dac.halfOutputRms(true);
    dac.wakeup();

    adc.setup();
    adc.setClockMode(Hardware::Audio::Clock_Mode::Master);
    adc.setMixer();
        
    std::cout << "DA setup done" << std::endl;

    */

    // DeepListening* app = new DeepListening(); 
    ListeningApp* app = new ListeningApp();
    auto runner = std::make_unique<AppRunner>(app);
    runner->init();

    // while(true) {
    //     delay(3000);
    //     dac.dump();
    // }


    runner->run();

    // DeepListening::setup();    
}

void loop() {}
