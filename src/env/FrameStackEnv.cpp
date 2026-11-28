#include "env/FrameStackEnv.hpp"

#include <vector>

namespace doomrl {
    FrameStackEnv::FrameStackEnv(Environment& inner, int count)
        : EnvWrapper{inner}, count{count} {}

    cv::Mat FrameStackEnv::stacked() const {
        cv::Mat result;
        cv::merge(std::vector<cv::Mat>(frames.begin(), frames.end()), result);
        return result;
    }

    cv::Mat FrameStackEnv::reset() {
        const cv::Mat first = inner.reset();
        frames.assign(count, first);
        return stacked();
    }

    StepResult FrameStackEnv::step(int action) {
        StepResult result = inner.step(action);
        frames.pop_front();
        frames.push_back(result.observation);
        result.observation = stacked();
        return result;
    }
}  // namespace doomrl
