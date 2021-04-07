#include "PCM1863.hpp"

using namespace Hardware::ADC;
using namespace Hardware::Audio;

PCM1863::PCM1863(const bool AD1) : I2CDevice(PCM1863::getAddress(AD1)) {}

void PCM1863::setup() {    
    I2CDevice::setup();
}

void PCM1863::setup(const bool AD1) {
    I2CDevice::setup(PCM1863::getAddress(AD1));
}

void PCM1863::setSamplingRate(Sampling_Rate sr) {

}

void PCM1863::setClockMode(Clock_Mode clk_mode) {
    write(0, 0);
    
    write(1, 0b01010000); // L ch gain
    write(2, 0b01010000); // R ch gain
    
    write(11, 0b00000000);  // i2s settings

    write(40, 0b00000000); // disable PLL
    
    switch (clk_mode)    {
    case Clock_Mode::Master:
        write(32, 0b00000000); // use SCK as ADC clock 
        

        // bit_write(0x20, 4, true);
        break;
    
    default:
        break;
    }
}

void PCM1863::setClockRatio(SCK_Ratio sck_ratio) {

}

void PCM1863::setMixer(const Microphone_Type mode) const {
    write(0, 0);
    switch (mode)  {
    case Microphone_Type::Builtin_Microphones:
        write(6, 0b01010000); 
        write(7, 0b01010000);
        break;
    case Microphone_Type::External_Balanced_Microphones:
        write(6, 0b01010000); 
        write(7, 0b01010000);
        break;
    case Microphone_Type::Single_LR_Stereo:
        write(6, 0b01000100);
        write(7, 0b01000100);
        break;
    default:
        break;
    }
}

