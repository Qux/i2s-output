#include "DeepListening.hpp"
using namespace DeepListening;

#include "DSP/Oscillator.hpp"
#include "DSP/DelayLine.h"

StereoDelay del(mstosamps(1000));

void DeepListening::setup() {    
    del.fill(0);
}

void DeepListening::dsp(const StereoSample& in, StereoSample& out) {    
    out = in;
    out += del.get(mstosamps(500));
    del.add(out);
    
};

void DeepListening::control() {

}

