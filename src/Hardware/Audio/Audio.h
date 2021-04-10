#pragma once

namespace Hardware  {
    namespace Audio {
        enum struct Clock_Mode {
            Master,
            BCK_Input_Slave_PLL,
            Non_Audio_MCK_PLL,
            Manual_PLL,
        };

        enum struct SCK_Ratio {                   
            x128 = 128,
            x256 = 256,
            x384 = 384,
            x512 = 512,
            x768 = 768,
        };

        enum struct BCK_Ratio_To_Clock {
            x32 = 32,
            x48 = 48,
            x64 = 64,
            x256 = 256,
        };

        enum struct Sampling_Rate {
            SR_44k = 44100,
            SR_48k = 48000,
            SR_88k = 88200,
            SR_96k = 96000,
            SR_176k = 17600,
            SR_192k = 192000,
            SR_384k = 384000,
        };

        enum struct Microphone_Type {
            Builtin_Microphones,
            External_Balanced_Microphones,
            Single_LR_Stereo,
        };

        enum struct Data_Format {
            I2S = 0,
            TDM_DSP = 1,
            RTJ = 2,
            LTJ = 3,
        };

        enum struct Word_Length {
            WL_16bit = 16,
            WL_20bit = 20,
            WL_24bit = 24,
            WL_32bit = 32,
        };

    }    
}
