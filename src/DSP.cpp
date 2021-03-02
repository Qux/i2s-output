#include <Arduino.h>
#include "DSP/Oscillator.h"
#include "DSP/DelayLine.h"

//Oscillator* osc = new Oscillator(1, Oscillator::Waveform::Sin);         

Oscillator osc(440, Oscillator::Waveform::Sin);         
DelayLine<float, 48000> del;


void DSP(const float in_L, const float in_R, float& out_l, float& out_r) {

    osc.setFreq(440);

    const float vol = osc.getNext();
    // const float vol = 1.0;

    // Lch = vol;
    out_l = in_L;
    out_r = in_R + del.get(24000); // somehow delay plays in the opposite channel

    // Rch += delayline.get(24000);

    // const float lval = in_R;
    del.add(vol);
}


