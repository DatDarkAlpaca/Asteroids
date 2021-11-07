#include "pch.h"
#include "Core.h"
#include "Application.h"
#include "scenes/SceneMainMenu.h"
#include "utils/Random.h"

void ast::Application::Run()
{
	InitializeSeed();

	CreateScenes();
	
	sf::Clock clock;
	while (m_WindowHandler.GetWindow().isOpen())
	{
		float dt = clock.restart().asSeconds();

		PollEvents();

		Update(dt);

		Render();
	}
}

void ast::Application::PollEvents()
{
	sf::Event e;
	while (m_WindowHandler.GetWindow().pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			m_WindowHandler.GetWindow().close();

		if (e.type == sf::Event::Resized)
			m_WindowHandler.ApplyResizedView(e);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::F11))
			m_WindowHandler.ToggleFullscreen();

		m_SceneManager.PollEvents(e);
	}
}

void ast::Application::Update(float dt)
{
	m_SceneManager.Update(dt);
}

void ast::Application::Render()
{
	m_WindowHandler.GetWindow().clear();

	m_WindowHandler.SetView();
	
	m_SceneManager.Render(m_WindowHandler.GetWindow());

	m_WindowHandler.GetWindow().display();
}

void ast::Application::InitializeSeed()
{
	rand.seed(std::random_device{}());
}

void ast::Application::CreateScenes()
{
	m_SceneManager.AddScene(SceneType::MainMenu, std::make_shared<SceneMainMenu>());

	m_SceneManager.SelectScene(SceneType::MainMenu);
}
