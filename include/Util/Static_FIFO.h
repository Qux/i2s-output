#pragma once

#ifndef STATIC_FIFO_H

#include <cstddef>
#include <array>
#include <mutex>

template<typename T, std::size_t N>
class Static_FIFO {
    
public:
    Static_FIFO() {
        in = 0;
        out = 0;
        total = 0;
    }
    void push(const T& elm) {
        std::lock_guard<std::mutex> lock(mutex);
        buf[in] = T(elm);
        in++;
        total++;

        if(in == N) {
           in = 0; 
        }
    }

    T* push() {
        std::lock_guard<std::mutex> lock(mutex);
        T* dest = &buf[in];
        in++;
        total++;

        if(in == N) {
           in = 0; 
        }
        return dest;
    }

    T& pop()  {
        std::lock_guard<std::mutex> lock(mutex);
        if(out == N) {
            out = 0;
        }
        total--;
        return buf[out++];
    }

    // const T& pop() {
    //     std::lock_guard<std::mutex> lock(mutex);
    //     if(out == N) {
    //         out = 0;
    //     }
    //     total--;
    //     return buf[out++];
    // }

    std::size_t has_space() const {
        return N - total - 1;
    }

    std::size_t has_queue() const {
        return total;
    }


private:
    std::array<T, N> buf;
    std::size_t in, out, total;
    std::mutex mutex;

};

#endif // !STATIC_FIFO_H

