#pragma once


/** C++ **/
#include <iostream>

/* ESP32 */
#include "Arduino.h"
#include "WiFi.h"


#include "Config.hpp"
#include "Types.hpp"

#include "App/ListeningApp.hpp"
#include "ListeningData.hpp"
#include "DeepListening.hpp"

/* ADC */
#include "Hardware/Audio/ADC/PCM1863.hpp"

/* DAC */
#include "Hardware/Audio/DAC/PCM5242.hpp"
#include "Hardware/Audio/DAC/PCM5142.hpp"


class AppRunner {
public:
    AppRunner(ListeningApp* _app);
        
    void initReader();
    void initWriter();
    void beginThread();

    void initADC();
    void initDAC();
    
    void beginWiFi();

    void init();
    void runThreads();
    void run();

private:    
    ListeningApp* app;    

    Hardware::DAC::PCM5242 dac;
    Hardware::ADC::PCM1863 adc;
};





