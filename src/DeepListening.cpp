#include "DeepListening.hpp"

void DeepListening::setup() {
    osc.setFreq(440);
    lfo.setFreq(1.0);
    osc.setWaveform(osc.Triangle);
    previousTime = millis();
    // ticker.attach(1.000, std::bind(bang1sec));
}


void DeepListening::dsp(const StereoSample& in, StereoSample& out) {
    const float val = osc.getNext();    
    // const float lfoval = (lfo.getNext() + 1.0) * 0.5;

    out = val * pow(line(), 2.0); // float or StereoSample

    out.L += del.get(24000).L;
    del.add(in);
    line.next();
};

void DeepListening::control() {
    const std::size_t currentTime = millis();
    if(currentTime - previousTime >= 200) {
        osc.setFreq(random(110, 220));
        line(1.0, 0.2, 100);        
        previousTime = currentTime;        
    }
}

void DeepListening::bang1sec() {
    osc.setFreq(random(440, 880));
}