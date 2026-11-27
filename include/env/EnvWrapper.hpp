#pragma once

#include "env/Environment.hpp"

namespace doomrl {
    class EnvWrapper : public Environment {
    protected:
        Environment& inner;

    public:
        explicit EnvWrapper(Environment& inner);

        int numActions() const override;

        StepResult step(int action) override;

        cv::Mat reset() override;

        bool closed() const override;
    };
}  // namespace doomrl
