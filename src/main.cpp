/*
- Helpful repo: https://github.com/atomic14/esp32_audio
*/

#include <Arduino.h>
#include <memory>

/* Original */
#include "App/AppRunner.hpp"

/* Tests */
#include "test/UnitTest.h"

void setup() {    
    // std::cout << "PSRAM Found: " << psramFound() << std::endl
    UnitTest::run();

    // auto app = std::make_unique<DeepListening>();   
    DeepListening* app = new DeepListening(); 
    auto runner = std::make_unique<AppRunner>(app);
    runner->run();
}

void loop() {}
