
#include "PCM5242.hpp"

using namespace Hardware::Audio;
using namespace Hardware::DAC;

PCM5242::PCM5242(const bool _ADR2, const bool _ADR1) : I2CDevice(PCM5242::getAddress(_ADR2, _ADR1)) {
    
}

void PCM5242::setup() {
    I2CDevice::setup();    
}


void PCM5242::setMixer() {

}


int PCM5242::setClockMode(Audio::Clock_Mode clock_mode) {
    
    switch (clock_mode)    {
    case Clock_Mode::Master:        
        write(0, 0); // move to page 0
        write(2, 0b00010000); // enter standby mode
        write(1, 0b00010001);   //enter reset mode
        write(34, 00000000); // fs single speed
        write(40, 0b00000011);  // use i2s, 32bit
        
        write(37, 0b00100010); // disable auto clock 
        write(37, 0b00000000); // enable auto clock 

        // write(0, 253); // move to page 253
        // write(63, 0x11);    // enable flex clock
        // write(64, 0xFF);
        
        write(0, 0);        
        // write(9, 0b00001001); // LRCK and BCK output

        // write(12, 0x0C); // from datasheet
        // write(14, 0x30);
        // write(28, 0x1F);
            

        
        write(14, 0x30); // set SCK as DAC clock
        // write(30, 0x30);
        // write(28, 31);
        // write(32, 64); // BCK clock
        // write(33, 0b00011111); // LRCK clock
        
        // write(65, 0b00000100); //disable auto mute

        write(12, 0b00000011); // BCK and LRCK divider functional

        write(1, 0b00000000);  //exit reset mode?
        write(2, 0b00000000); // exit standby mode


        // return endTransmission();
        return 0;
        break;
    
    default:
        return 255;
    }
}


void PCM5242::enable_PLL(const bool state) {
    write(0, 0);
    write(0x04, state);
}

void PCM5242::halfOutputRms(bool half_db) {
    // beginTransmission();
    write(0, 1);
    if(half_db) {        
        write(2, 0b00010001);
    } else {
        write(2, 0b00000000);
    }
    // endTransmission();
};

void PCM5242::wakeup() {
    // beginTransmission();
    // write(0x00, 01);
    // write(0x02, 11);

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

    // write(0, 0);
    // write(2, 0b00010001);

}

void PCM5242::dump() {
    write(0, 0);
    std::cout << "91: "<< std::bitset<8>(read(91)) << std::endl;
    std::cout << "93: "<< std::bitset<8>(read(93)) << std::endl;
    std::cout << "94: "<< std::bitset<8>(read(94)) << std::endl;
    std::cout << "95: "<< std::bitset<8>(read(95)) << std::endl;
    std::cout << "108: "<< std::bitset<8>(read(108)) << std::endl;
    std::cout << "109: "<< std::bitset<8>(read(109)) << std::endl;
    std::cout << "114: "<< std::bitset<8>(read(114)) << std::endl;
    std::cout << "118: "<< std::bitset<8>(read(118)) << std::endl;

    std::cout << "Page 1" << std::endl;
    write(0, 1);
    std::cout << "1: "<< std::bitset<8>(read(1)) << std::endl;
    // std::cout << "120: "<< std::bitset<8>(read(120)) << std::endl;
}


