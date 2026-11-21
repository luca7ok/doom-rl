#include "util/Logger.hpp"

#include <stdexcept>

namespace doomrl {
    Logger::Logger(const std::string& path) : file{path} {
        if (!file) {
            throw std::runtime_error("failed to open log file: " + path);
        }
    }
}  // namespace doomrl
