#include "scene_manager.h"
#include "scene.h"

SceneManager::~SceneManager()
{
    std::map<std::string, Scene*> m_scenes;

    for(auto& scene : m_scenes)
    {
        delete scene.second;
        m_scenes.erase(scene.first);
    }

    delete m_currentScene;
}

void SceneManager::selectScene(std::string&& sceneName)
{
    m_currentScene = m_scenes[sceneName];

    m_currentScene->initialize();
}

void SceneManager::addScene(std::string&& sceneName, Scene* scene)
{
    m_scenes[sceneName] = scene;
    m_scenes[sceneName]->setSceneManager(this);
}

void SceneManager::removeScene(std::string&& sceneName)
{
    delete m_scenes.at(sceneName);
    m_scenes.erase(sceneName);
}

void SceneManager::initialize()
{
    if (m_currentScene == nullptr)
        return;

    m_currentScene->initialize();
}

void SceneManager::update(float dt)
{
    if (m_currentScene == nullptr)
        return;

    m_currentScene->update(dt);
}

void SceneManager::handleInput()
{
    if (m_currentScene == nullptr)
        return;

    m_currentScene->handleInput();
}

void SceneManager::render(sf::RenderWindow* m_window)
{
    if (m_currentScene == nullptr)
        return;

    m_currentScene->render(m_window);
}
