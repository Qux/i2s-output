#include "DeepListening.hpp"

void DeepListening::setup() {
    osc.setFreq(440);
    lfo.setFreq(1.0);
    previousTime = millis();
}


void DeepListening::dsp(const StereoSample& in, StereoSample& out) {
    const float val = osc.getNext();    
    const float lfoval = (lfo.getNext() + 1.0) * 0.5;

    out = val * lfoval; // float or StereoSample

    out.L += del.get(24000).L;
    del.add(in);
};

void DeepListening::control() {
    const std::size_t currentTime = millis();
    if(currentTime - previousTime >= 1000) {
        osc.setFreq(random(440, 880));
        previousTime = currentTime;
    }
}

