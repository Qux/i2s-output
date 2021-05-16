#include "AppRunner.hpp"

/* Depends */
#include "Arduino.h"
#include "WiFi.h"

#include "Config.hpp"
#include "Types.hpp"

/* Audio Thread */
#include "I2S/AudioThread.hpp"

#include "DeepListening.hpp"

using namespace Types;
using namespace Hardware::Audio;

AppRunner::AppRunner(ListeningApp* _app) : app{_app} {}

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

void AppRunner::beginThread() {
    I2S::beginThread(*app);
}

void AppRunner::beginWiFi() {
    WiFi.disconnect(true, true);  // disable wifi, erase ap info
    delay(100);
    WiFi.mode(WIFI_STA);    
    WiFi.begin(Config::WiFi::SSID.c_str(), Config::WiFi::Password.c_str());
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
    }
}

void AppRunner::init() {
    if(Config::WiFi::Use) {
        beginWiFi();
    }
    
    app->setup();
    
    switch (Config::Stream) {    
        case Stream_State::InOut:
            initADC();
            initDAC();
            vTaskDelay(Config::Hardware::Boot_Time_Tick);
            I2S::setupReader();
            I2S::setupWriter();
            beginThread();
            break;

        case Stream_State::Input_Only:
            initADC();
            vTaskDelay(Config::Hardware::Boot_Time_Tick);
            I2S::setupReader();
            I2S::setupWriter();
            vTaskDelay(Config::Hardware::Boot_Time_Tick);
            beginThread();
            break;

        case Stream_State::Output_Only:
            initDAC();
            vTaskDelay(Config::Hardware::Boot_Time_Tick);
            I2S::setupWriter();
            vTaskDelay(Config::Hardware::Boot_Time_Tick);
            beginThread();
            break;

        case Stream_State::No_Audio: 
            beginThread();
            break;
    }
}

void AppRunner::run() {
    app->controlLoop();
}



