#include "agents/RandomAgent.hpp"

namespace doomrl {
    RandomAgent::RandomAgent(int numActions, int seed)
        : engine{static_cast<unsigned>(seed)}, distribution{0, numActions - 1} {
    }

    int RandomAgent::act(const cv::Mat&) {
        return distribution(engine);
    }

}  // namespace doomrl