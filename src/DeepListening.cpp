#include "DeepListening.hpp"

#include <iostream>
#include <array>

#include "Ticker.h"
#include "ArduinoOSC.h"

#include "DSP/Oscillator.hpp"
#include "DSP/DelayLine.h"
#include "DSP/Line.h"
#include "DSP/ADSR.h"
#include "DSP/Filter.h"

using namespace DeepListening;
// History history;

Oscillator osc;
Oscillator lfo;
Filter fil;

StereoDelay del = StereoDelay(mstosamps(1000));

DSP::Line line;
DSP::ADSR adsr;
std::size_t counter;    
Ticker ticker;

float* fft;

static void func() {
    OscWiFi.send("224.0.0.1", 50000, "/foo", 1);
}

void onFreqMessage(const OscMessage& m) {    
    const float freq = m.arg<float>(0);
    osc.setFreq(freq);
}

void DeepListening::setup(ListeningData& data) {
    osc.setFreq(440);
    lfo.setFreq(1.0);
    osc.setWaveform(osc.Sin);
    fil.setFiltertype(fil.LowPass);

    adsr.set(10, 30, 0.1, 200);
    counter = 0;

    ticker.attach_ms(1000, func);
    OscWiFi.subscribe(50000, "/freq", onFreqMessage);
}

void DeepListening::dsp(const StereoSample& in, StereoSample& out, const ListeningData& data) {    
    const float val = osc.getNext();    
    // const float lfoval = (lfo.getNext() + 1.0) * 0.5;    

    // out = fil.getNext(val);
    // out = in;
    out = val;

    // out += del.get(mstosamps(500)) * 0.5;    
    // del.add(out);
    // adsr.next();
};

void DeepListening::control(const ListeningData& data) {
    OscWiFi.update();
}

