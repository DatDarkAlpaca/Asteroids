#include "pch.h"
#include "SceneMainMenu.h"
#include "components/Components.h"
#include "systems/Systems.h"
#include "prefabs/Asteroid.h"
#include "prefabs/Ship.h"

void ast::SceneMainMenu::Initialize()
{
	CreateShip(registry, ShipForm::Standard, sf::Color::White, false);
	back.setFillColor(sf::Color(10, 10, 10, 255));
	back.setSize({ 2000, 2000 });
}

void ast::SceneMainMenu::PollEvents(const sf::Event& event)
{
	InputSystem(registry);
}

void ast::SceneMainMenu::Update(float dt)
{
	ShootingSystem(registry, dt);

	DestroyOnBounds(registry);

	StayOnBounds(registry);

	PhysicsSystem(registry, dt);
}

void ast::SceneMainMenu::Render(sf::RenderWindow& window)
{
	window.draw(back);

	RenderSystem(registry, window);
}
