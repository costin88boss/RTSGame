#include "util/StreamLogger.hpp"
#include <iostream>
#include <SFML/System.hpp>
#include <spdlog/spdlog.h>
#include <physfs.h>
#include "game/Game.hpp"


int main(int argc, const char* argv[]) {
    PHYSFS_init(argv[0]);

    spdlog::set_level(spdlog::level::info);
    spdlog::enable_backtrace(32);

    for (size_t i = 0; i < argc; i++) {
        // strcmp rets 0 if equal 'lexicographically' (tf is this)
        if (!strcmp(argv[i], "-debug")) {
            spdlog::set_level(spdlog::level::trace);
            spdlog::disable_backtrace();
            spdlog::debug("[Debug on]");
        }
    }

    util::StreamLogger<> sfmlLogger(sf::err(), spdlog::default_logger());

    try {
        std::unique_ptr<game::Game> game = std::make_unique<game::Game>();
    } catch (std::exception ex) {
        spdlog::dump_backtrace();
        spdlog::critical("EXCEPTION: {}", ex.what());
    } catch (...) {
        spdlog::dump_backtrace();
        spdlog::critical("Unknown Exception Crash");
        return 1;
    }
}
