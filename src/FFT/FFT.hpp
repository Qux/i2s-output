#pragma once

#include <cstddef>
#include <array>
#include <functional>
#include <iostream>

#include "esp_system.h"

#include "esp_dsp.h"


// #include "Arduino.h"
#include "Config.h"
#include "Types.hpp"

#include "Util/RangedCounter.h"


class FFT {
    public:
    FFT(const RAM_Type ram_type = RAM_Type::SRAM);
    ~FFT();
    void init();    
    // void calculate(const float* input);
    // void calculate(const std::array<float, Config::FFT::Size>& input);
    void calculate();
    void add(const float val);
    float* getResult();

    private:
    void allocate(std::function<void*(size_t, size_t)> calloc_func);
    void deallocate();
    float* input;
    float* data;
    float* window;
    float* fft_result;
    static constexpr float FFT_Size_Reciprocal = 1.0 / Config::FFT::Size;

    std::size_t counter;        
};


inline void FFT::add(const float val) {
    input[counter] = val;
    counter++;
    if(counter == Config::FFT::Size) {
        calculate();
        counter = 0;
    }
}

inline float* FFT::getResult() {
    return fft_result;
}
