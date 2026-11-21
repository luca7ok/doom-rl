#pragma once

#include <fstream>
#include <iostream>
#include <string>

namespace doomrl {
    class Logger {
    private:
        std::ofstream file;

    public:
        explicit Logger(const std::string& path);

        template <typename... Args>
        void log(const Args&... args) {
            (std::cout << ... << args) << '\n';
            (file << ... << args) << std::endl;
        }
    };
}  // namespace doomrl
