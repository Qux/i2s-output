#pragma once

#include <cstddef>
#include <functional>

#include "Config.hpp"
#include "Types.hpp"
#include "Base/StereoSample.hpp"

#include "DeepListening.hpp"

class ListeningApp {
    public:
        virtual void setup() {            
            DeepListening::setup();
        }

        inline void dspLoop(Types::audiobuf_t& buf) noexcept {
            static StereoSample in, out;                        
            for(std::size_t i = 0; i <  Config::DMA::Buffer_Size; i++) {
                in.L = static_cast<float>(buf[2 * i]) * Config::Bit_Range_Reciprocal;   // covert int to float
                in.R = static_cast<float>(buf[2 * i + 1]) * Config::Bit_Range_Reciprocal;
                
                DeepListening::dsp(in, out);
                
                buf[2 * i] = static_cast<int>(out.L * Config::Bit_Range); // convert float to int
                buf[2 * i + 1] = static_cast<int>(out.R * Config::Bit_Range);                
            }            
        }

        inline void runDSP(Types::audiobuf_t& buf) noexcept {
            // this->inDSP = true;
            this->dspLoop(buf);
            // this->inDSP = false;
        }

        inline void controlLoop(void* p = nullptr) noexcept {
            while (true)  {                        
                // if(!this->inDSP)  { // not sure if we should do this
                    DeepListening::control();
                // }                
                vTaskDelay(Config::Control_Interval_Tick);    
            }
        }
        
    private:
        bool inDSP;                
};