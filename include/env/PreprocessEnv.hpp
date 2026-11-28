#pragma once

#include <opencv2/core.hpp>

#include "env/EnvWrapper.hpp"

namespace doomrl {
    class PreprocessEnv : public EnvWrapper {
    private:
        cv::Size size;

        cv::Mat process(const cv::Mat& frame) const;

    public:
        PreprocessEnv(Environment& inner, int obsSize);

        StepResult step(int action) override;

        cv::Mat reset() override;
    };
}  // namespace doomrl
