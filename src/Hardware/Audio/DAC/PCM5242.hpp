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
            // void setSampleingRate();
            int setClockMode(Audio::Clock_Mode);
            // void reset();
            // int setBitRate(const int Bit_Rate = 32);
            void setMixer();
            void halfOutputRms(bool);
            void enable_PLL(const bool);
            void dump();

            void wakeup();

            static constexpr int getAddress(const bool ADR2 = false, const bool ADR1 = false) {
                return 0b1001100 + (2 * ADR2) + ADR1;       
            };
        private:
            
            // int bit_write(const int register_address, const int bit, const bool state) const

        };
    }
}
