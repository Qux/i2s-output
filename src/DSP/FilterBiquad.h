#pragma once

#include "esp_dsp.h"

#include "DelayLine.h"

#ifndef FILTERBIQUAD_H
#define FILTERBIQUAD_H
#endif

class Biquad {
public:
    enum Filtertype {
        LowPass,
        BandPass,
        HighPass,
    };

    Biquad();

    void setFilterInfo(const Filtertype _filtertype, const float _freq, const float _qfactor);
    void setFiltertype(const Filtertype _filtertype);
    void setFreq(const float _freq);
    void setQFactor(const float _qfactor);
    void genCoefficients();
    void test_keisan(const float *input, float *output, int len, float *coef, float *w);
    float process(float in);
    StereoSample process(StereoSample sample);
    void setArray(float *a);

private:
    MonoDelay dline = MonoDelay(10); // TODO 本当はwindowsizeの取りうる最大値にしたい。
    Filtertype filtertype;
    float freq_normalized;
    float qfactor;
    float out;
    float coeff[5];
    float w_lpf[2];
};