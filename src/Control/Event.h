#pragma once

#include <Arduino.h>

#include <functional>

#include "Types.h"

struct Event {
    esp_timer_cb_t func;
    Time_Unit resolution;
    std::size_t interval;
    std::size_t previousTime;
};