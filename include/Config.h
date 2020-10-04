#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>

namespace Config {
    constexpr int Sampling_Rate = 48000;
    constexpr float Sampling_Rate_Reciprocal = 1.0 / static_cast<float>(Sampling_Rate);
    constexpr int range = (std::pow(2, static_cast<int>(I2S_BITS_PER_SAMPLE_24BIT))/2 - 1);

    constexpr int Buffer_Length = 64;
    constexpr int SampleSize_Byte = 4; 
    constexpr int I2S_Write_Size = Buffer_Length * SampleSize_Byte;

    
};

#endif

