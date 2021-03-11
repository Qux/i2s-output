#include "DeepListening.hpp"

void DeepListening::setup() {
    osc.setFreq(440);
}


void DeepListening::dsp(float& lch, float& rch) {
    const float val = osc.getNext();
    rch = val;
    lch = val;
}

void DeepListening::dsp(const float inL, const float inR, float& outL, float& outR) {
    const float val = osc.getNext();
    outR = val;
    outL = val;    
}

void DeepListening::control() {
    
}