/*
- Helpful repo: https://github.com/atomic14/esp32_audio
*/

#include <Arduino.h>
#include <memory>

/* Original */
#include "App/ListeningApp.hpp"
#include "App/AppRunner.hpp"

/* Tests */
#include "test/UnitTest.h"


void setup() {    
    // std::cout << "PSRAM Found: " << psramFound() << std::endl
    UnitTest::run();

    // DeepListening* app = new DeepListening(); 
    ListeningApp* app = new ListeningApp();
    auto runner = std::make_unique<AppRunner>(app);
    runner->run();

    // DeepListening::setup();    
}

void loop() {}
