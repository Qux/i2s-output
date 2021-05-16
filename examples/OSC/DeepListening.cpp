/*
- When you use OSC, turn WiFi on in Config.hpp and set your SSID and Password
*/

#include "DeepListening.hpp"

#include <iostream>
#include <array>

#include "Ticker.h"
#include "ArduinoOSC.h"

#include "DSP/Oscillator.hpp"

using namespace DeepListening;

Oscillator osc;

Ticker ticker;

static void func() {
    OscWiFi.send("224.0.0.1", 50000, "/foo", 1);
}

static void onFreqMessage(const OscMessage& m) {    
    const float freq = m.arg<float>(0);
    osc.setFreq(freq);
}

void DeepListening::setup() {
    osc.setFreq(440);

    osc.setWaveform(osc.Sin);

    ticker.attach_ms(1000, func);
    OscWiFi.subscribe(50000, "/freq", onFreqMessage);
}

void DeepListening::dsp(const StereoSample& in, StereoSample& out) {    
    const float val = osc.getNext();    
    out = val;
};

void DeepListening::control() {
    OscWiFi.update();
}

