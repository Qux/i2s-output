#pragma once


#include <Arduino.h>


#include <cmath>
#include "Config.h"

class Oscillator {
public: 
    enum Waveform {
        Sin, Cos, Triangle, Square, Sawtooth, DC,
    };

    Oscillator(Waveform _waveform = Sin);
    Oscillator(std::size_t _freq, Waveform _waveform = Sin);

    void setWaveform(const Waveform _waveform);
    void setFreq(const float _freq);
    void setDuty(float _duty);
    void setDC(const float);

    float getNext();
    void reset();

private:
    float freq;
    float freq_reciprocal;  // reciprocal of the freqency to redule division.
    float phase_inc;  // increment value of phase per sample (everytime getNext() called).
    float phase;    // 0.0 ~ 2pi.
    float duty; // 0.0 < duty < 1.0
    float DC_value;
    Waveform waveform;

    // If Arduino.h is included this will not be called.
    #ifndef TWO_PI  
    static constexpr float TWO_PI = M_PI * 2.0;    
    #endif // !TWO_PI

    static constexpr float TWO_PI_RECIPROCAL = 1.0 / TWO_PI;
};