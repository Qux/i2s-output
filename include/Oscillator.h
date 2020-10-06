#include <Arduino.h>
#include "Config.h"

class Oscillator {
public:
    Oscillator() {
        phase = 0.0;
    }

    template<typename T>
    void setFreq(T _freq) {
        freq = static_cast<float>(_freq);
        freq_reciprocal = 1.0 / freq;
        phase_inc = TWO_PI * freq * Config::Sampling_Rate_Reciprocal;
    }

    float getNext() { //returns -1.0 ~ 1.0        
        phase += phase_inc; 

        if (TWO_PI <= phase) {
            phase = 0.0;
        }
        return sinf(phase);
    }    
    void reset() {
        phase = 0.0;
    };
    

private:
    float freq;
    float freq_reciprocal;
    float phase_inc;
    float phase;
};