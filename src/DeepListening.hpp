#pragma once

#include "App/ListeningApp.hpp"
#include "DSP/Oscillator.h"
#include "DSP/DelayLine.h"


class DeepListening : public ListeningApp{
public:    
    void setup();    
    void dsp(const float, const float, float&, float&);
    void control();    

private:
    std::size_t previousTime;
    Oscillator osc;

    Delay del = Delay(mstosamps(1000));
    
};

