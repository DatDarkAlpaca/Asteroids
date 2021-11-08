#pragma once
#include <entt.hpp>
#include "components/Components.h"

#include "PhysicsSystems.h"

// Todo: split these up into multiple files for further fucking with my life and pretending that i actually
// know what im doing by adding unnecessary overhead and overengineering this already overengineered game.
namespace ast
{
	// Organize these asap:
	void TrailSystem(entt::registry& registry, float dt);

	// Player:
	void RenderSystem(entt::registry& registry, sf::RenderWindow& window);

	void InputSystem(entt::registry& registry);

	void ShootingSystem(entt::registry& registry, float dt);

	// NPC:
	void StayOnBounds(entt::registry& registry);

	void DestroyOnBounds(entt::registry& registry);

	void FollowPlayerSystem(entt::registry& registry);

	// Health:
	bool GameoverHealthSystem(entt::registry& registry);
}