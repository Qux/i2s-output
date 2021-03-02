#pragma once

#ifndef DELAYLINE_H
#define DELAYLINE_H

#include <Arduino.h>

#include <cstddef>
#include <array>

template<typename T, std::size_t N>class DelayLine    {
private:
    // std::array<T, N> buffer; 
    T* buffer;
    std::size_t z0_index = N - 1;

    std::size_t get_index_on_buffer(std::size_t z_index) const  {
        if(z0_index < z_index) {
            return N - (z_index - z0_index);
        } else {
            return z0_index - z_index;
        }
    }

public:
    DelayLine(/* args */){
        // for(auto& elm : buffer) {
        //     elm = T();
        // }
        psramInit();
        buffer = static_cast<T*>(ps_calloc(N, sizeof(T)));
    };

    // DelayLine(const T& _default) {
    //     for(auto& elm : buffer) {
    //         elm = T(_default);
    //     }
    // }

    ~DelayLine(){};

    void add(const T value)  {
        z0_index++;
        if(N <= z0_index) {
            z0_index = 0;
        }

        this->buffer[z0_index] = value;                
    }
    
    const T& get(std::size_t z_index) const {
        // return buffer.at(get_index_on_buffer(z_index));
        return buffer[get_index_on_buffer(z_index)];
    }

    // Just get
    const T& operator[](std::size_t z_index) const {                
        return this->get(z_index);
    }        

    constexpr std::size_t size() const {
        return N;
    }
};


#endif // !DELAYLINE_H
