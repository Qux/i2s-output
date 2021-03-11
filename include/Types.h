#pragma once

#include <array>
#include <Util/Static_FIFO.h>

namespace Types {
    typedef std::array<int, Config::ADC::DMA::Buffer_Length> audiobuf_t;
    typedef Static_FIFO<audiobuf_t, Config::ADC::DMA::Buffer_Count> fifobuffer_t;
}

