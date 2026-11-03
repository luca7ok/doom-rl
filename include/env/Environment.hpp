#pragma once

#include <opencv2/core.hpp>

namespace doomrl {
    struct StepResult {
        float reward;
        bool done;
        cv::Mat observation;
    };

    class Environment {
    public:
        virtual int numActions() const = 0;

        virtual StepResult step(int action) = 0;

        virtual cv::Mat reset() = 0;

        virtual ~Environment() = default;
    };
}  // namespace doomrl