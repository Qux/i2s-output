#pragma once

#include "Types.h"

struct History {
    StereoDelay input = StereoDelay(Config::History::Size);
    StereoDelay output = StereoDelay(Config::History::Size);
};

using FFT = Types::audiobuf_t;

struct ListeningData {
    History history;
    FFT fft;
};
