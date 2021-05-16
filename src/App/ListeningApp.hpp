#pragma once

// #include <Arduino.h>

#include <cstddef>
#include <functional>

#include "Config.hpp"
#include "Types.hpp"
#include "Base/StereoSample.hpp"
#include "FFT/FFT.hpp"
#include "ListeningData.hpp"

#include "DeepListening.hpp"

class ListeningApp {
    public:
        virtual void setup() {
            data.fft.init();
            DeepListening::setup(data);
        }

        inline void dspLoop(Types::audiobuf_t& buf) noexcept {
            static StereoSample in, out;                        
            for(std::size_t i = 0; i <  Config::DMA::Buffer_Size; i++) {
                in.L = static_cast<float>(buf[2 * i]) * Config::Bit_Range_Reciprocal;   // covert int to float
                in.R = static_cast<float>(buf[2 * i + 1]) * Config::Bit_Range_Reciprocal;

                data.fft.add(in.R * 0.5);                

                DeepListening::dsp(in, out, data);
                
                buf[2 * i] = static_cast<int>(out.L * Config::Bit_Range); // convert float to int
                buf[2 * i + 1] = static_cast<int>(out.R * Config::Bit_Range);                
            }            
        }

        static constexpr inline std::size_t mstosamps(const float ms) noexcept {
            return Config::Sampling_Rate * 0.001 * ms;
        }

        static constexpr inline float sampstoms(const std::size_t samps) noexcept {
            return samps * Config::Sampling_Rate_Reciprocal * 1000.0;
        }

        inline void runDSP(Types::audiobuf_t& buf) noexcept {
            // this->inDSP = true;
            this->dspLoop(buf);
            // this->inDSP = false;
        }

        inline void controlLoop(void* p = nullptr) noexcept {
            while (true)  {                        
                // if(!this->inDSP)  { // not sure if we should do this
                    DeepListening::control(data);
                // }                
                vTaskDelay(Config::Control_Interval);    
            }
        }
        
    private:
        bool inDSP;
        ListeningData data;
        FFT fft;
};