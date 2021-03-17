#pragma once

#include "App/ListeningApp.hpp"

#include "DSP/Oscillator.h"
#include "DSP/DelayLine.h"


class DeepListening : public ListeningApp{
public:    
    void setup();        
    void dsp(const StereoSample&, StereoSample&);
    void control();    

private:
    std::size_t previousTime;
    Oscillator osc;

    StereoDelay del = StereoDelay(mstosamps(1000));
    
};

