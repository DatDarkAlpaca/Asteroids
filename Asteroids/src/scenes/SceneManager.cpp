#include "pch.h"
#include "SceneManager.h"

void ast::SceneManager::PollEvents(const sf::Event& event)
{
	if (m_CurrentScene == nullptr)
		return;

	m_CurrentScene->PollEvents(event);
}

void ast::SceneManager::Update(float dt)
{
	if (m_CurrentScene == nullptr)
		return;

	m_CurrentScene->Update(dt);
}

void ast::SceneManager::Render(sf::RenderWindow& window)
{
	if (m_CurrentScene == nullptr)
		return;

	m_CurrentScene->Render(window);
}

void ast::SceneManager::SelectScene(SceneType sceneName)
{
	if (m_Scenes.find(sceneName) == m_Scenes.end())
		return;

	if (m_CurrentScene != nullptr)
		m_CurrentScene->Cleanup();

	m_CurrentScene = m_Scenes[sceneName];
	m_CurrentScene->Initialize();
}
