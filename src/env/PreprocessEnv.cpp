#include "env/PreprocessEnv.hpp"

#include <opencv2/imgproc.hpp>

namespace doomrl {
    PreprocessEnv::PreprocessEnv(Environment& inner, int obsSize)
        : EnvWrapper{inner}, size{obsSize, obsSize} {}

    cv::Mat PreprocessEnv::process(const cv::Mat& frame) const {
        cv::Mat gray;
        cv::cvtColor(frame, gray, cv::COLOR_RGB2GRAY);

        cv::Mat small;
        cv::resize(gray, small, size, 0, 0, cv::INTER_AREA);
        return small;
    }

    cv::Mat PreprocessEnv::reset() {
        return process(inner.reset());
    }

    StepResult PreprocessEnv::step(int action) {
        StepResult result = inner.step(action);
        result.observation = process(result.observation);
        return result;
    }
}  // namespace doomrl
