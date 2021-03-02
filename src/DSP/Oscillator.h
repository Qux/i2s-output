#pragma once

#ifndef Arduino_h
#include <Arduino.h>
#endif

#include <cmath>
#include "Config.h"

class Oscillator {
public: 
    enum Waveform {
        Sin,
        Cos,
        Triangle,
        Square,
        Sawtooth,
        DC,
    };

    Oscillator(Waveform _waveform = Sin);
    Oscillator(std::size_t _freq, Waveform _waveform = Sin);

    void setWaveform(const Waveform _waveform);
    void setFreq(const float _freq);
    float getNext();
    void reset();

private:
    float freq;
    float freq_reciprocal;  // reciprocal of the freqency to redule division.
    float phase_inc;  // increment value of phase per sample (everytime getNext() called).
    float phase;    // 0.0 ~ 2pi.
    Waveform waveform;

    // If Arduino.h is included this will not be called.
    #ifndef TWO_PI  
    static constexpr float TWO_PI = M_PI * 2.0;    
    #endif // !TWO_PI

};