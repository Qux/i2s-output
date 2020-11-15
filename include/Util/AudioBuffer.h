#pragma once

#ifndef AUDIOBUFFER_H
#define AUDIOBUFFER_H

#include <array>
#include <cstddef>
#include <cstring>
#include <cmath>

#include "RangedCounter.h"

template<typename T, std::size_t N> class AudioBuffer   {
private:
    /* data */
    std::array<T, N> buf;
    std::size_t count;
    RangedCounter<N> head;
    RangedCounter<N> tail;
    static constexpr std::size_t type_size = sizeof(T);

public:
    AudioBuffer(/* args */) {
        count = head = tail = 0;
    };

    bool append(T* data, std::size_t size_byte) {
        
    }

    std::size_t has_space() const {
        return N - count; 
    }

    ~AudioBuffer();
};

#endif // !AUDIOBUFFER_H
