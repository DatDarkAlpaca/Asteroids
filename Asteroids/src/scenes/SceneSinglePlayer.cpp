#include "pch.h"
#include "SceneSinglePlayer.h"

void ast::SceneSinglePlayer::Initialize()
{
	CreateShip(registry, ShipForm::Standard, sf::Color::White, true);
}

void ast::SceneSinglePlayer::PollEvents(const sf::Event& event)
{
	InputSystem(registry);
}

void ast::SceneSinglePlayer::Update(float dt)
{
	ShootingSystem(registry, dt);

	DestroyOnBounds(registry);

	StayOnBounds(registry);

	PhysicsSystem(registry, dt);
}

void ast::SceneSinglePlayer::Render(sf::RenderWindow& window)
{
	RenderSystem(registry, window);
}
