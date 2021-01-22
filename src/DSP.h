#ifndef DSP_H_
#define DSP_H_

#pragma once

#include "DSP/Oscillator.h"



inline void DSP(float& Lch, float& Rch) {
    static Oscillator* osc = new Oscillator(1, Oscillator::Waveform::Sin);         
    osc->setFreq(1);
    const float vol = osc->getNext();
    // std::cout << "Vol: " << vol << std::endl;
    // Lch *= -1;
    // Rch *= -1;

    // Lch >>= 8;
    // Rch = 8;

    // int output = static_cast<int>(vol * range_max);
    // Lch = vol;
    // Rch = vol;
    Lch *= vol;
    Rch *= vol;
}


#endif