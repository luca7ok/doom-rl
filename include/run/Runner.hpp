#pragma once

#include <vector>

#include "agents/Agent.hpp"
#include "env/Environment.hpp"
#include "util/Logger.hpp"

namespace doomrl {
    struct EpisodeStats {
        std::vector<double> rewards;

        double mean() const;
    };

    EpisodeStats runEpisodes(Environment& env, Agent& agent, int episodes, Logger& logger);
}  // namespace doomrl
