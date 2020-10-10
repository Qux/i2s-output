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

    namespace ADC {
        const i2s_port_t I2S_NUM = I2S_NUM_0;
        namespace Pins {
            const int BCK = 13;
            const int WS = 15;
            const int DO = 12;
            const int DI = I2S_PIN_NO_CHANGE;
        }
    }    
    
    namespace DAC {
        const i2s_port_t I2S_NUM = I2S_NUM_1;
        namespace Pins {
            const int BCK = 26;
            const int WS = 25;
            const int DO = I2S_PIN_NO_CHANGE;
            const int DI = 22;
        }
    }
};

#endif

