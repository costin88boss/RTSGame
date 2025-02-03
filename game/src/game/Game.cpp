#include "../util/PhysFsStream.hpp"
#include <map>
#include <physfs.h>
#include <SFML/Graphics.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/stopwatch.h>
#include "../scene/Splash.hpp"
#include "../scene/Menu.hpp"
#include "Game.hpp"

namespace game {
    // may get removed soon
    constexpr auto GAMENAME = "RTSGame";
    constexpr auto DEFWIDTH = 720;
    constexpr auto DEFHEIGHT = 480;

    Game::Game() {
        PHYSFS_addToSearchPath("data.bin", 0);
        PHYSFS_setWriteDir(0);

        auto window = sf::RenderWindow(sf::VideoMode({DEFWIDTH, DEFHEIGHT}), GAMENAME);

        sf::Clock clock;
        float deltaTime = 0.0f;
        float tickTime = 0;
        const int tickRate = 60;

        util::PhysFsStream pfStream("fonts/Roboto-Regular.ttf");

        window.setFramerateLimit(150);

        window.setVerticalSyncEnabled(true);

        const sf::Font font(pfStream);
        sf::Text text(font);
        text.setFillColor(sf::Color::Green);

        this->m_registeredScenes.insert({"splash", std::make_unique<scene::Splash>()});
        this->m_registeredScenes.insert({"menu", std::make_unique<scene::Menu>()});

        m_nextScene = 0;
        m_currScene = this->m_registeredScenes.at("splash").get();

        while (window.isOpen()) {
            while (const std::optional event = window.pollEvent()) {
                if (event->is<sf::Event::Closed>()) {
                    window.close();
                }
                if (auto ev = event->getIf<sf::Event::KeyPressed>()) {
                }
            }

            m_currScene->render(*this);

            deltaTime = clock.getElapsedTime().asSeconds();

            tickTime += deltaTime;

            while (tickTime >= 0) {
                tickTime -= 1.f / tickRate;
            }
            clock.restart();

            text.setString(std::to_string(static_cast<float>(deltaTime)));

            window.clear();
            window.draw(text);
            window.display();

            if (this->m_nextScene) {
                this->m_currScene->exit();
                this->m_currScene = this->m_nextScene;
                this->m_currScene->enter(*this);
            }
        }
    }

    Game::~Game() {
        PHYSFS_deinit();
    }
}  // namespace game
