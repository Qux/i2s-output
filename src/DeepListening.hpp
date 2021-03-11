#pragma once

#include <Arduino.h>
#include "DSP/Oscillator.h"

class DeepListening {
public:
    DeepListening(){};
    ~DeepListening(){};
    void setup();
    void dsp(float&, float&);
    void dsp(const float, const float, float&, float&);
    void control();

private:
    int counter;
    Oscillator osc;
};

