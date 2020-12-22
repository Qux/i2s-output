#pragma once
// #include <Arduino.h>

#include "Config.h"

#include <cmath>

class Oscillator {
public: 
    enum Waveform {
        Sin,
        Cos,
        Triangle,
        Square,
        Sawtooth,
    };

    Oscillator(Waveform _waveform = Sin) {
        phase = 0.0;
        freq = 0;
        waveform = _waveform;
    }

    Oscillator(std::size_t _freq, Waveform _waveform = Sin) {
        freq = _freq;
        waveform = _waveform;
    }

    void setWaveform(const Waveform _waveform) {
        waveform = _waveform;
    }
    
    void setFreq(const float _freq) {
        freq = _freq;
        freq_reciprocal = 1.0 / freq;
        phase_inc = TWO_PI * freq * Config::Sampling_Rate_Reciprocal;
    }

    float getNext() { 
        /*
        - Proceeds the oscillator
        - returns -1.0 ~ 1.0
        */    
        float t = 0.0;
        float duty = 0.5; // 0.0 < duty < 1.0

        switch (waveform) {
            case Sin:
                phase += phase_inc; 

                if (TWO_PI <= phase) {
                    phase = 0.0;
                }
                return sin(phase);
                break;
            case Cos:
                phase += phase_inc; 

                if (TWO_PI <= phase) {
                    phase = 0.0;
                }
                return cos(phase);
                break;
            case Triangle:
                

                phase += phase_inc;
                if (TWO_PI <= phase) {
                    phase = 0.0;
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
            default:
                return 0.0;
                break;
        }
        return 0.0;
    }    

    void reset() {
        phase = 0.0;
    };
    

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