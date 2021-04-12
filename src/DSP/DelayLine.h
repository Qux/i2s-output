#pragma once

#ifndef DELAYLINE_H
#define DELAYLINE_H

#include <Arduino.h>

#include <cstddef>
#include <array>

#include "esp_himem.h"

#include "Base/StereoSample.hpp"

template<typename T = float>class DelayLine    {
private:
    // std::array<T, N> buffer; 
    T* buffer;
    const std::size_t Buffer_Size;
    std::size_t z0_index;    

    std::size_t get_index_on_buffer(std::size_t z_index) const {
        if(z0_index < z_index) {
            return Buffer_Size - (z_index - z0_index);
        } else {
            return z0_index - z_index;
        }
    }

public:
    enum class RAM_Type {
        PSRAM,
        SRAM
    };

    DelayLine(std::size_t max_samples, const RAM_Type ram_type = RAM_Type::PSRAM) : Buffer_Size{max_samples} {
        switch (ram_type)   {
            case RAM_Type::PSRAM:
                psramInit();
                buffer = static_cast<T*>(ps_calloc(Buffer_Size, sizeof(T)));
                break;
            case RAM_Type::SRAM:
                buffer = static_cast<T*>(calloc(Buffer_Size, sizeof(T)));
                break;
            default:
                break;
        }

        z0_index = Buffer_Size - 1;
    };

    ~DelayLine(){
        free(buffer);
    };

    inline void add(const T value)  {
        z0_index++;
        if(Buffer_Size <= z0_index) {
            z0_index = 0;
        }

        this->buffer[z0_index] = value;                
    }
    
    inline const T& get(std::size_t z_index) const {
        // return buffer.at(get_index_on_buffer(z_index));
        return buffer[get_index_on_buffer(z_index)];
    }

    // Just get
    inline const T& operator[](std::size_t z_index) const {                
        return this->get(z_index);
    }        

    inline constexpr std::size_t size() const {
        return Buffer_Size;
    }
};

using StereoDelay = DelayLine<StereoSample>;
using MonoDelay = DelayLine<float>;

#endif // !DELAYLINE_H
