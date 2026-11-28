#include <filesystem>
#include <iostream>

#include "agents/AgentRegistry.hpp"
#include "app/Cli.hpp"
#include "config/Config.hpp"
#include "env/FrameStackEnv.hpp"
#include "env/PreprocessEnv.hpp"
#include "env/ViZDoomEnv.hpp"
#include "run/Runner.hpp"
#include "util/Logger.hpp"
#include "util/MetricsWriter.hpp"
#include "util/Utils.hpp"

using namespace doomrl;

int main(int argc, char** argv) {
    try {
        const CliArgs args = parseArgs(argc, argv);
        EnvConfig envConfig = loadEnvConfig("configs/env/" + args.env + ".yaml");
        envConfig.render = args.mode == "run";
        const AgentConfig agentConfig = loadAgentConfig("configs/agent/" + args.agent + ".yaml");

        const std::string runDir = "runs/" + args.env + "/" + args.agent;
        const std::string startedAt = timestamp();
        std::filesystem::create_directories(runDir + "/metrics");
        Logger logger(runDir + "/run.log");
        logger.log("=== ", startedAt, " mode ", args.mode, ", env ", args.env, ", agent ",
                   args.agent);
        MetricsWriter metrics(runDir + "/metrics/" + startedAt + "_" + args.mode + ".csv");

        ViZDoomEnv game(envConfig, static_cast<unsigned>(agentConfig.seed));
        PreprocessEnv preprocessed(game, agentConfig.obsSize);
        FrameStackEnv env(preprocessed, agentConfig.frameStack);
        auto agent = AgentRegistry::instance().make(agentConfig, env.numActions());

        const ModeConfig& mode = args.mode == "train" ? agentConfig.train : agentConfig.run;
        const EpisodeStats stats = runEpisodes(env, *agent, mode.episodes, logger, metrics);

        logger.log("mean reward ", stats.mean());
    } catch (const std::exception& e) {
        std::cerr << "error: " << e.what() << '\n';
        return 1;
    }
    return 0;
}
