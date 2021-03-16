#include "DeepListening.hpp"

void DeepListening::setup() {
    osc.setFreq(440);
    previousTime = millis();
}


void DeepListening::dsp(const float inL, const float inR, float& outL, float& outR) {
    const float val = osc.getNext();
    // outR = val;
    // outL = val;   

    outL = inL + val * 0.5;
    outR = del.get(24000); 

    del.add(inL);

}

void DeepListening::control() {
    const std::size_t currentTime = millis();
    if(currentTime - previousTime >= 1000) {
        osc.setFreq(random(440, 880));
        previousTime = currentTime;
    }
}

