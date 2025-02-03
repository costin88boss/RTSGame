#pragma once

#include <memory>

namespace scene {
    class IScene;
}

namespace game {
    class Game {
    public:
        Game();
        ~Game();

    private:
        std::unordered_map<std::string, std::unique_ptr<scene::IScene>> m_registeredScenes;

        scene::IScene* m_currScene;
        scene::IScene* m_nextScene;
    };

}  // namespace game
