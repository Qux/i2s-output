#include "FilterBiquad.h"

Biquad::Biquad(Filtertype _filtertype, const float _freq, const float _qfactor) {
    setFilterInfo(_filtertype, _freq, _qfactor);
    out      = 0.0;
    w_lpf[0] = 0.0;
    w_lpf[1] = 0.0;
}

void Biquad::setFilterInfo(const Filtertype _filtertype, const float _freq, const float _qfactor) {
    filtertype = _filtertype;
    setFreq(_freq);
    qfactor    = _qfactor;
    genCoefficients(coeff);
    //TODO レゾナンス付きの場合、coeffresの係数生成もする
}

//////////////////////////////////////////////////////////////////////////
// setFreq: freqを0 ~ SamplingRate/2で扱い、自動で0 ~ 0.5に正規化する
// _freq: frequency (0 - SamplingRate/2.0)
// freq_normalized = freq * Config::Sampling_Rate_Reciprocal
//////////////////////////////////////////////////////////////////////////
void Biquad::setFreq(const float _freq) {
    if(_freq < 0.0) {
        freq_normalized = 0.0;
    } else if(_freq > 0.5 * Config::Sampling_Rate) {
        freq_normalized = 0.5; 
    } else {
        freq_normalized = _freq * Config::Sampling_Rate_Reciprocal;
    }
}

void Biquad::debug_auto_setfreq() {
    static float _freq = 10.0;
    _freq *= 2.0;
    if (_freq> 20000) {
        _freq = 10.0;
    }
    std::cout << _freq << std::endl;
    setFreq(_freq);
    genCoefficients(coeff);
}

void Biquad::genCoefficients(float *_coeff) {
    esp_err_t ret = ESP_OK;
    switch(filtertype) {
        case LowPass:
           ret = dsps_biquad_gen_lpf_f32(_coeff, freq_normalized, qfactor); 
           break;
        case BandPass:
           ret = dsps_biquad_gen_bpf0db_f32(_coeff, freq_normalized, qfactor); 
           break;
        case HighPass:
           ret = dsps_biquad_gen_hpf_f32(_coeff, freq_normalized, qfactor);
           break;
        case LowPassWithRes:
           ret += dsps_biquad_gen_lpf_f32(_coeff, freq_normalized, qfactor); 
           ret += dsps_biquad_gen_peakingEQ_f32(&_coeff[5] , freq_normalized, qfactor);
           break;
        case PeakingEQ:
           ret = dsps_biquad_gen_peakingEQ_f32(_coeff , freq_normalized, qfactor);
           break;
        default:
           ret = dsps_biquad_gen_lpf_f32(_coeff, freq_normalized, qfactor); 
           break;
    }
    if(ret != ESP_OK) {
        std::cout << "[error]: genCoefficients function" << std::endl;
    }
}

float Biquad::process(float in) {
    esp_err_t ret = ESP_OK;
    float temp;
    switch(filtertype) {
        case LowPassWithRes:
            ret += dsps_biquad_f32_ae32(&in, &temp, 1, coeff, w_lpf);
            ret += dsps_biquad_f32_ae32(&temp, &out, 1, &coeff[5], w_lpf);
            if (ret != ESP_OK) {
                std::cout << "[error]: dsps_biquad_f32_ae32 function" << std::endl;
            }
        default:
            ret = dsps_biquad_f32_ae32(&in, &out, 1, coeff, w_lpf); // ae32 is optimized for esp32. In other case, use ansi.
            if (ret != ESP_OK) {
                std::cout << "[error]: dsps_biquad_f32_ae32 function" << std::endl;
            }
    }
    return out;
}

StereoSample Biquad::process(StereoSample sample) {
    StereoSample out;
    out.L = Biquad::process(sample.L);
    out.R = Biquad::process(sample.R);
    return out;
}