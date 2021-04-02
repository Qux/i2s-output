#pragma once

#include <array>

#include "Util/Static_FIFO.h"

#include "Base/StereoSample.hpp"
#include "DSP/DelayLine.h"

namespace Types {
    typedef std::array<int, Config::DMA::Buffer_Length> audiobuf_t;
    typedef Static_FIFO<audiobuf_t, Config::DMA::Buffer_Count> fifobuffer_t;    
}


using StereoDelay = DelayLine<StereoSample>;
using MonoDelay = DelayLine<float>;


enum Time_Unit {
    Micros = 0,
    Millis, 
    Seconds,
};

class History {
    public:
        StereoDelay input = StereoDelay(Config::History::Size);
        StereoDelay output = StereoDelay(Config::History::Size);
};