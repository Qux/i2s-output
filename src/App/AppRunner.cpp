#include "AppRunner.hpp"

using namespace Types;
using namespace Hardware::Audio;

AppRunner::AppRunner(ListeningApp* _app) : app{_app} {
    this->buf = new fifobuffer_t;
}

void AppRunner::initReader() {
    std::cout << "Reader begin" << std::endl;
    reader = new I2S::Reader(app);
    reader->setBufferPtr(buf);
    reader->begin();
}

void AppRunner::initWriter() {
    std::cout << "Writer begin" << std::endl;

    writer = new I2S::Writer(app);
    writer->setAudioBufferPtr(buf);
    writer->begin();
}

void AppRunner::initADC() {
    if(Config::ADC::Use_I2C_Device) {
        adc.setup();
        adc.setClockMode(Hardware::Audio::Clock_Mode::Master);
        adc.setGain();
        adc.setMixer();
    }
}

void AppRunner::initDAC() {
    if(Config::DAC::Use_I2C_Device) {
        dac.setup();
        dac.enable_PLL(false);
        dac.setSampleRate(Config::Sampling_Rate);
        dac.setClockMode(Clock_Mode::Master);
        dac.setWordLength(Word_Length::WL_32bit);
        dac.halfOutputRms(true);
        dac.wakeup();
    }
}

void AppRunner::init() {
    app->setup();
    switch (Config::Stream) {
        case InOut:
            initADC();
            initDAC();
            vTaskDelay(Config::Hardware::Boot_Time);
            initReader();            
            initWriter();
            break;

        case Input_Only:
            initADC();
            vTaskDelay(Config::Hardware::Boot_Time);
            initReader();
            break;

        case Output_Only:
            initDAC();
            vTaskDelay(Config::Hardware::Boot_Time);
            initWriter();
            break;

        case No_Audio: 
            break;
    }
}

void AppRunner::run() {
    app->controlLoop();
}



