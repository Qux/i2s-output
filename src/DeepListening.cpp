#include "DeepListening.hpp"

#include <iostream>
#include <array>


#include "Ticker.h"
#include "DSP/Oscillator.hpp"
#include "DSP/DelayLine.h"
#include "DSP/Line.h"
#include "DSP/ADSR.h"
#include "DSP/Filter.h"

using namespace DeepListening;
// History history;

std::size_t previousTime;
Oscillator osc;
Oscillator lfo;
Filter fil;

StereoDelay del = StereoDelay(mstosamps(1000));

DSP::Line line;
DSP::ADSR adsr;
std::size_t counter;    
Ticker ticker;

float* fft;

static void func() {
    static bool state = true;
    if(state) {
        const int pitch = random(72, 84);
        const float freq = mtof(pitch);        
        // osc.setFreq(freq);
        adsr.noteOn();
    } else {
        adsr.noteOff();
    }

    state = !state;
}


void DeepListening::setup(ListeningData& data) {
    osc.setFreq(4000);
    lfo.setFreq(1.0);
    osc.setWaveform(osc.Triangle);
    fil.setFiltertype(fil.LowPass);

    previousTime = millis();

    adsr.set(10, 30, 0.1, 200);
    counter = 0;

    ticker.attach_ms(1000, func);
}

void DeepListening::dsp(const StereoSample& in, StereoSample& out, const ListeningData& data) {

    const float val = osc.getNext();    
    // const float lfoval = (lfo.getNext() + 1.0) * 0.5;    

    out = fil.getNext(val);
    // out = val;
    // out = in;
    // out += del.get(mstosamps(500)) * 0.5;
    
    // out.L += del.get(mstosamps(500)).L;
    // del.add(out);
    // adsr.next();
};

void DeepListening::control(const ListeningData& data) {

}

