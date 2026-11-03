#pragma once

#include <opencv2/core/mat.hpp>

namespace doomrl {
    class Agent {
    public:
        virtual int act(const cv::Mat& observation) = 0;

        virtual ~Agent() = default;
    };
}  // namespace doomrl