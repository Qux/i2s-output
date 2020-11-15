#pragma once

#include <cstddef>

template<std::size_t Max> class RangedCounter {
    public:
        RangedCounter() {
            count = 0;
        };

        RangedCounter(const std::size_t value) {
            count = value;
        }


        template<typename T>
        bool operator==(const T& elm) const  {
            return count == static_cast<std::size_t>(elm);
        }

        template<typename T>
        std::size_t operator=(const T value) {
            count = static_cast<std::size_t>(value);
            return count;
        }

        template<typename T>
        std::size_t operator+(const T& elm) {
            std::size_t tmp(static_cast<std::size_t>(elm) + count);
            return tmp;
        }

        // template<typename T, std::size_t M>
        // std::size_t operator+(const RangedCounter<M>& lhs, const T& rhs) {
        //     std::size_t tmp(static_cast<std::size_t>(rhs) + lhs.get());
        //     return tmp;
        // }

        template<typename T>
        std::size_t operator-(const T& elm) {
            std::size_t tmp(static_cast<std::size_t>(elm) - count);
            return tmp;
        }

        // template<typename T, std::size_t M>
        // std::size_t operator-(const RangedCounter<M>& lhs, const T& rhs) {
        //     std::size_t tmp(static_cast<std::size_t>(rhs) - lhs.get());
        //     return tmp;
        // }

        template<typename T>
        inline operator T() const {
            return static_cast<T>(count);
        };

        // operator int&() {
        //     return static_cast<int>(count);
        // };


        // 前置インクリメントのオーバーロード
        std::size_t& operator++() {
            count++;
            if(count == Max) {
                count = 0;
            }
            return count;
        }
        // 後置インクリメントのオーバーロード
        std::size_t operator++(int) {  // 引数にintと書くと後置扱いになる
            RangedCounter<Max> tmp = RangedCounter(count);
            this->count++;
            return tmp;
        }

        // 前置デクリメントのオーバーロード
        std::size_t& operator--() {
            if(count == 0) {
                count = Max;
            } 
            count--;
            return count;
        }
        // 後置デクリメントのオーバーロード
        std::size_t operator--(int) {  // 引数にintと書くと後置扱いになる
            RangedCounter<Max> tmp = RangedCounter(count);
            this->count--;
            return tmp;
        }

        inline std::size_t get() const {
            return this->count;
        }

    private:    
        std::size_t count;

};