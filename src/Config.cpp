#include "Config.hpp"

#include "yaml-cpp/yaml.h"

namespace doomrl {

    EnvConfig loadEnvConfig(const std::string& path) {
        const YAML::Node config = YAML::LoadFile(path);

        const std::string scenario = config["scenario"].as<std::string>();
        const int frameSkip = config["frame_skip"].as<int>();
        const bool render = config["render"].as<bool>();

        return EnvConfig{scenario, frameSkip, render};
    }

    AgentConfig loadAgentConfig(const std::string& path) {
        const YAML::Node config = YAML::LoadFile(path);

        const std::string type = config["type"].as<std::string>();
        const int seed = config["seed"].as<int>();
        const int episodes = config["episodes"].as<int>();

        return AgentConfig{type, seed, episodes};
    }
}  // namespace doomrl