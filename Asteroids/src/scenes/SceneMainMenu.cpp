#include "pch.h"
#include "SceneMainMenu.h"
#include "components/Components.h"
#include "systems/Systems.h"
#include "prefabs/Asteroid.h"
#include "prefabs/Ship.h"
#include "../application/window/WindowHandler.h"
#include "../utils/Utilities.h"


void ast::SceneMainMenu::Initialize()
{
	// Todo: Store the font into the future engine's resources manager:
	m_Font = new sf::Font();
	if (!m_Font->loadFromFile("res/free-pixel.ttf"))
		std::cout << "[Error]: Failed to initialize font free-pixel.ttf\n";

	// Background:
	m_Background.setFillColor(sf::Color(10, 10, 10, 255));
	m_Background.setSize({ (float)WorldWidth, (float)WorldHeight });

	// Alpha:
	auto alpha = std::make_shared<gui::Label>("version 0.1s", *m_Font, 18);
	alpha->setPosition({ 0, (float)WorldHeight - alpha->getText().getGlobalBounds().height * 2 });

	// Title:
	auto titleBar = std::make_shared<gui::Label>("Asteroids", *m_Font, 60);
	titleBar->setPosition ({
		(float)WorldWidth / 2 - titleBar->getText().getGlobalBounds().width / 2,
		(float)WorldHeight / 2 - 150
	});

	// Play button:
	sf::RectangleShape buttonShape({300, 50});
	auto play = std::make_shared<gui::Button>(buttonShape, *m_Font, 36, gui::standaloneText, gui::noneHolder, false);
	play->SetText("Play");
	play->setPosition({
		(float)WorldWidth / 2 - play->shape().getGlobalBounds().width / 2,
		(float)WorldHeight / 2
	});

	// Settings button:
	auto settings = std::make_shared<gui::Button>(buttonShape, *m_Font, 36, gui::standaloneText, gui::noneHolder, false);
	settings->SetText("Settings");
	settings->setPosition({
		(float)WorldWidth / 2 - play->shape().getGlobalBounds().width / 2,
		(float)WorldHeight / 2 + 50
	});

	// Workshop button:
	auto workshop = std::make_shared<gui::Button>(buttonShape, *m_Font, 36, gui::standaloneText, gui::noneHolder, false);
	workshop->SetText("Workshop");
	workshop->setPosition({
		(float)WorldWidth / 2 - play->shape().getGlobalBounds().width / 2,
		(float)WorldHeight / 2 + 100
	});

	// Exit button:
	auto exit = std::make_shared<gui::Button>(buttonShape, *m_Font, 36, gui::standaloneText, gui::noneHolder, false);
	exit->SetText("Exit");
	exit->setPosition({
		(float)WorldWidth / 2 - play->shape().getGlobalBounds().width / 2,
		(float)WorldHeight / 2 + 150
	});
	exit->SetCallback([this]() { windowHolder.window->close(); });

	m_MainContainer.Pack(alpha);
	m_MainContainer.Pack(titleBar);

	m_MainContainer.Pack(play);
	m_MainContainer.Pack(settings);
	m_MainContainer.Pack(workshop);
	m_MainContainer.Pack(exit);
}

void ast::SceneMainMenu::PollEvents(const sf::Event& event)
{
	InputSystem(registry);

	m_MainContainer.PollEvents(event);
}

void ast::SceneMainMenu::Update(float dt)
{
	/*ShootingSystem(registry, dt);

	DestroyOnBounds(registry);

	StayOnBounds(registry);

	PhysicsSystem(registry, dt);*/

	m_MainContainer.Update(dt);
}

void ast::SceneMainMenu::Render(sf::RenderWindow& window)
{
	window.draw(m_Background);

	RenderSystem(registry, window);

	window.draw(m_MainContainer);
}

void ast::SceneMainMenu::Cleanup()
{
	delete m_Font;
}
