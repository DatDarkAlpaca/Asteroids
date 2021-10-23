#pragma once
#include <SFML/Graphics.hpp>

#include "settings.h"
#include "button.h"
#include "scene.h"

#include "asteroid.h"

#include <iostream>

class SceneMainMenu : public Scene
{
public:
    ~SceneMainMenu()
    {
        delete m_singlePlayButton;
        delete m_font;
        delete m_exitButton;
    }

public:
    void initialize() override;

    void update(float dt) override;

    void handleInput() override {}

    void render(sf::RenderWindow* m_window) override;

private:
    sf::Font* m_font = nullptr;
    sf::Text m_gameTitle;
    Button *m_singlePlayButton = nullptr, *m_exitButton = nullptr;

    std::vector<Asteroid*> m_asteroids;
};