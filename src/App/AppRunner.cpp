#include "AppRunner.hpp"

#include "I2S/AudioThread.hpp"

using namespace Types;
using namespace Hardware::Audio;

AppRunner::AppRunner(ListeningApp* _app) : app{_app} {
    
}


void AppRunner::beginThread() {
    I2S::beginThread(*app);
}

void AppRunner::initADC() {
    if(Config::ADC::Use_I2C_Device) {
        adc.setup();
        // adc.setClockMode(Hardware::Audio::Clock_Mode::Master);
        adc.setGain();
        adc.setMixer(Microphone_Type::Builtin_Microphones);
    }
}

void AppRunner::initDAC() {
    if(Config::DAC::Use_I2C_Device) {
        dac.setup();
        dac.enable_PLL(false);
        dac.setSampleRate(Config::Sampling_Rate);
        // dac.setClockMode(Clock_Mode::Master);
        dac.setWordLength(Word_Length::WL_32bit);
        dac.use16xInterpolation(true);
        dac.halfOutputRms(true);
        dac.wakeup();
    }
}

void AppRunner::init() {
    app->setup();
    
    switch (Config::Stream) {    
        case Stream_State::InOut:
            initADC();
            initDAC();
            vTaskDelay(Config::Hardware::Boot_Time);
            // initReader();            
            // initWriter();
            I2S::setupReader();
            I2S::setupWriter();
            beginThread();
            break;

        case Stream_State::Input_Only:
            initADC();
            vTaskDelay(Config::Hardware::Boot_Time);
            // initReader();
            I2S::setupReader();
            I2S::setupWriter();
            vTaskDelay(Config::Hardware::Boot_Time);
            beginThread();
            break;

        case Stream_State::Output_Only:
            initDAC();
            vTaskDelay(Config::Hardware::Boot_Time);
            // initWriter();
            I2S::setupWriter();
            vTaskDelay(Config::Hardware::Boot_Time);
            beginThread();
            break;

        case Stream_State::No_Audio: 
            beginThread();
            break;
    }
}

void AppRunner::run() {
    app->controlLoop();
    // TaskHandle_t taskHandle;
    // xTaskCreatePinnedToCore(app->controlLoop, "Control Loop", 4096, nullptr, 0, &taskHandle, PRO_CPU_NUM);
}



