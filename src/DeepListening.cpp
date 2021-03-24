#include "DeepListening.hpp"

void DeepListening::setup() {
    osc.setFreq(440);
    lfo.setFreq(1.0);
    osc.setWaveform(osc.Triangle);
    previousTime = millis();

    adsr.set(10, 100, 0.3, 800);
    counter = 0;
}

void DeepListening::dsp(const StereoSample& in, StereoSample& out) {
    const float val = osc.getNext();    
    // const float lfoval = (lfo.getNext() + 1.0) * 0.5;

    out = val * adsr.get(); // float or StereoSample

    out.L += del.get(24000).L;
    del.add(in);
    adsr.next();
};

void DeepListening::control() {
    const std::size_t currentTime = millis();    
    if(currentTime - previousTime >= 1000) {        
        this->bang1sec();
        previousTime = currentTime;        
    }
}

void DeepListening::bang1sec() {
    // osc.setFreq(random(440, 880));
    switch (counter)    {
    case 0:
        osc.setFreq(random(440, 880));
        adsr.noteOn();
        counter++;
        break;
    case 1:
        adsr.noteOff();
        counter++;
        break;
    case 2:
        counter = 0;
        break;
    default:
        break;
    }
}