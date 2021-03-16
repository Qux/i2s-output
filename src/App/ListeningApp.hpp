#pragma once

#include <Arduino.h>
#include <cstddef>

#include "Config.h"

class ListeningApp {
    public:
        virtual void setup() = 0;
        virtual void dsp(const float, const float, float&, float&) = 0;
        virtual void control() = 0;

        static inline std::size_t mstosamps(float ms) {
            return Config::Sampling_Rate * 0.001 * ms;
        }

        static inline float sampstoms(std::size_t samps) {
            return samps * Config::Sampling_Rate_Reciprocal * 1000.0;
        }
};