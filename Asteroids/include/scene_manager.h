#pragma once
#include <SFML/Graphics.hpp>
#include <map>

class Game;
class Scene;

class SceneManager
{
public:
    SceneManager(Game* game);
    ~SceneManager();

public:
    void selectScene(std::string&& sceneName);

    inline Scene* getScene(std::string&& sceneName) { return m_scenes[sceneName]; }

    void addScene(std::string&& sceneName, Scene* scene);

    void removeScene(std::string&& sceneName);

public:
    void initialize();

    void update(float dt);

    void handleInput();

    void render(sf::RenderWindow* m_window);

private:
    std::map<std::string, Scene*> m_scenes;
    Scene* m_currentScene = nullptr;

public:
    Game* game;
};