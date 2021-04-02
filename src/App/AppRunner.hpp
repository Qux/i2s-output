#pragma once

#include <Arduino.h>

/** C++ **/
#include <iostream>

#include "Config.h"
#include "Types.h"

#include "App/ListeningApp.hpp"
#include "DeepListening.hpp"

#include "I2S/Writer.h"
#include "I2S/Reader.h"


class AppRunner {
public:
    AppRunner(ListeningApp* _app);
        
    void initReader();
    void initWriter();

    void init();
    void run();

private:    
    ListeningApp* app;
    Types::fifobuffer_t* buf;
    I2S::Reader* reader;
    I2S::Writer* writer;
};





