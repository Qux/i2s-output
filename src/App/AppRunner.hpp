#pragma once

/** C++ **/
#include <iostream>

/* ESP32 */
#include "App/ListeningApp.hpp"

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

    void initADC();
    void initDAC();

    void beginThread();    
    void beginWiFi();

    void init();    
    void run();

private:    
    ListeningApp* app;    

    Hardware::DAC::PCM5242 dac;
    Hardware::ADC::PCM1863 adc;
};





