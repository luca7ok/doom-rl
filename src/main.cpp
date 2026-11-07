#include <iostream>

#include "agents/AgentRegistry.hpp"
#include "app/Cli.hpp"
#include "config/Config.hpp"
#include "env/ViZDoomEnv.hpp"
#include "run/Runner.hpp"

using namespace doomrl;

int main(int argc, char** argv) {
    try {
        const CliArgs args = parseArgs(argc, argv);
        const EnvConfig envConfig = loadEnvConfig("configs/env/" + args.env + ".yaml");
        const AgentConfig agentConfig = loadAgentConfig("configs/agent/" + args.agent + ".yaml");

        ViZDoomEnv env(envConfig, static_cast<unsigned>(agentConfig.seed));
        auto agent = AgentRegistry::instance().make(agentConfig, env.numActions());

        const EpisodeStats stats = runEpisodes(env, *agent, agentConfig.episodes);

        for (size_t i = 0; i < stats.rewards.size(); i++) {
            std::cout << "episode " << i + 1 << ": reward " << stats.rewards[i] << '\n';
        }
        std::cout << "mean reward " << stats.mean() << '\n';
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
