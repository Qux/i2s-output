#pragma once

/*
#include "App/ListeningApp.hpp"

#include <functional>
#include <cstddef>

#include "Ticker.h"
#include "DSP/Oscillator.h"
#include "DSP/DelayLine.h"
#include "DSP/Line.h"
#include "DSP/ADSR.h"

class DeepListening : public ListeningApp{
public:    
    void setup();        
    void dsp(const StereoSample&, StereoSample&);
    void control();    

    void bang1sec() { std::cout << "foo" << std::endl;};
    
private:
    std::size_t previousTime;
    Oscillator osc;
    Oscillator lfo;

    StereoDelay del = StereoDelay(mstosamps(1000));

    // DSP::Line line;
    DSP::ADSR adsr;
    std::size_t counter;    

    Ticker ticker;
};
*/
#include "Config.h"
#include "Types.h"


namespace DeepListening {
    void setup();
    void dsp(const StereoSample& in, StereoSample& out);
    void control();
    
    extern History history;

    constexpr inline std::size_t mstosamps(float ms) {
        return Config::Sampling_Rate * 0.001 * ms;
    }

    constexpr inline float sampstoms(std::size_t samps) {
        return samps * Config::Sampling_Rate_Reciprocal * 1000.0;
    }
}