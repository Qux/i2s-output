#include "DeepListening.hpp"

#include "Ticker.h"
#include "ArduinoOSC.h"

#include "DSP/Oscillator.hpp"
#include "DSP/ADSR.h"

using namespace DeepListening;

Oscillator osc;

DSP::ADSR adsr;
std::size_t counter;    


Ticker ticker;

static void func() {
    switch (counter)    {
    case 0: {
        const auto pitch = random(36, 72);
        const float freq = mtof(pitch);
        osc.setFreq(freq);
        adsr.noteOn();
        break;
    }
        
    case 1:
        adsr.noteOff();
        break;

    default:
        break;
    }

    counter++;
    if(counter == 3) {
        counter = 0;
    }
    
}


void DeepListening::setup() {
    
    osc.setFreq(440);
    osc.setWaveform(osc.Sin);
    
    adsr.set(10, 30, 0.1, 200);
    counter = 0;

    ticker.attach_ms(1000, func);
}

void DeepListening::dsp(const StereoSample& in, StereoSample& out) {    
    const float val = osc.getNext();    
    
    out = val * adsr.get();

    adsr.next();
};

void DeepListening::control() {

}

