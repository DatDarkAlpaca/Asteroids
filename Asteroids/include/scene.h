#pragma once
#include <SFML/Graphics.hpp>
#include "scene_manager.h"

class SceneManager;

class Scene
{
public:
    virtual ~Scene() { }

public:
    void setSceneManager(SceneManager* sceneManager) { m_sceneManager = sceneManager; }

    virtual void initialize() {}

    virtual void update(float dt) = 0;

    virtual void handleInput() = 0;

    virtual void render(sf::RenderWindow* m_window) = 0;

protected:
    SceneManager* m_sceneManager = nullptr;
};