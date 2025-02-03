#pragma once

#include <memory>
#include <spdlog/spdlog.h> // removes necessity to include in each file

namespace game {
    class Game;
}

namespace scene {
    class IScene {
    public:
        virtual void enter(game::Game&) = 0;
        virtual void render(game::Game&) = 0;
        virtual void logic(game::Game&) = 0;
        virtual void exit() = 0;

        ~IScene() = default;
        IScene() = default;
    };
}  // namespace scene