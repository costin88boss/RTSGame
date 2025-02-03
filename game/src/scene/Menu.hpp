#pragma once

#include "IScene.hpp"

namespace scene {
    class Menu : public IScene {
    public:
        virtual void enter(game::Game&) override final;
        virtual void render(game::Game&) override final;
        virtual void logic(game::Game&) override final;
        virtual void exit() override final;
    };
}