#pragma once

#include "Line.h"

namespace DSP {
    class ADSR {
        public:
            ADSR();
            ADSR(const float _a, const float _d, const float _s, const float _r);
            float operator()() const;
            operator float() const;


            template<typename T>
            T operator+ (const T&) const;

            template<typename T>
            T operator- (const T&) const;

            template<typename T> 
            T operator* (const T&) const;
            
            template<typename T>
            T operator/ (const T&) const;

            void set(const float _a, const float _d, const float _s, const float _r);
            void setAttack(const float _a);
            void setDecay(const float _d);
            void setSustain(const float _s);
            void setRelease(const float _r);
            void noteOn();
            void noteOff();
            void next();
            float get() const;
            void disable(const float static_val);
            bool isWorking();

        private:
            enum Status {
                Attack,
                Decay,
                Sustain,
                Release,
                Waiting,
            };

            Status status;

            float attack, decay, sustain, release;
            float val;
            Line line;
    };
}

inline DSP::ADSR::ADSR(){}

inline DSP::ADSR::ADSR(const float _a, const float _d, const float _s, const float _r) : attack(_a), decay(_d), sustain(_s), release(_r) {
    val = 0.0;
}

inline float DSP::ADSR::operator()() const {
    return this->val;
}

inline DSP::ADSR::operator float() const {
    return val;
}

template<typename T>
inline T DSP::ADSR::operator+(const T& rhs) const {
    return this->val + rhs;
}

template<typename T>
inline T DSP::ADSR::operator-(const T& rhs) const {
    return this->val - rhs;
}

template<typename T>
inline T DSP::ADSR::operator*(const T& rhs) const {
    return this->val * rhs;
}

template<typename T>
inline T DSP::ADSR::operator/(const T& rhs) const {
    return this->val / rhs;
}

inline void DSP::ADSR::set(const float _a, const float _d, const float _s, const float _r) {
    attack = _a;
    decay = _d;
    sustain = _s;
    release = _r;
}

inline void DSP::ADSR::setAttack(const float _a) {
    attack = _a;
}

inline void DSP::ADSR::setDecay(const float _d) {
    decay = _d;
}

inline void DSP::ADSR::setSustain(const float _s) {
    sustain = _s;
}

inline void DSP::ADSR::setRelease(const float _r) {
    release = _r;
}

inline void DSP::ADSR::noteOn() {
    status = Attack;
    line(1.0, attack);
}

inline void DSP::ADSR::noteOff() {
    status = Release;
    line(sustain, 0.0, release);
}

inline void DSP::ADSR::disable(const float static_val = 1.0) {
    status = Waiting;
    val = static_val;
}

inline void DSP::ADSR::next() {    
    switch (status)    {
    case Attack:
        line.next();
        this->val = line();
        if(1.0 <= this->val) {
            this->status = Decay;
            line(sustain, decay);
        }
        break;
    case Decay:
        line.next();
        this->val = line();
        if(this->val <= sustain) {
            this->status = Sustain;
        }
        break;
    case Sustain:
        this->val = sustain;        
        break;
    case Release:
        line.next();
        this->val = line();
        if(this->val <= 0.0) {
            this->status = Waiting;
        }
        break;
    case Waiting:
        break;
    default:
        break;
    }
}

inline float DSP::ADSR::get() const {
    return val;
}

inline bool DSP::ADSR::isWorking() {
    if(this->status == Waiting) {
        return false;
    } else {
        return true;
    }
}