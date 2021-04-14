#pragma once

#ifndef CONFIG_H
#define CONFIG_H

#ifndef Arduino_h
#include "Arduino.h"
#endif

#include <cmath>
#include <cstddef>
#include "driver/i2s.h"

// This enum should move to somewhere else. maybe inside Config?
enum Stream_State {
    InOut,
    Input_Only,
    Output_Only,
    No_Audio,
};

constexpr int Test_Value = 1;

namespace Config {
    constexpr std::size_t Sampling_Rate = 48000;
    constexpr float Sampling_Rate_Reciprocal = 1.0 / static_cast<float>(Sampling_Rate);

    constexpr std::size_t Samples_Per_Msec = Sampling_Rate / 1000;
    constexpr float Samples_Per_Msec_Reciprocal = 1.0 / static_cast<float>(Samples_Per_Msec);

    constexpr i2s_bits_per_sample_t Bit_Rate = I2S_BITS_PER_SAMPLE_32BIT;
    constexpr std::size_t Bit_Range = std::pow(2, static_cast<int>(Bit_Rate) - 1) - 1; // 8388607
    
    constexpr float Bit_Range_Reciprocal = 1.0 / static_cast<float>(Bit_Range);
    constexpr std::size_t MCLK_Freq = Sampling_Rate * 512;
    constexpr std::size_t Channels = 2;

    constexpr Stream_State Stream = Output_Only;  // InOut, Output_Only, Input_Only, No_Audio
    constexpr std::size_t Control_Interval_ms = 2;
    constexpr TickType_t Control_Interval = Control_Interval_ms / portTICK_PERIOD_MS;

    namespace DMA {
        constexpr std::size_t Buffer_Count = 4;
        constexpr std::size_t Buffer_Size = 512;
        constexpr std::size_t Buffer_Length = Channels * Buffer_Size;
        constexpr std::size_t Size_byte = sizeof(int);
        constexpr std::size_t I2S_Buffer_Size = Buffer_Length * Size_byte;
    }

    namespace Hardware{
        constexpr std::size_t Boot_Time = 1;
    }
    

    namespace ADC {
        const bool Use_I2C_Device = false; 
        const i2s_port_t I2S_NUM = I2S_NUM_0;

        namespace Pins {
            const int BCK = GPIO_NUM_12;
            const int WS = GPIO_NUM_14;
            const int DO = I2S_PIN_NO_CHANGE;
            const int DI = GPIO_NUM_13;
        }
    }    

    namespace DAC {
        const bool Use_I2C_Device = false;
        const i2s_port_t I2S_NUM = I2S_NUM_1;

        namespace Pins {
            const int BCK = GPIO_NUM_25;
            const int WS = GPIO_NUM_27;
            const int DO = GPIO_NUM_26;
            const int DI = I2S_PIN_NO_CHANGE;
        }
    }

    namespace History {
        constexpr std::size_t Size = 48000;
    }

    namespace FFT {
        constexpr std::size_t Size = 1024;
        constexpr std::size_t Overlap = 0;        
    }
};

#endif

