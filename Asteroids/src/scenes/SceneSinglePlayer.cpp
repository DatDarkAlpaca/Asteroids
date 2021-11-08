#include "pch.h"
#include "SceneSinglePlayer.h"

void ast::SceneSinglePlayer::Initialize()
{
	CreateShip(registry, ShipForm::Standard, sf::Color::White, false);

	// FPS Label Clock:
	m_SceneTimer.SetTime(0.3f);
	m_AsteroidTimer.SetTime(2.f);

	// FPS Label:
	m_FPSLabel = std::make_shared<gui::Label>("60 fps", *fontHolder.font, 18);
	m_FPSLabel->setPosition({ 0, 0 });
	
	m_MainContainer.Pack(m_FPSLabel);
}

void ast::SceneSinglePlayer::PollEvents(const sf::Event& event)
{
	InputSystem(registry);

	m_MainContainer.PollEvents(event);
}

void ast::SceneSinglePlayer::Update(float dt)
{
	// FPS Label:
	m_SceneTimer.Update(dt);

	if (m_SceneTimer.IsDone())
	{
		m_FPSLabel->SetText(std::to_string(int(1 / dt)) + " fps");
		m_SceneTimer.Restart();
	}

	// GUI:
	m_MainContainer.Update(dt);

	// Rudimentar game progression:
	// Todo: wave system.
	if (!m_Gameover)
	{
		// Asteroid Spawn:
		m_AsteroidTimer.Update(dt);

		if (m_AsteroidTimer.IsDone())
		{
			AsteroidFactory::SpawnAsteroid(registry, regularAsteroid, 3);
			m_AsteroidTimer.Restart();
		}

		// Physics & Input:
		ShootingSystem(registry, dt);

		DestroyOnBounds(registry);

		StayOnBounds(registry);

		PhysicsSystem(registry, dt);

		// Trail:
		TrailSystem(registry, dt);

		// Check whether it's a gameover:
		m_Gameover = GameoverHealthSystem(registry);	
	}
}

void ast::SceneSinglePlayer::Render(sf::RenderWindow& window)
{
	RenderSystem(registry, window);

	window.draw(m_MainContainer);
}
