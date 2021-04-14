/*
- Helpful repo: https://github.com/atomic14/esp32_audio
*/

#include "Arduino.h"
#include <memory>

/* Original */
#include "App/ListeningApp.hpp"
#include "App/AppRunner.hpp"

/* Tests */
#include "test/UnitTest.h"


extern "C" void app_main() {
    initArduino();
    UnitTest::run();
        
    ListeningApp* app = new ListeningApp();
    auto runner = std::make_unique<AppRunner>(app);
    runner->init();

    runner->run();
}