#pragma once

#ifndef DELAYLINE_H
#define DELAYLINE_H

#include <cstddef>
#include <array>

// #include "esp_himem.h"

#include "Types.hpp"
#include "Base/StereoSample.hpp"

template<typename T = float>class DelayLine    {
private:
    // std::array<T, N> buffer; 
    T* buffer;
    const std::size_t Buffer_Size;
    std::size_t head;    


public:
    DelayLine(const std::size_t max_samples, const RAM_Type ram_type = RAM_Type::PSRAM) : Buffer_Size{max_samples} {
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

        head = Buffer_Size - 1;
    };

    ~DelayLine(){
        free(buffer);
    };

    inline void add(const T value)  {
        head++;

        if(Buffer_Size <= head) {
            head = 0;
        }

        this->buffer[head] = value;                
    }
    
    inline const T& get(std::size_t z_index) const {
        int index = head - z_index;
        while(index < 0) {
            index += Buffer_Size;
        }

        return buffer[index];
    }

    // Just get
    inline const T& operator[](std::size_t z_index) const {                        
        return this->get(z_index);
    }        

    inline constexpr std::size_t size() const {
        return Buffer_Size;
    }

    void fill(const T& val) {
        for(std::size_t i = 0; i < Buffer_Size; i++) {
            buffer[i] = val;
        }
    }
};

using StereoDelay = DelayLine<StereoSample>;
using MonoDelay = DelayLine<float>;

#endif // !DELAYLINE_H
