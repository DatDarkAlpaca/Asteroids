#include "pch.h"
#include "Core.h"
#include "Application.h"
#include "scenes/SceneMainMenu.h"
#include "utils/Random.h"

void ast::Application::Run()
{
	InitializeRandomSystem();

	CreateWindow();

	CreateView();

	CreateScenes();

	sf::Clock clock;
	while (m_Window->isOpen())
	{
		float dt = clock.restart().asSeconds();

		PollEvents();

		Update(dt);

		Render();
	}
}

void ast::Application::PollEvents()
{
	sf::Event event;
	while (m_Window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
			m_Window->close();

		// Todo: fix it:
		if (event.type == sf::Event::Resized)
			m_MainView = AdjustLetterboxView(m_MainView, event.size.width, event.size.height);
		
		m_SceneManager.PollEvents(event);
	}
}

void ast::Application::Update(float dt)
{
	// m_Window->setView(m_MainView);

	m_SceneManager.Update(dt);
}

void ast::Application::Render()
{
	m_SceneManager.Render(*m_Window);
}

void ast::Application::InitializeRandomSystem()
{
	rand.seed(std::random_device{}());
}

void ast::Application::CreateScenes()
{
	m_SceneManager.AddScene(SceneType::MainMenu, std::make_shared<SceneMainMenu>());

	m_SceneManager.SelectScene(SceneType::MainMenu);
}

sf::View& ast::Application::AdjustLetterboxView(sf::View& view, int windowWidth, int windowHeight)
{
	float currentRatio = float(windowWidth / windowHeight);
	float viewRatio = view.getSize().x / (float)view.getSize().y;

	bool horizontalSpacing = currentRatio >= viewRatio;

	float sizeX = 1.f, sizeY = 1.f;
	float positionX = 0.f, positionY = 0.f;

	if (horizontalSpacing)
	{
		sizeX = viewRatio / currentRatio;
		positionX = (1 - sizeX) / 2.f;
	}
	else
	{
		sizeY = currentRatio / viewRatio;
		positionY = (1 - sizeY) / 2.f;
	}

	view.setViewport(sf::FloatRect({ positionX, positionY }, { sizeX, sizeY }));

	return view;
}

void ast::Application::CreateView()
{
	m_MainView.setSize((float)WindowWidth, (float)WindowHeight);
	m_MainView.setCenter(m_MainView.getSize().x / 2, m_MainView.getSize().y / 2);
	m_MainView = AdjustLetterboxView(m_MainView, WindowWidth, WindowHeight);
}

void ast::Application::CreateWindow()
{
	if (m_Window != nullptr && m_Window->isOpen())
		m_Window->close();

	sf::ContextSettings Settings;
	Settings.antialiasingLevel = 8;

	m_Window = new sf::RenderWindow(sf::VideoMode(WindowWidth, WindowHeight), 
		                            WindowTitle, sf::Style::Default, Settings);

	m_Window->setVerticalSyncEnabled(true);
}
