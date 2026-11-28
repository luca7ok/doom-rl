#include "util/MetricsWriter.hpp"

#include <stdexcept>

namespace doomrl {
    MetricsWriter::MetricsWriter(const std::string& path) : file{path} {
        if (!file) {
            throw std::runtime_error("failed to open metrics file: " + path);
        }
        file << "episode,reward,steps" << std::endl;
    }

    void MetricsWriter::write(int episode, double reward, int steps) {
        file << episode << ',' << reward << ',' << steps << std::endl;
    }
}  // namespace doomrl
