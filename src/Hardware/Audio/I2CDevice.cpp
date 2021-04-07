#include "I2CDevice.h"

I2CDevice::I2CDevice(){
    device_address = 0;
}

I2CDevice::I2CDevice(const int _device_address)  {
     device_address = _device_address;
}

void I2CDevice::setup() {
    // Wire.setClock(100000);
    Wire.begin();  
}

void I2CDevice::setup(const int _device_address) {
    device_address = _device_address;
    Wire.begin();  
}

void I2CDevice::beginTransmission() {
    Wire.flush();
    Wire.beginTransmission(device_address);
}

int I2CDevice::endTransmission(bool sendStop) {
    return Wire.endTransmission(sendStop);
}

void I2CDevice::write(const int register_address, const int val) const {
    Wire.beginTransmission(device_address);
    Wire.write(register_address);
    Wire.write(val);    
    Wire.endTransmission();
}

void I2CDevice::bit_write(const int register_address, const int bit, const bool state) const {
    int value = this->read(register_address);

    if(state) {
        value |= (1 << bit);
    } else {
        value &= !(1 << bit);
    }
 
    this->write(register_address, value);
}

int I2CDevice::read(const int register_address) const {
    Wire.flush();
    Wire.beginTransmission(device_address);    
    Wire.write(register_address);      
    Wire.endTransmission(false);
    Wire.requestFrom(device_address, 1);        
        
    return Wire.read();
}

bool I2CDevice::bit_read(const int register_address, const std::size_t bit) const {
    return false;
}   