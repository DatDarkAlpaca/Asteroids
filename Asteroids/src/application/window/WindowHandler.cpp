#include "pch.h"
#include "WindowHandler.h"
#include "application/Core.h"

void ast::WindowHandler::CreateWindow()
{
	if (m_Window.isOpen())
		m_Window.close();

	sf::ContextSettings Settings;
	Settings.antialiasingLevel = 8;

	if (!m_Fullscreen)
		m_Window.create(sf::VideoMode(WorldWidth, WorldHeight),
			WindowTitle, sf::Style::Titlebar + sf::Style::Close, Settings);
	else
		m_Window.create(sf::VideoMode(WorldWidth, WorldHeight),
			WindowTitle, sf::Style::Fullscreen, Settings);

	m_Window.setFramerateLimit(60);
}

ast::WindowHandler::WindowHandler()
{
	CreateWindow();

	CreateView();
}

sf::View& ast::WindowHandler::ApplyLetterboxView(sf::View view, int windowWidth, int windowHeight)
{
	float windowRatio = windowWidth / (float)windowHeight;
	float viewRatio = view.getSize().x / (float)view.getSize().y;
	float sizeX = 1, sizeY = 1;
	float posX = 0, posY = 0;

	bool horizontalSpacing = true;
	if (windowRatio < viewRatio)
		horizontalSpacing = false;

	if (horizontalSpacing) 
	{
		sizeX = viewRatio / windowRatio;
		posX = (1 - sizeX) / 2.f;
	}

	else 
	{
		sizeY = windowRatio / viewRatio;
		posY = (1 - sizeY) / 2.f;
	}

	view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));

	return view;
}

void ast::WindowHandler::CreateView()
{
	m_MainView.setSize((float)WorldWidth, (float)WorldHeight);
	m_MainView.setCenter(m_MainView.getSize().x / 2, m_MainView.getSize().y / 2);
	m_MainView = ApplyLetterboxView(m_MainView, WorldWidth, WorldHeight);
}
