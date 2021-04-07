
#include "PCM5142.hpp"

using namespace Hardware::Audio;
using namespace Hardware::DAC;

PCM5142::PCM5142(const bool ADR2, const bool ADR1) : I2CDevice(PCM5142::getAddress(ADR2, ADR1)) {

}

void PCM5142::setup() {
    I2CDevice::setup();    
}

int PCM5142::setClockMode(const Audio::Clock_Mode clock_mode) {
switch (clock_mode)    {
    case Clock_Mode::Master:        
        write(0, 0); // move to page 0
        write(2, 0b00010000); // enter standby mode
        write(1, 0b00010001);   //enter reset mode
        write(34, 00000000); // fs double speed
        write(40, 0b00000011);  // use i2s, 32bit
        
        write(37, 0b00100010); // disable auto clock 
        write(37, 0b00000000); // disable auto clock 

        // write(0, 253); // move to page 253
        // write(63, 0x11);    // enable flex clock
        // write(64, 0xFF);
        
        write(0, 0);        
        write(9, 0b00001001); // LRCK and BCK output

        write(12, 0x0C); // from datasheet
        write(14, 0x30);
        write(28, 0x1F);
            

        
        // write(14, 0x30); // set SCK as DAC clock
        write(30, 0x30);
        write(28, 31);
        write(32, 64); // BCK clock
        write(33, 0b00011111); // LRCK clock
        
        write(65, 0b00000100); //disable auto mute

        write(12, 0b00000011); // BCK and LRCK devider functional

        write(1, 0b00000000);  //exit reset mode?
        write(2, 0b00000000); // exit standby mode


        // return endTransmission();
        return 0;
        break;
    
    default:
        return 255;
    }
}

void PCM5142::enable_PLL(const bool state) const {
    write(0, 0);
    write(0x04, state);
};

void PCM5142::wakeup() {
        write(0x00, 0); // move to page 0
    // write(0x3B, 0x66);
    // write(0x3C, 0x01);
    // write(0x3D, 0x4F);
    // write(0x3F, 0xBB);


    write(0x00, 1); // move to page 1
    write(0x06, 0b00000001); // disable analog mute control

    write(0, 0); // move to page 0
    write(2, 0b00010000); // enter standby mode 
    write(0, 1); // move to page 1
    write(8, 1); // turn on fast ramp-up
    write(9, 0); // vcom powered on
    write(8, 0); // disable fast ramp-up
    write(1, 1); // VCOM mode

    write(0, 0); // move to page 0
    write(2, 0); // get out of standby mode
}

void PCM5142::dump() {
    write(0, 0);
    std::cout << "91: "<< std::bitset<8>(read(91)) << std::endl;
    std::cout << "94: "<< std::bitset<8>(read(94)) << std::endl;
    std::cout << "95: "<< std::bitset<8>(read(95)) << std::endl;
    std::cout << "108: "<< std::bitset<8>(read(108)) << std::endl;
    std::cout << "109: "<< std::bitset<8>(read(109)) << std::endl;
    std::cout << "118: "<< std::bitset<8>(read(118)) << std::endl;
}




