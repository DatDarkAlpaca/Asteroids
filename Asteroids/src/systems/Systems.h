#pragma once
#include <entt.hpp>
#include "components/Components.h"

// Todo: split these up into multiple files for further fucking with my life and pretending that i actually
// know what im doing by adding unnecessary overhead and overengineering this already overengineered game.
namespace ast
{
	// Player:
	void RenderSystem(entt::registry& registry, sf::RenderWindow& window);

	void InputSystem(entt::registry& registry);

	void ShootingSystem(entt::registry& registry, float dt);

	void StayOnBounds(entt::registry& registry);

	// NPC:
	void DestroyOnBounds(entt::registry& registry);

	void FollowPlayerSystem(entt::registry& registry);

	void HitboxSystem(entt::registry& registry);

	// Collisions:
	void ShipCollisionSystem(entt::registry& registry, float dt);

	void BulletCollisionSystem(entt::registry& registry);

	void CollisionSystem(entt::registry& registry, float dt);

	// Health:
	bool GameoverHealthSystem(entt::registry& registry);

	// Physics:
	void AccelerateSystem(entt::registry& registry, float dt);

	void BrakeSystem(entt::registry& registry, float dt, float multiplier = 1.0f);

	void VelocityClampSystem(entt::registry& registry);
	
	void MoveSystem(entt::registry& registry, float dt);

	void RotateSystem(entt::registry& registry, float dt);

	void PhysicsSystem(entt::registry& registry, float dt);
}