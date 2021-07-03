#include "Oscillator.hpp"
#include "Config.hpp"

Oscillator::Oscillator(Waveform _waveform) {
        phase = 0.0;
        freq = 0;
        duty = 0.5;
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

void Oscillator::setDuty(float _duty) {
    if (_duty <= 0.0) {
        _duty = 0.001;
    } else if (_duty >= 1.0) {
        _duty = 0.999;
    }
    duty = _duty;
}

void Oscillator::setDC(const float _dc) {
    DC_value = _dc;
}

float Oscillator::getNext() { 
    /*
    - Proceeds the oscillator
    - returns -1.0 ~ 1.0
    */    
    

    phase += phase_inc; 
    while (TWO_PI <= phase) {
        phase -= TWO_PI;
    }

    switch (waveform) {
        case Sin:
            return sinf(phase);
            break;
        case Cos:
            return cosf(phase);
            break;
        case Triangle: {            
            float t = -1.0 + 2.0 * (0.5 * phase / ((1-duty) * TWO_PI) );
            if (t > 0) {
                t = -1.0/duty + 1.0 + 2.0 * (0.5 * phase / (duty * TWO_PI) );
            }
            return 2.0 * (fabsf(t) - 0.5);
            break;
        }            

        case Square:
            if (phase < duty * TWO_PI) {
                return 1.0;
            } else {
                return -1.0;
            }
            break;
        case Sawtooth:
            if (duty >= 0.5) {
                return ((phase * 2.0 * TWO_PI_RECIPROCAL) - 1);
            } else {
                return (-1) * ((phase * 2.0 * TWO_PI_RECIPROCAL) - 1);
            }
            break;
        case DC:
            return DC_value;
            break;
        case WhiteNoise: {
            constexpr long I24_Max = std::pow(2, 23);
            constexpr long I24_Min = -1 * std::pow(2, 23);
            constexpr float I24_Max_Reciprocal = 1.0 / static_cast<float>(I24_Max);

            return static_cast<float>(random(I24_Min, I24_Max)) * I24_Max_Reciprocal;
            break;
        }

        default:
            return 0.0f;
            break;
    }
    return 0.0;
}    

void Oscillator::reset() {
    phase = 0.0;
};