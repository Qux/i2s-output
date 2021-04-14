#pragma once

// #include <Arduino.h>
#include "Wire.h"

class I2CDevice {
    public:
        I2CDevice();
        I2CDevice(const int _device_address);
        virtual void setup();
        virtual void setup(const int _device_address);

        void beginTransmission();
        int endTransmission(bool sendStop = true);
        
        int write(const int register_address, const int val) const;
        void bit_write(const int register_address, const int bit, const bool val) const;

        int read(const int register_address) const;
        bool bit_read(const int register_address, const std::size_t bit) const;

    protected:
        int bit_flag(int bits, const uint8_t index) const;
        int bit_unflag(int bits, const uint8_t index) const;
    private:        
        int device_address;
};