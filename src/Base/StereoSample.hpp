#ifndef STEREOSAMPLE_HPP_
#define STEREOSAMPLE_HPP_

#pragma once

/* Declaration */
class StereoSample {
public:        
    StereoSample(const float);
    StereoSample(const StereoSample&);
    StereoSample(const float, const float);

    bool operator== (const StereoSample&) const;
    bool operator== (const float) const;

    StereoSample& operator=(const StereoSample&);
    StereoSample& operator=(const float);

    /* add */
    StereoSample operator+(const StereoSample&) const;
    StereoSample operator+(const float) const;

    StereoSample& operator+=(const StereoSample&);
    StereoSample& operator+=(const float);
    
    /* sub */ 
    StereoSample operator-(const StereoSample&) const;
    StereoSample operator-(const float) const;

    StereoSample& operator-=(const StereoSample&);
    StereoSample& operator-=(const float);

    /* mul */
    StereoSample operator*(const StereoSample&) const;
    StereoSample operator*(const float) const;
    
    StereoSample& operator*=(const StereoSample&);
    StereoSample& operator*=(const float);

    /* div */
    StereoSample operator/(const StereoSample&) const;
    StereoSample operator/(const float) const;
    
    StereoSample& operator/=(const StereoSample&);
    StereoSample& operator/=(const float);
    
    float L, R;
    
};


/* Implementation  */
inline StereoSample::StereoSample(const float val = 0.0) : L(val), R(val) {};
inline StereoSample::StereoSample(const StereoSample& obj) : L(obj.L), R(obj.R) {};
inline StereoSample::StereoSample(const float _l, const float _r) : L(_l), R(_r) {};

/* equal */ 
inline bool StereoSample::operator==(const StereoSample& val) const {
    if(val.L == this->L && val.R == this->R) {
        return true;
    } else {
        return false;
    }
}

inline bool StereoSample::operator==(const float val) const {
    if(val == this->L && val == this->R) {
        return true;
    } else {
        return false;
    }
}

/* assign */
inline StereoSample& StereoSample::operator=(const StereoSample& val) {
    this->L = val.L;
    this->R = val.R;
    return *this;
}

inline StereoSample& StereoSample::operator=(const float val) {
    this->L = this->R = val;
    return *this;
}

/* add */
inline StereoSample StereoSample::operator+(const StereoSample& val) const {
    return StereoSample(this->L + val.L, this->R + val.R);
};

inline StereoSample StereoSample::operator+(const float val) const {
    return StereoSample(this->L + val, this->R + val);
};
inline StereoSample& StereoSample::operator+=(const StereoSample& val) {
    this->L += val.L;
    this->R += val.R;
    return *this;
};
inline StereoSample& StereoSample::operator+=(const float val) {
    this->L += val;
    this->R += val;
    return *this;
};

/* sub */ 
inline StereoSample StereoSample::operator-(const StereoSample& val) const {
    return StereoSample(this->L - val.L, this->R - val.R);
};
inline StereoSample StereoSample::operator-(const float val) const {
    return StereoSample(this->L - val, this->R - val);
};

inline StereoSample& StereoSample::operator-=(const StereoSample& val) {
    this->L -= val.L;
    this->R -= val.R;
    return *this;
};
inline StereoSample& StereoSample::operator-=(const float val) {
    this->L -= val;
    this->R -= val;
    return *this;
};

/* mul */
inline StereoSample StereoSample::operator*(const StereoSample& val) const {
    return StereoSample(this->L * val.L, this->R * val.R);
};
inline StereoSample StereoSample::operator*(const float val) const {
    return StereoSample(this->L * val, this->R * val);
};

inline StereoSample& StereoSample::operator*=(const StereoSample& val) {
    this->L *= val.L;
    this->R *= val.R;
    return *this;
};
inline StereoSample& StereoSample::operator*=(const float val) {
    this->L *= val;
    this->R *= val;
    return *this;
};

/* div */
inline StereoSample StereoSample::operator/(const StereoSample& val) const {
    return StereoSample(this->L / val.L, this->R / val.R);
};
inline StereoSample StereoSample::operator/(const float val) const {
    return StereoSample(this->L / val, this->R / val);
};

inline StereoSample& StereoSample::operator/=(const StereoSample& val) {
    this->L /= val.L;
    this->R /= val.R;
    return *this;
};
inline StereoSample& StereoSample::operator/=(const float val) {
    this->L /= val;
    this->R /= val;
    return *this;
};

#endif