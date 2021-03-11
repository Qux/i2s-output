#pragma once

#ifndef CONFIG_H
#define CONFIG_H

// #ifndef Arduino_h
#include <Arduino.h>
// #endif

#include <cmath>
#include <cstddef>
#include "driver/i2s.h"

enum Stream_State {
    Use_InOut,
    Input_Only,
    Output_Only,
    No_Audio,
};

namespace Config {
    constexpr std::size_t Sampling_Rate = 48000;
    constexpr float Sampling_Rate_Reciprocal = 1.0 / static_cast<float>(Sampling_Rate);
    constexpr i2s_bits_per_sample_t Bit_Rate = I2S_BITS_PER_SAMPLE_32BIT;
    constexpr std::size_t Bit_Range = std::pow(2, static_cast<int>(Bit_Rate) - 1) - 1; // 8388607
    
    constexpr float Bit_Range_Reciprocal = 1.0 / static_cast<float>(Bit_Range);
    constexpr std::size_t MCLK_Freq = Sampling_Rate * 512;
    constexpr std::size_t Channels = 2;

    constexpr Stream_State Stream = Stream_State::Output_Only;  // Use_InOut, 
    constexpr std::size_t Control_Interval_ms = 2;
    constexpr TickType_t Control_Interval = Control_Interval_ms / portTICK_PERIOD_MS;

    namespace ADC {
        const i2s_port_t I2S_NUM = I2S_NUM_0;

        namespace DMA {
            constexpr std::size_t Buffer_Count = 4;
            constexpr std::size_t Buffer_Length = Channels * 512;
            constexpr std::size_t Size_byte = sizeof(int);
            constexpr std::size_t I2S_Buffer_Size = Buffer_Length * Size_byte;
        }

        namespace Pins {
            const int BCK = GPIO_NUM_13;
            const int WS = GPIO_NUM_14;
            const int DO = I2S_PIN_NO_CHANGE;
            const int DI = GPIO_NUM_15;
        }
    }    

    namespace DAC {
        const i2s_port_t I2S_NUM = I2S_NUM_1;

        namespace DMA {
            constexpr std::size_t Buffer_Count = 4;
            constexpr std::size_t Buffer_Length = 512;
            constexpr std::size_t Size_byte = sizeof(int); 
            constexpr std::size_t I2S_Buffer_Size = Buffer_Length * Channels * Size_byte;
        }

        namespace Pins {
            const int BCK = GPIO_NUM_27;
            const int WS = GPIO_NUM_25;
            const int DO = GPIO_NUM_26;
            const int DI = I2S_PIN_NO_CHANGE;
        }
    }
};

#endif

