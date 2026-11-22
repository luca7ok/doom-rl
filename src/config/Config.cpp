#include "config/Config.hpp"

#include "yaml-cpp/yaml.h"

namespace {
    doomrl::ModeConfig loadModeConfig(const YAML::Node& node, doomrl::ModeConfig result) {
        if (node) {
            result.episodes = node["episodes"].as<int>(result.episodes);
        }
        return result;
    }
}  // namespace

namespace doomrl {

    EnvConfig loadEnvConfig(const std::string& path) {
        const YAML::Node config = YAML::LoadFile(path);

        EnvConfig result;
        result.scenario = config["scenario"].as<std::string>(result.scenario);
        result.frameSkip = config["frame_skip"].as<int>(result.frameSkip);

        return result;
    }

    AgentConfig loadAgentConfig(const std::string& path) {
        const YAML::Node config = YAML::LoadFile(path);

        AgentConfig result;
        result.type = config["type"].as<std::string>(result.type);
        result.seed = config["seed"].as<int>(result.seed);
        result.train = loadModeConfig(config["train"], result.train);
        result.run = loadModeConfig(config["run"], result.run);

        return result;
    }
}  // namespace doomrl