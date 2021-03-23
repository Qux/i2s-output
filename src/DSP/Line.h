#pragma once

#include "Config.h"

namespace DSP {
    class Line {
        public:
            float operator()() const;  // get 
            void operator()(const float, const float); // set and start
            void operator()(const float, const float, const float);
            void bang(const float, const float);    // same 
            void bang(const float, const float, const float);
            float get() const ;
            void next(); // proceed one sample
            

        private:
            float dest, step_per_sample, val;
            bool isRunning;
            inline void stop() {
                this->val = this->dest;
                this->isRunning = false;
            };

            enum Direction {
                Inc = 0,
                Dec = 1,
            };
            Direction direction;
    };
};

inline float DSP::Line::operator()() const {
    return this->val;
}

inline void DSP::Line::operator()(float _dest, float duration_ms) {
    this->bang(_dest, duration_ms);
}

inline void DSP::Line::operator()(const float _inital, const float _dest, const float duration_ms) {
    this->val = _inital;
    this->bang(_dest, duration_ms);
}

inline void DSP::Line::bang(const float _dest, const float duration_ms) {
    this->dest = _dest;

    const float diff = this->dest - this->val;
    this->step_per_sample = diff / duration_ms * Config::Samples_Per_Msec_Reciprocal;

    if(diff < 0) {
        this->direction = Dec;
    } else {
        this->direction = Inc;
    }

    this->isRunning = true;
}

inline void DSP::Line::bang(const float _inital, const float _dest, const float duration_ms) {
    this->val = _inital;
    this->bang(_dest, duration_ms);
}

inline float DSP::Line::get() const {
    return this->val; 
}

inline void DSP::Line::next() {
    if(this->isRunning) {
        this->val += this->step_per_sample;
        if(direction == Inc && this->dest <= this->val ) {
            this->stop();
        } else if (direction == Dec && this->val <= this->dest) {
            this->stop();
        }
    }
}




