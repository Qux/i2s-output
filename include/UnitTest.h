
#pragma once

#ifndef UNITTEST_H
#define UNITTEST_H

#include <stdio.h>
#include <cstddef>

namespace UnitTest  {
    std::size_t index(bool reset = false) {
        static std::size_t counter = 0;

        if(reset) counter = 0;

        return counter++;
    }

    template<typename Ta, typename Tb> bool assert_equal(Ta obj1, Tb obj2, bool reset_index = false) {        
        if(obj1 == obj2) {
            printf("Test %d succeeded.\n", index(reset_index));
            return true;
        } else {
            printf("[FAILED] Test %d not succeeded. -- [FAILED]\n", index(reset_index));
            return false;
        }        
    }
}


#endif // !UNITTEST_H

