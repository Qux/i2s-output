#pragma once

#include "App/ListeningApp.hpp"

#include <functional>

#include "Ticker.h"

#include "DSP/Oscillator.h"
#include "DSP/DelayLine.h"
#include "DSP/Line.h"

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

    DSP::Line line;
    // Ticker ticker;
    // Control::Metro metro;
    // Event eve;
};

