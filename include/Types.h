#pragma once

#include <array>


#include <Config.h>

#include "Util/Static_FIFO.h"

#include "Base/StereoSample.hpp"
#include "DSP/DelayLine.h"

namespace Types {
    using audiobuf_t =  std::array<int, Config::DMA::Buffer_Length> ;
    using fifobuffer_t = Static_FIFO<audiobuf_t, Config::DMA::Buffer_Count>;    
}

enum Time_Unit {
    Micros = 0,
    Millis, 
    Seconds,
};
