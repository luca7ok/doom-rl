#include "app/Cli.hpp"

#include <CLI/CLI.hpp>
#include <stdexcept>

#include "CLI/CLI.hpp"

namespace doomrl {
    CliArgs parseArgs(int argc, char** argv) {
        CliArgs args;
        CLI::App app{"doom-rl"};

        app.add_option("--env", args.env, "environment config name")->capture_default_str();
        app.add_option("--agent", args.agent, "agent config name")->capture_default_str();

        try {
            app.parse(argc, argv);
        } catch (const CLI::ParseError& e) {
            std::exit(app.exit(e));
        }
        
        return args;
    }
}  // namespace doomrl