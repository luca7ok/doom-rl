#include "config/Config.hpp"

#include "yaml-cpp/yaml.h"

namespace doomrl {

    EnvConfig loadEnvConfig(const std::string& path) {
        const YAML::Node config = YAML::LoadFile(path);

        EnvConfig result;
        result.scenario = config["scenario"].as<std::string>(result.scenario);
        result.frameSkip = config["frame_skip"].as<int>(result.frameSkip);
        result.render = config["render"].as<bool>(result.render);

        return result;
    }

    AgentConfig loadAgentConfig(const std::string& path) {
        const YAML::Node config = YAML::LoadFile(path);

        AgentConfig result;
        result.type = config["type"].as<std::string>(result.type);
        result.seed = config["seed"].as<int>(result.seed);
        result.episodes = config["episodes"].as<int>(result.episodes);

        return result;
    }
}  // namespace doomrl