#pragma once

#include <iostream>

#include "test/UnitTest.h"

#include "Base/StereoSample.hpp"

namespace StereoSampleTest  {
    void run() {
        std::cout << "Stereo Sample test begining..." << std::endl;

        using namespace UnitTest;
        StereoSample sample = StereoSample(0.0);
        assert_equal(sample.L, 0.0, true);
        assert_equal(sample.R, 0.0);
        
        sample = 1.0;
        assert_equal(sample.L, 1.0);
        assert_equal(sample.R, 1.0);

        sample += 1.0;
        assert_equal(sample.L, 2.0);
        assert_equal(sample.R, 2.0);

        assert_equal(sample.L == 2.0, true);
        assert_equal(sample.R == 2.0, true);
        assert_equal(sample == 2.0, true);


        sample = sample * 3.0;
        assert_equal(sample.L, static_cast<float>(6.0));
        assert_equal(sample.R, static_cast<float>(6.0));

        sample /= StereoSample(2.0);
        assert_equal(sample.L, static_cast<float>(6.0 / 2.0));
        assert_equal(sample.R, static_cast<float>(6.0 / 2.0));

        StereoSample sample2 = StereoSample(1.0);
        assert_equal(sample2.L, 1.0);
        assert_equal(sample2.R, 1.0);

        StereoSample sample3 = StereoSample(sample + sample2);
        assert_equal(sample3.L, static_cast<float>(6.0 / 2.0 + 1.0));
        assert_equal(sample3.R, static_cast<float>(6.0 / 2.0 + 1.0));

        std::cout << "Stereo Sample test finished." << std::endl;

    };
}

