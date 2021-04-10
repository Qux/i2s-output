#pragma once

#include "../Audio.h"
#include "../I2CDevice.h"

// Debug
#include <iostream>
#include <bitset>


namespace Hardware  {
    namespace DAC {
        class PCM5242 : public I2CDevice {            
            public:
            PCM5242(const bool _ADR2 = false, const bool _ADR1 = false);               
            void setup();            
            int setClockMode(Audio::Clock_Mode);            
            int setDataFormat(const Audio::Data_Format) const;
            int setWordLength(const Audio::Word_Length wl) const;
            int use16xInterpolation(const bool state = true) const;
            int setSampleRate(const Audio::Sampling_Rate sr) const;
            int setSampleRate(const std::size_t sr) const;            
            void halfOutputRms(bool);
            void enable_PLL(const bool);
            void dump();

            void wakeup();

            static constexpr int getAddress(const bool ADR2 = false, const bool ADR1 = false) {
                return 0b1001100 + (2 * ADR2) + ADR1;       
            };
        private:
            int moveToPage(const std::size_t page) const;
            // int bit_write(const int register_address, const int bit, const bool state) const

        };
    }
}
