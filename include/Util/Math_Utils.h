#pragma once

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cstddef>

#include "Config.h"

namespace Math {
    inline std::size_t ms2samps(float time_in_ms) {
        return static_cast<std::size_t>(time_in_ms * 1000.0 * Config::Sampling_Rate_Reciprocal);
    }

    template <typename T>
    constexpr T ipow(T num, std::size_t pow)   {
        return (pow >= sizeof(unsigned int)*8) ? 0 :
            pow == 0 ? 1 : num * ipow(num, pow-1);
    }
}



#endif // !UTILS_H