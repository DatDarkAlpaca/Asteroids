#pragma once
#include <entt.hpp>
#include "Components.h"

namespace ast
{
	void RenderSystem(entt::registry& registry, sf::RenderWindow& window);

	void InputSystem(entt::registry& registry);

	void ShootingSystem(entt::registry& registry, float dt);


	void AccelerateSystem(entt::registry& registry, float dt);

	void BrakeSystem(entt::registry& registry, float dt, float multiplier = 1.0f);

	void VelocityClampSystem(entt::registry& registry);
	
	void MoveSystem(entt::registry& registry, float dt);

	void RotateSystem(entt::registry& registry, float dt);

	void PhysicsSystem(entt::registry& registry, float dt);
}