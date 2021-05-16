#include "DeepListening.hpp"

#include <iostream>
#include <array>

#include "Ticker.h"

#include "DSP/Oscillator.hpp"
#include "FFT/FFT.hpp"

using namespace DeepListening;

Oscillator osc;
Ticker ticker;
FFT fft;

float vol;

static void displayFFT() {
    
}

static void func() {
    dsps_view(fft.getResult(), fft.size() >> 1, 64, 10,  0.0, 1.0, '|');

    const int pitch = random(36, 72);
    const float freq = mtof(pitch);

    osc.setFreq(freq);
}


void DeepListening::setup() {
    fft.init();

    osc.setFreq(440);
    osc.setWaveform(osc.Sin);
    
    ticker.attach_ms(1000, func);
}

void DeepListening::dsp(const StereoSample& in, StereoSample& out) {    
    const float val = osc.getNext();    
    out = val * vol;
    fft.add(out.L);
};

void DeepListening::control() {
    vol = fft.get(10); // get 10th freq bin.
}

