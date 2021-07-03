#pragma once

#ifndef TWO_PI  
#include <cmath>
#endif

class Oscillator {
public: 
    enum Waveform {
        Sin, Cos, Triangle, Square, Sawtooth, DC, WhiteNoise,
    };

    Oscillator(Waveform _waveform = Waveform::Sin);
    Oscillator(std::size_t _freq, Waveform _waveform = Waveform::Sin);

    void setWaveform(const Waveform _waveform);
    void setFreq(const float _freq);
    void setDuty(float _duty);
    void setDC(const float);

    float getNext();
    void reset();

private:
    volatile float freq;
    volatile float freq_reciprocal;  // reciprocal of the freqency to redule division.
    volatile float phase_inc;  // increment value of phase per sample (everytime getNext() called).
    volatile float phase;    // 0.0 ~ 2pi.
    float duty; // 0.0 < duty < 1.0
    float DC_value;
    Waveform waveform;
    
    // If Arduino.h is included this will not be called.
    #ifndef TWO_PI  
    static constexpr float TWO_PI = M_PI * 2.0;    
    #endif // !TWO_PI

    static constexpr float TWO_PI_RECIPROCAL = 1.0 / TWO_PI;
};