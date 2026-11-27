#include "env/EnvWrapper.hpp"

namespace doomrl {
    EnvWrapper::EnvWrapper(Environment& inner) : inner{inner} {}

    int EnvWrapper::numActions() const {
        return inner.numActions();
    }

    StepResult EnvWrapper::step(int action) {
        return inner.step(action);
    }

    cv::Mat EnvWrapper::reset() {
        return inner.reset();
    }

    bool EnvWrapper::closed() const {
        return inner.closed();
    }
}  // namespace doomrl
