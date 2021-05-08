#pragma once

#ifndef FILTER_H
#define FILTER_H
#endif

#define MAX_WINDOW_SIZE 40
#include "DelayLine.h"

class Filter {
public:
    enum Filtertype {
        LowPass, HighPass, AllPass,
    };

    Filter(Filtertype _filtertype = LowPass);
    
    void init();
    void setFiltertype(const Filtertype _filtertype);
    void setWindowsize(const int _windowsize);
    void setIndex(const std::size_t _index);
    float getNext(float in);
    
private:
    MonoDelay dline = MonoDelay(MAX_WINDOW_SIZE); // TODO 本当はwindowsizeの取りうる最大値にしたい。
    float windowsize;
    float windowsize_reciprocal;
    Filtertype filtertype;
    std::size_t index;
    std::size_t initial_index;
    float sum;
    float outL;
    float outR;
};
