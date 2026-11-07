#pragma once

#include <string>

namespace doomrl {
    struct CliArgs {
        std::string env = "basic";
        std::string agent = "random";
    };

    CliArgs parseArgs(int argc, char** argv);
}  // namespace doomrl