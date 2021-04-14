#pragma once

#include "../Audio.h"
#include "../I2CDevice.h"

namespace Hardware  {
    namespace ADC {
        class PCM1863 : public I2CDevice {     
            
            public:       
                PCM1863(const bool AD1 = false);   
                void setup() override;
                void setup(const bool AD1);
                void setGain(const float _gain = 0.0);
                void setSamplingRate(Hardware::Audio::Sampling_Rate sr);
                void setClockMode(Hardware::Audio::Clock_Mode);
                void setClockRatio(Hardware::Audio::SCK_Ratio);
                void setMixer(const Hardware::Audio::Microphone_Type mode = Hardware::Audio::Microphone_Type::Builtin_Microphones) const;        

                static constexpr int getAddress(const bool AD1) {
                    return 0b1001010 + AD1;
                };
            // private:
                // const int Device_Address;
                
        };
    }
}
