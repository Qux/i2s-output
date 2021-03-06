#include "DeepListening.hpp"
using namespace DeepListening;

#include "Ticker.h"

#include "DSP/Oscillator.hpp"

Oscillator osc;
Ticker ticker;

static void func() {
    const auto pitch = random(36, 73);
    const float freq = mtof(pitch);

    osc.setFreq(freq);
}

void DeepListening::setup() {
    osc.setFreq(440);
    osc.setWaveform(osc.Sin);

    ticker.attach_ms(250, func);
}

void DeepListening::dsp(const StereoSample& in, StereoSample& out) {    
    const float val = osc.getNext();    
    out = val;
};

void DeepListening::control() {

}

