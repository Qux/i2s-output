#pragma once

#include "Config.hpp"
#include "Types.hpp"

#include "App/ListeningData.hpp"

namespace DeepListening {
    void setup();
    void dsp(const StereoSample& in, StereoSample& out, const ListeningData& data);
    void control(const ListeningData& data);

    constexpr inline std::size_t mstosamps(float ms) {
        return Config::Sampling_Rate * 0.001 * ms;
    };

    constexpr inline float sampstoms(std::size_t samps) {
        return samps * Config::Sampling_Rate_Reciprocal * 1000.0;
    };   

    constexpr inline float mtof(const int pitch, const float base = 440.0) {  // might want to use the pre-defined look up table 
       constexpr float twelve_reciprocal = 1.0 / 12.0;
        return base * pow(2, (pitch - 69) * twelve_reciprocal);
    }
}