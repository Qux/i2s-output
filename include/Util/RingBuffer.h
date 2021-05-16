#pragma once


template<typename T, int N>
class RingBuffer {
    public:
        T get(std::size_t index) const {
            int target_index = head - index;
            while(target_index < 0) {
                target_index += N;
            }
            return buf.at(target_index);
        };  
        
        void add(T val) {
            buf[head] = val;
            head++;
            if(head == N) {
                head = 0;
            }
        };

        void fill(T val) {
            buf.fill(val);
        }

    private:        
        int head;
        std::array<T, N> buf;

};

 