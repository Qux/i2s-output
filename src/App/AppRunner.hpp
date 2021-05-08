#pragma once

// #include "Arduino.h"

/** C++ **/
#include <iostream>

#include "Config.hpp"
#include "Types.hpp"

#include "App/ListeningApp.hpp"
#include "ListeningData.hpp"
#include "DeepListening.hpp"

#include "Hardware/Audio/DAC/PCM5242.hpp"
#include "Hardware/Audio/DAC/PCM5142.hpp"

#include "Hardware/Audio/ADC/PCM1863.hpp"


class AppRunner {
public:
    AppRunner(ListeningApp* _app);
        
    void initReader();
    void initWriter();
    void beginThread();

    void initADC();
    void initDAC();
    

    void init();
    void runThreads();
    void run();

private:    
    ListeningApp* app;    

    Hardware::DAC::PCM5242 dac;
    Hardware::ADC::PCM1863 adc;
};





