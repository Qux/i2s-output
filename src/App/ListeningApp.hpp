#pragma once

#include <Arduino.h>

#include <cstddef>
#include <functional>

#include "Config.h"
#include "Types.h"
#include "Base/StereoSample.hpp"

#include "DeepListening.hpp"

class ListeningApp {
    public:
        virtual void setup() {
            DeepListening::setup();
        }

        virtual inline void dspLoop(Types::audiobuf_t& buf) {
            static StereoSample in, out;
            for(std::size_t i = 0; i < Config::DMA::Buffer_Size; i++) {
                in.L = static_cast<float>(buf[2 * i]) * Config::Bit_Range_Reciprocal;   // covert int to float
                in.R = static_cast<float>(buf[2 * i + 1]) * Config::Bit_Range_Reciprocal;

                // DeepListening::history.input.add(in);
                    
                DeepListening::dsp(in, out);

                buf[2 * i] = static_cast<int>(out.R * Config::Bit_Range); // convert float to int
                buf[2 * i + 1] = static_cast<int>(out.L * Config::Bit_Range);
                // DeepListening::history.input.add(out);
            }
        }

        static constexpr inline std::size_t mstosamps(float ms) {
            return Config::Sampling_Rate * 0.001 * ms;
        }

        static constexpr inline float sampstoms(std::size_t samps) {
            return samps * Config::Sampling_Rate_Reciprocal * 1000.0;
        }

        inline void runDSP(Types::audiobuf_t& buf) {
            this->inDSP = true;
            this->dspLoop(buf);
            this->inDSP = false;
        }

        inline void controlLoop() {
            while (true)  {                        
                if(!this->inDSP)  {
                    DeepListening::control();
                }
                // vPortYield();
                vTaskDelay(Config::Control_Interval);    
            }
        }
        
    private:
        bool inDSP;
};