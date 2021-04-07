#include "DeepListening.hpp"

#include "Ticker.h"
#include "DSP/Oscillator.h"
#include "DSP/DelayLine.h"
#include "DSP/Line.h"
#include "DSP/ADSR.h"

using namespace DeepListening;
History history;

std::size_t previousTime;
Oscillator osc;
Oscillator lfo;

StereoDelay del = StereoDelay(mstosamps(1000));

DSP::Line line;
DSP::ADSR adsr;
std::size_t counter;    
Ticker ticker;

static void func() {
    static bool state = true;
    if(state) {
        osc.setFreq(random(440, 880));
        adsr.noteOn();
    } else {
        adsr.noteOff();
    }

    state = !state;

}

void DeepListening::setup() {
    osc.setFreq(440);
    lfo.setFreq(1.0);
    osc.setWaveform(osc.Sin);
    previousTime = millis();

    adsr.set(10, 100, 0.3, 800);
    counter = 0;

    ticker.attach_ms(2000, func);
}

void DeepListening::dsp(const StereoSample& in, StereoSample& out) {
    const float val = osc.getNext();    
    // const float lfoval = (lfo.getNext() + 1.0) * 0.5;

    // out = val * adsr.get(); // float or StereoSample
    // out = val;
    
    out = in;
    out.L = val * 0.5 * adsr.get(); 
    
    // out.L += del.get(mstosamps(500)).L;
    // del.add(in);
    adsr.next();
};

void DeepListening::control() {
    // const std::size_t currentTime = millis();    
    // if(currentTime - previousTime >= 1000) {        
    //     // this->bang1sec();
    //     previousTime = currentTime;        
    // }
}

