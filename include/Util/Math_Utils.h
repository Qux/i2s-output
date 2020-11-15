#pragma once

#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <cstddef>

#include "Config.h"

inline std::size_t ms2samps(float time_in_ms) {
    return static_cast<std::size_t>(time_in_ms * 1000.0 * Config::Sampling_Rate_Reciprocal);
}

#endif // !UTILS_H