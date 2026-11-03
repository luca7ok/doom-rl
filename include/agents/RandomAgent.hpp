#pragma once

#include <random>

#include "agents/Agent.hpp"

namespace doomrl {
    class RandomAgent : public Agent {
    private:
        std::mt19937 engine;
        std::uniform_int_distribution<int> distribution;

    public:
        RandomAgent(int numActions, int seed);

        int act(const cv::Mat& observation) override;
    };
}  // namespace doomrl