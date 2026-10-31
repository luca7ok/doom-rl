#pragma once

#include <string>

namespace doomrl {
    struct EnvConfig {
        std::string scenario = "basic";
        int frameSkip = 4;
        bool render = false;
    };

    struct AgentConfig {
        std::string type = "random";
        int seed = 0;
        int episodes = 1;
    };

    EnvConfig loadEnvConfig(const std::string& path);

    AgentConfig loadAgentConfig(const std::string& path);

}  // namespace doomrl