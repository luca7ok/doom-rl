#pragma once

#include <deque>
#include <opencv2/core.hpp>

#include "env/EnvWrapper.hpp"

namespace doomrl {
    class FrameStackEnv : public EnvWrapper {
    private:
        int count;
        std::deque<cv::Mat> frames;

        cv::Mat stacked() const;

    public:
        FrameStackEnv(Environment& inner, int count);

        StepResult step(int action) override;

        cv::Mat reset() override;
    };
}  // namespace doomrl
