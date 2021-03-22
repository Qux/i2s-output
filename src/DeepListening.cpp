#include "DeepListening.hpp"

void DeepListening::setup() {
    osc.setFreq(440);
    osc.setWaveform(osc.Triangle);
    previousTime = millis();
}


void DeepListening::dsp(const float inL, const float inR, float& outL, float& outR) {
    const float val = osc.getNext();
    outR = val;
    outL = val;    
}

void DeepListening::control() {
    const std::size_t currentTime = millis();
    if(currentTime - previousTime >= 1000) {
        osc.setFreq(random(440, 880));
        previousTime = currentTime;
    }
}

