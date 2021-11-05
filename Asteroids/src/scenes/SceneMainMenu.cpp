#include "pch.h"
#include "SceneMainMenu.h"
#include "components/Components.h"
#include "systems/Systems.h"
#include "prefabs/Asteroid.h"
#include "prefabs/Ship.h"

void ast::SceneMainMenu::Initialize()
{
	CreateShip(registry, ShipForm::Standard, sf::Color::White, false);
}

void ast::SceneMainMenu::PollEvents(const sf::Event& event)
{
	InputSystem(registry);
}

void ast::SceneMainMenu::Update(float dt)
{
	ShootingSystem(registry, dt);

	DestroyOnBounds(registry);

	PhysicsSystem(registry, dt);
}

void ast::SceneMainMenu::Render(sf::RenderWindow& window)
{
	RenderSystem(registry, window);
}
