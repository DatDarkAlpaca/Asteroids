#pragma once
#include <SFML/Graphics.hpp>

#include "scene_manager.h"
#include "settings.h"

#include "scene_main_menu.h"
#include "scene_playing.h"

class SceneManager;

class Game
{
public:
    Game() : m_gen(m_rd()) {}

public:
    void run();

    void pollEvents();

    void update(float dt);

    void render();

private:
    void handleAspectRatio();

    void createWindow();

    void createScenes();

public:
    sf::RenderWindow* getWindow() const { return m_window; }

private:
    sf::RenderWindow* m_window = nullptr;
    SceneManager* m_sceneManager = nullptr;

private:
    std::random_device m_rd;
    std::mt19937_64 m_gen;
};