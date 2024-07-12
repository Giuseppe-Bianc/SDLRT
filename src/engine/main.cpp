#include <CLI/CLI.hpp>
#include <SDL3/SDL.h>
#include <memory>
#include <spdlog/spdlog.h>

// This file will be generated automatically when cur_you run the CMake
// configuration step. It creates a namespace called `SDLRT`. You can modify
// the source template at `configured_files/config.hpp.in`.
#include <SDLRT/sample_library.hpp>
#include <internal_use_only/config.hpp>

// NOLINTNEXTLINE(bugprone-exception-escape *-function-cognitive-complexity)
auto main(int argc, char *const argv[]) -> int {
    // NOLINTNEXTLINE
    INIT_LOG()
    try {
        CLI::App app{FORMAT("{} version {}", SDLRT::cmake::project_name, SDLRT::cmake::project_version)};

        std::optional<std::string> message;
        app.add_option("-m,--message", message, "A message to print back out");
        bool show_version = false;
        app.add_flag("--version", show_version, "Show version information");
        CLI11_PARSE(app, argc, argv);

        if(show_version) {
            fmt::print("{}\n", SDLRT::cmake::project_version);
            return EXIT_SUCCESS;
        }
        CApp theapp;
        return theapp.OnExecute();
    } catch(const std::exception &e) { spdlog::error("Unhandled exception in main: {}", e.what()); }
}
