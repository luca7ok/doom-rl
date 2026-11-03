#include "agents/RandomAgent.hpp"

#include "agents/AgentRegistry.hpp"
#include "config/Config.hpp"

namespace doomrl {
    RandomAgent::RandomAgent(int numActions, int seed)
        : engine{static_cast<unsigned>(seed)}, distribution{0, numActions - 1} {
    }

    int RandomAgent::act(const cv::Mat&) {
        return distribution(engine);
    }

}  // namespace doomrl

namespace {
    const bool registered = [] {
        doomrl::AgentRegistry::instance().add(
            "random", [](const doomrl::AgentConfig& config, int numActions) {
                return std::make_unique<doomrl::RandomAgent>(numActions, config.seed);
            });
        return true;
    }();
}  // namespace