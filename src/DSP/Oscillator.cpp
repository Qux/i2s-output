#include "Oscillator.h"


Oscillator::Oscillator(Waveform _waveform) {
        phase = 0.0;
        freq = 0;
        DC_value = 0;
        waveform = _waveform;
    }

Oscillator::Oscillator(std::size_t _freq, Waveform _waveform) {
    freq = _freq;
    waveform = _waveform;
}

void Oscillator::setWaveform(const Waveform _waveform) {
    waveform = _waveform;
}

void Oscillator::setFreq(const float _freq) {
    freq = _freq;
    freq_reciprocal = 1.0 / freq;
    phase_inc = TWO_PI * freq * Config::Sampling_Rate_Reciprocal;
}

void Oscillator::setDC(const float _dc) {
    DC_value = _dc;
}

float Oscillator::getNext() { 
    /*
    - Proceeds the oscillator
    - returns -1.0 ~ 1.0
    */    
    float t = 0.0;
    float duty = 0.5; // 0.0 < duty < 1.0

    switch (waveform) {
        case Sin:
            phase += phase_inc; 

            while (TWO_PI <= phase) {
                phase -= TWO_PI;
            }
            return sin(phase);
            break;
        case Cos:
            phase += phase_inc; 

            while (TWO_PI <= phase) {
                phase -= TWO_PI;
            }
            return cos(phase);
            break;
        case Triangle:
            

            phase += phase_inc;
            while (TWO_PI <= phase) {
                phase -= TWO_PI;
            }
            t = -1.0 + 2.0 * (0.5 * phase / ((1-duty) * TWO_PI) );
            if (t > 0) {
                t = -1.0/duty + 1.0 + 2.0 * (0.5 * phase / (duty * TWO_PI) );
            }
            return 2.0 * (fabsf(t) - 0.5);
            break;
        case Square:
            return 0;
            break;
        case Sawtooth:
            return 0;
            break;
        case DC:
            return DC_value;
        default:
            return 0.0;
            break;
    }
    return 0.0;
}    

void Oscillator::reset() {
    phase = 0.0;
};