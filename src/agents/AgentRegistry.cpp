#include "agents/AgentRegistry.hpp"

#include <memory>
#include <stdexcept>

#include "config/Config.hpp"

namespace doomrl {
    AgentRegistry& AgentRegistry::instance() {
        static AgentRegistry registry;
        return registry;
    }

    void AgentRegistry::add(const std::string& type, Builder builder) {
        builders[type] = std::move(builder);
    }

    std::unique_ptr<Agent> AgentRegistry::make(const AgentConfig& config, int numActions) const {
        const auto it = builders.find(config.type);
        if (it == builders.end()) {
            throw std::runtime_error("unknown agent: " + config.type);
        }
        return it->second(config, numActions);
    }
}  // namespace doomrl