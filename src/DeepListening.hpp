#pragma once

#include "App/ListeningApp.hpp"

// #include <functional>
// #include "Ticker.h"

#include <cstddef>

#include "DSP/Oscillator.h"
#include "DSP/DelayLine.h"
// #include "DSP/Line.h"
#include "DSP/ADSR.h"

class DeepListening : public ListeningApp{
public:    
    void setup();        
    void dsp(const StereoSample&, StereoSample&);
    void control();    

    void bang1sec();
private:
    std::size_t previousTime;
    Oscillator osc;
    Oscillator lfo;

    StereoDelay del = StereoDelay(mstosamps(1000));

    // DSP::Line line;
    DSP::ADSR adsr;
    std::size_t counter;    

};

