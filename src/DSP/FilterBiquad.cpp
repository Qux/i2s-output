#include "FilterBiquad.h"

Biquad::Biquad(Filtertype _filtertype) {
    filtertype = _filtertype;
    out = 0.0;
}

void Biquad::setFilterInfo(const Filtertype _filtertype, const float _freq, const float _qfactor) {
    filtertype = _filtertype;
    freq = _freq;
    qfactor = _qfactor;
}

void Biquad::genCoefficients() {
    esp_err_t ret = ESP_OK;
    switch(filtertype) {
        case LowPass:
           ret = dsps_biquad_gen_lpf_f32(coeffs_lpf, freq, qfactor); 
           break;
        default:
            break;
    }
}

float Biquad::process(float in) {
    esp_err_t ret = ESP_OK;
    float input[MAX_DLINE_SIZE];
    float output[MAX_DLINE_SIZE];
    float w_lpf[2] = {0.0, 0.0};
    dline.add(in);
    setArray(input);
    ret = dsps_biquad_f32_ae32(input, output, MAX_DLINE_SIZE, coeffs_lpf, w_lpf); 
    for(int8_t i = MAX_DLINE_SIZE - 1; i <= 0; i--) {
        dline.add(i);
    }
    return dline.get(0);
}

void Biquad::setArray(float *a) {
    for(int8_t i = 0; i < MAX_DLINE_SIZE; i++) {
        a[i] = dline.get(i);
    }
}

// void Biquad::setFiltertype(const Filtertype _filtertype) { filtertype = _filtertype;
// }
// void Biquad::setFreq(const float _freq) {
//     freq = _freq;
// }
// void Biquad::setQFactor(const float _qfactor) {
//     qfactor = _qfactor;
// }
