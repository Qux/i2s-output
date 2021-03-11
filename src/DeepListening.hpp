#pragma once

#include <Arduino.h>
#include "DSP/Oscillator.h"


class DeepListening {
public:    
    void setup();    
    void dsp(const float, const float, float&, float&);
    void control();    

private:
    std::size_t previousTime;
    Oscillator osc;

};

