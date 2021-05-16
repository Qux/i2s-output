#pragma once

#include <array>

#include "Config.hpp"
#include "Util/Static_FIFO.h"

namespace Types {
    using audiobuf_t =  std::array<int, Config::DMA::Buffer_Length>;
    using fifobuffer_t = Static_FIFO<audiobuf_t, Config::DMA::Buffer_Count>;    
}

enum class Time_Unit {
    Micros = 0,
    Millis, 
    Seconds,
};

enum class RAM_Type {
    PSRAM,
    SRAM,
    // Try_SRAM
};