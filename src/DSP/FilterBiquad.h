#pragma once

#include "esp_dsp.h"

#define MAX_DLINE_SIZE 20
#include "DelayLine.h"

#ifndef FILTERBIQUAD_H
#define FILTERBIQUAD_H
#endif

class Biquad {
public:
    enum Filtertype {
        LowPass,
    };

    Biquad(Filtertype _filtertype = LowPass);

    void setFilterInfo(const Filtertype _filtertype, const float _freq, const float _qfactor);
    void setFiltertype(const Filtertype _filtertype);
    void setFreq(const float _freq);
    void setQFactor(const float _qfactor);
    void genCoefficients();
    float process(float in);
    void setArray(float *a);

private:
    MonoDelay dline = MonoDelay(MAX_DLINE_SIZE);
    Filtertype filtertype;
    float freq;
    float qfactor;
    float out;
    float coeffs_lpf[5];
};