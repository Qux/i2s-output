#ifndef DSP_H_
#define DSP_H_

#pragma once

#include "DSP/Oscillator.h"

inline void DSP(float& Lch, float& Rch) {
    static Oscillator* osc = new Oscillator(1, Oscillator::Waveform::Sin);         
    osc->setFreq(440);

    const float vol = osc->getNext();
    // const float vol = 1.0;

    Lch = vol;
    Rch = vol;
}


#endif