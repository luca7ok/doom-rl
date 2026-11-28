#pragma once

#include <vector>

#include "agents/Agent.hpp"
#include "env/Environment.hpp"
#include "util/Logger.hpp"
#include "util/MetricsWriter.hpp"

namespace doomrl {
    struct EpisodeStats {
        std::vector<double> rewards;

        double mean() const;
    };

    EpisodeStats runEpisodes(Environment& env, Agent& agent, int episodes, Logger& logger,
                             MetricsWriter& metrics);
}  // namespace doomrl
