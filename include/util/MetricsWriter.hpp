#pragma once

#include <fstream>
#include <string>

namespace doomrl {
    class MetricsWriter {
    private:
        std::ofstream file;

    public:
        explicit MetricsWriter(const std::string& path);

        void write(int episode, double reward, int steps);
    };
}  // namespace doomrl
