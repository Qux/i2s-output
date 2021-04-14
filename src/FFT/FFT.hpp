#pragma once

#include <cstddef>
#include "esp_system.h"

#include "esp_dsp.h"

#include "Arduino.h"
#include "Config.h"
#include "Types.hpp"


class FFT {
    public:
    FFT(RAM_Type ram_type = RAM_Type::SRAM) {
        switch (ram_type)   {
        case RAM_Type::PSRAM:
            data = static_cast<float*>(ps_calloc(Config::FFT::Size * sizeof(float), sizeof(float)));
            break;

        case RAM_Type::SRAM:
            data = static_cast<float*>(calloc(Config::FFT::Size * sizeof(float), sizeof(float)));
            break;        
        default:
            data = static_cast<float*>(ps_calloc(Config::FFT::Size * sizeof(float), sizeof(float)));
            break;
        };
        counter = 0;

        dsps_fft2r_init_fc32(NULL, Config::FFT::Size);        
    };
    ~FFT() {
        free(data);
    };
    void add(const float val) {
        data[counter] = val;
    };
    

    private:
    float* data;
    float* window;
    float* fft_result;
    std::size_t counter;
    
    
};