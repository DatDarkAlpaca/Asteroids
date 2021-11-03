#include "pch.h"
#include "SceneMainMenu.h"
#include "Components.h"
#include "Systems.h"
#include "Ship.h"

void ast::SceneMainMenu::Initialize()
{
	CreateShip(registry, ShipForm::Standard, sf::Color::Green, true);
}

void ast::SceneMainMenu::PollEvents(const sf::Event& event)
{
	InputSystem(registry);
}

void ast::SceneMainMenu::Update(float dt)
{
	PhysicsSystem(registry, dt);
}

void ast::SceneMainMenu::Render(sf::RenderWindow& window)
{
	window.clear();

	RenderSystem(registry, window);

	window.display();
}
