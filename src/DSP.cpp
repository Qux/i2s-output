#include "DSP/Oscillator.h"

//Oscillator* osc = new Oscillator(1, Oscillator::Waveform::Sin);         

Oscillator osc(1, Oscillator::Waveform::Sin);         

void DSP(float& Lch, float& Rch) {
    // static 
    osc.setFreq(440);

    const float vol = osc.getNext();
    // const float vol = 1.0;

    Lch = vol;
    Rch = vol;
}


