#include "pch.h"
#include "Core.h"
#include "Application.h"
#include "scenes/SceneMainMenu.h"
#include "utils/Random.h"

void ast::Application::Run()
{
	InitializeSeed();

	CreateWindow();

	CreateScenes();
	
	sf::Clock clock;
	while (m_Window.isOpen())
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
	while (m_Window.pollEvent(e))
	{
		if (e.type == sf::Event::Closed)
			m_Window.close();

		if (e.type == sf::Event::Resized)
			m_MainView = ApplyLetterboxView(m_MainView, e.size.width, e.size.height);

		m_SceneManager.PollEvents(e);
	}
}

void ast::Application::Update(float dt)
{
	m_SceneManager.Update(dt);
}

void ast::Application::Render()
{
	m_Window.clear();

	//m_Window.setView(m_MainView);
	
	m_SceneManager.Render(m_Window);

	m_Window.display();
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

sf::View& ast::Application::ApplyLetterboxView(sf::View view, int windowWidth, int windowHeight)
{
	float windowRatio = windowWidth / (float)windowHeight;
	float viewRatio = view.getSize().x / (float)view.getSize().y;
	float sizeX = 1;
	float sizeY = 1;
	float posX = 0;
	float posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	if (horizontalSpacing) {
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
	}

	else {
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.f;
	}

	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

	return view;
}

void ast::Application::CreateView()
{
	m_MainView.setSize(WindowWidth, WindowHeight);
	m_MainView.setCenter(m_MainView.getSize().x / 2, m_MainView.getSize().y / 2);
	m_MainView = ApplyLetterboxView(m_MainView, WindowWidth, WindowHeight);
}

void ast::Application::CreateWindow()
{
	if (m_Window.isOpen())
		m_Window.close();

	sf::ContextSettings Settings;
	Settings.antialiasingLevel = 0;

	m_Window.create(sf::VideoMode(WindowWidth, WindowHeight),
		            WindowTitle, sf::Style::Resize + sf::Style::Close, Settings);
}
