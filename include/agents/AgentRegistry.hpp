#pragma once

#include <functional>
#include <memory>
#include <string>
#include <unordered_map>

#include "Agent.hpp"
#include "config/Config.hpp"

namespace doomrl {
    class AgentRegistry {
    public:
        using Builder = std::function<std::unique_ptr<Agent>(const AgentConfig&, int)>;

        static AgentRegistry& instance();

        void add(const std::string& type, Builder builder);

        std::unique_ptr<Agent> make(const AgentConfig& config, int numActions) const;

    private:
        AgentRegistry() = default;

        std::unordered_map<std::string, Builder> builders;
    };
}  // namespace doomrl