
#include "PCM5242.hpp"

using namespace Hardware::Audio;
using namespace Hardware::DAC;

PCM5242::PCM5242(const bool _ADR2, const bool _ADR1) : I2CDevice(PCM5242::getAddress(_ADR2, _ADR1)) {
    
}

void PCM5242::setup() {
    I2CDevice::setup();    
}


int PCM5242::setClockMode(Audio::Clock_Mode clock_mode) {
    
    switch (clock_mode)    {
    case Clock_Mode::Master:        
        write(0, 0); // move to page 0
        write(2, 0b00010000); // enter standby mode
        write(1, 0b00010001);   //enter reset mode
        write(12, 0b00000000); // BCK and LRCK divider disable        
        
        write(37, 0b00100010); // disable auto clock 
        write(37, 0b00000000); // enable auto clock 

        write(0, 253); // move to page 253
        write(63, 0x11);    // enable flex clock
        write(64, 0xFF);
        
        write(0, 0);        
        write(9, 0b00001001); // LRCK and BCK output

        write(12, 0x0C); // from datasheet
        write(14, 0x30);
        write(28, 0x1F);
                
        // write(14, 0x30); // set SCK as DAC clock
        // write(30, 0x30);
        // write(28, 31);
        // write(32, 64); // BCK clock
        // write(33, 0b00011111); // LRCK clock
        
        write(65, 0b00000100); //disable auto mute

        write(12, 0b00000011); // BCK and LRCK divider functional

        write(1, 0b00000000);  //exit reset mode?
        write(2, 0b00000000); // exit standby mode

        return 0;
        break;
    
    default:
        return -1;
    }
}

int PCM5242::setDataFormat(const Data_Format format) const {
    constexpr uint8_t Data_Format_Registor = 40;
    constexpr uint8_t AFMT_1 = 5;
    constexpr uint8_t AFMT_0 = 4;
    moveToPage(0);

    int bits = read(Data_Format_Registor);

    switch (format)    {
    case Data_Format::I2S:
        bits |= bit_unflag(bits, AFMT_1);
        bits |= bit_unflag(bits, AFMT_0);
        break;
    case Data_Format::TDM_DSP:
        bits |= bit_unflag(bits, AFMT_1);
        bits |= bit_flag(bits, AFMT_0);
        break;
    case Data_Format::RTJ:
        bits |= bit_flag(bits, AFMT_1);
        bits |= bit_unflag(bits, AFMT_0);
        break;
    case Data_Format::LTJ:
        bits |= bit_flag(bits, AFMT_1);
        bits |= bit_flag(bits, AFMT_0);
        break;
    default:
        break;
    }

    return write(Data_Format_Registor, bits);  // use i2s, 32bit
}

int PCM5242::setWordLength(const Word_Length wl) const {
    constexpr uint8_t Word_Length_Registor = 40;
    constexpr uint8_t ALEN_1 = 1;
    constexpr uint8_t ALEN_0 = 0;

    int bits = read(Word_Length_Registor);
    bits = bit_unflag(bits, ALEN_1);
    bits = bit_unflag(bits, ALEN_0);

    switch(wl) {
        case Word_Length::WL_16bit:        
            break;
        case Word_Length::WL_20bit:
            bits |= 1;
            break;
        
        case Word_Length::WL_24bit:
            bits |= 0b10;
            break;

        case Word_Length::WL_32bit:
            bits |= 0b11;
            break;

        default:
            break;
    }

    return write(Word_Length_Registor, bits);
}

int PCM5242::use16xInterpolation(const bool state) const {
    constexpr auto Interpolation_Mode_Registor = 34;
    constexpr auto Interpolation_Bit = 4;
    moveToPage(0);
    auto bits = read(Interpolation_Mode_Registor);
    if(state) {
        bits = bit_flag(bits, Interpolation_Bit);
    } else {
        bits = bit_unflag(bits, Interpolation_Bit);
    }

    return write(Interpolation_Mode_Registor, bits);
}

int PCM5242::setSampleRate(const Sampling_Rate sr) const {
    constexpr auto FS_Speed_Mode_Register = 34;
    constexpr uint8_t FSSP1 = 1;
    constexpr uint8_t FSSP0 = 0;

    moveToPage(0);
    auto bits = read(FS_Speed_Mode_Register);
    switch (sr)    {
        case Sampling_Rate::SR_44k:
        case Sampling_Rate::SR_48k:
            bit_unflag(bits, FSSP1);
            bit_unflag(bits, FSSP0);
            break;
        case Sampling_Rate::SR_88k: 
        case Sampling_Rate::SR_96k:
            bit_unflag(bits, FSSP1);
            bit_flag(bits, FSSP0);
            break;
        case Sampling_Rate::SR_176k: 
        case Sampling_Rate::SR_192k:
            bit_flag(bits, FSSP1);
            bit_unflag(bits, FSSP0);
            break;
        case Sampling_Rate::SR_384k:
            bit_flag(bits, FSSP1);
            bit_flag(bits, FSSP0);
            break;
        default:            
            break;
    }
    return write(FS_Speed_Mode_Register, bits); // fs Quad speed
}

int PCM5242::setSampleRate(const std::size_t sr) const {
    return this->setSampleRate(static_cast<Sampling_Rate>(sr));
}

void PCM5242::enable_PLL(const bool state) {
    write(0, 0);
    // write(0x04, state);
    const uint8_t PLL_Register = 0x04;    
    auto bits = read(PLL_Register);

    if(state == true) {
        bit_flag(bits, 0);
    } else {
        bit_unflag(bits, 0);
    }
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

int PCM5242::moveToPage(const std::size_t page) const {
    return write(0, page);
}

