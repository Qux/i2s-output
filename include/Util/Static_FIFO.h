#pragma once

#ifndef STATIC_FIFO_H

#include <cstddef>
#include <array>

template<typename T, std::size_t N>
class Static_FIFO {
    
public:
    Static_FIFO() {
        in = 0;
        out = 0;
        total = 0;
    }
    void push(const T& elm) {
        buf[in] = T(elm);
        in++;
        total++;

        if(in == N) {
           in = 0; 
        }
    }

    T& pop()  {
        if(out == N) {
            out = 0;
        }
        total--;
        return &buf[out++];
    }

    std::size_t available() const {
        return N - total - 1;
    }

    std::size_t count() const {
        return total;
    }



private:
    std::array<T, N> buf;
    std::size_t in, out, total;

};

#endif // !STATIC_FIFO_H

