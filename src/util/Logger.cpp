#include "util/Logger.hpp"

#include <cstdint>
#include <filesystem>
#include <stdexcept>

namespace {
    constexpr std::uintmax_t maxLogBytes = 1'000'000;
}  // namespace

namespace doomrl {
    Logger::Logger(const std::string& path) {
        if (std::filesystem::exists(path) && std::filesystem::file_size(path) > maxLogBytes) {
            std::filesystem::rename(path, path + ".old");
        }

        const bool hasContent =
            std::filesystem::exists(path) && std::filesystem::file_size(path) > 0;

        file.open(path, std::ios::app);
        if (!file) {
            throw std::runtime_error("failed to open log file: " + path);
        }

        if (hasContent) {
            file << '\n';
        }
    }
}  // namespace doomrl
