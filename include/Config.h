#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#include <cmath>
#include "driver/i2s.h"
#include <cstddef>

namespace Config {
    constexpr int Sampling_Rate = 48000;
    constexpr float Sampling_Rate_Reciprocal = 1.0 / static_cast<float>(Sampling_Rate);
    constexpr i2s_bits_per_sample_t Bit_Rate = I2S_BITS_PER_SAMPLE_32BIT;
    constexpr std::size_t Bit_Range = (std::pow(2, static_cast<int>(Bit_Rate))/2 - 1);
    constexpr float Bit_Range_Reciprocal = 1.0 / static_cast<float>(Bit_Range);
    
    constexpr std::size_t Channels = 2;
    constexpr std::size_t Buffer_Count = 4;
    constexpr std::size_t Buffer_Length = 128;
    constexpr std::size_t SampleSize_Byte = 4; 
    constexpr std::size_t I2S_Buffer_Size = Buffer_Length * Channels * SampleSize_Byte;

    namespace ADC {
        const i2s_port_t I2S_NUM = I2S_NUM_0;
        namespace Pins {
            const int BCK = 13;
            const int WS = 16;
            const int DO = I2S_PIN_NO_CHANGE;
            const int DI = 14;
        }
    }    

    namespace DAC {
        const i2s_port_t I2S_NUM = I2S_NUM_1;
        namespace Pins {
            const int BCK = 26;
            const int WS = 25;
            const int DO = 22;
            const int DI = I2S_PIN_NO_CHANGE;
        }
    }


};

#endif

