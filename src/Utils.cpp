#include "Utils.hpp"

#include <filesystem>
#include <stdexcept>

namespace doomrl {
    std::string getScenarioPath(const std::string& name) {
        const std::filesystem::path local = std::filesystem::path("scenarios") / (name + ".cfg");
        if (std::filesystem::exists(local)) {
            return local.string();
        }

        const std::filesystem::path vizdoomPath =
            std::filesystem::path(VIZDOOM_SCENARIOS_DIR) / (name + ".cfg");
        if (std::filesystem::exists(vizdoomPath)) {
            return vizdoomPath.string();
        }

        throw std::runtime_error("scenario not found: " + name);
    }
}  // namespace doomrl