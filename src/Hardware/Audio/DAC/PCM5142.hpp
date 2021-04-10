#pragma once

#include "../Audio.h"
#include "../I2CDevice.h"

#include <iostream>
#include <bitset>

namespace Hardware  {
    namespace DAC {
        class PCM5142 : public I2CDevice {            
            public:
                PCM5142(const bool ADR2 = false, const bool ADR1 = false);   
                void setup();
                int setClockMode(const Audio::Clock_Mode);
                int setDataFormat(const Audio::Data_Format) const;
                int setWordLength(const Audio::Word_Length wl) const;
                void enable_PLL(const bool state) const; 
                int setSampleRate(const Audio::Sampling_Rate sr) const;
                int setSampleRate(const std::size_t sr) const;            
                void halfOutputRms(bool);
                void wakeup();

                void dump();
                static constexpr int getAddress(const bool ADR2 = false, const bool ADR1 = false) {
                    return 0b01001100 + (2 * ADR2) + ADR1;
                };
            private:
                int moveToPage(const std::size_t page) const;
        };
    }
}