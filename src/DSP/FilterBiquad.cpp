#include "FilterBiquad.h"

Biquad::Biquad() {
    filtertype = BandPass;
    freq     = 0.15; // in range of 0.0 - 0.5
    qfactor  = 0.80;
    out      = 0.0;
    w_lpf[0] = 0.0;
    w_lpf[1] = 0.0;
    genCoefficients();
}

void Biquad::setFilterInfo(const Filtertype _filtertype, const float _freq, const float _qfactor) {
    filtertype = _filtertype;
    freq       = _freq;
    qfactor    = _qfactor;
    genCoefficients();
}
// TODO setFreq関数を用意して、freqを0 ~ SamplingRate/2で扱い、自動で0 ~ 0.5に正規化する機能をつける
void Biquad::setFreq(const float _freq) {
    if(_freq < 0.0) {
        freq = 0.0;
    } else if(_freq > Config::Sampling_Rate) {
        freq = Config::Sampling_Rate;
    } else {
        freq = _freq;
    }
}

void Biquad::genCoefficients() {
    esp_err_t ret = ESP_OK; // TODO retをちゃんと使う
    switch(filtertype) {
        case LowPass:
           ret = dsps_biquad_gen_lpf_f32(coeff, freq, qfactor); 
           break;
        case BandPass:
           ret = dsps_biquad_gen_bpf0db_f32(coeff, freq, qfactor); 
           break;
        case HighPass:
            ret = dsps_biquad_gen_hpf_f32(coeff, freq, qfactor);
            break;
        default:
           ret = dsps_biquad_gen_lpf_f32(coeff, freq, qfactor); 
            break;
    }
}

void Biquad::test_keisan(const float *input, float *output, int len, float *coef, float *w) {
    for (int i = 0 ; i < len ; i++) {
        float d0 = input[i] - coef[3] * w[0] - coef[4] * w[1];
        float outtemp = coef[0] * d0 +  coef[1] * w[0] + coef[2] * w[1];
        output[i] = outtemp;
        w[1] = w[0];
        w[0] = d0;
    }
}

float Biquad::process(float in) {
    esp_err_t ret = ESP_OK;
    ret = dsps_biquad_f32_ae32(&in, &out, 1, coeff, w_lpf); // ae32 is optimized for esp32. In other case, use ansi.
    return out;
}
