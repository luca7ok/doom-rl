#include "run/Runner.hpp"

#include <numeric>

namespace doomrl {
    double EpisodeStats::mean() const {
        if (rewards.empty()) {
            return 0.0;
        }
        return std::accumulate(rewards.begin(), rewards.end(), 0.0) / rewards.size();
    }

    EpisodeStats runEpisodes(Environment& env, Agent& agent, int episodes) {
        EpisodeStats stats;

        for (int episode = 0; episode < episodes; episode++) {
            cv::Mat observation = env.reset();
            double total = 0.0;
            bool done = false;

            while (!done) {
                const StepResult result = env.step(agent.act(observation));
                observation = result.observation;
                total += result.reward;
                done = result.done;
            }

            stats.rewards.push_back(total);
        }

        return stats;
    }
}  // namespace doomrl
