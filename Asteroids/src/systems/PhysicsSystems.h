#pragma once

namespace ast
{
	// Physics:
	void AccelerateSystem(entt::registry& registry, float dt);

	void BrakeSystem(entt::registry& registry, float dt, float multiplier = 1.0f);

	void VelocityClampSystem(entt::registry& registry);

	void MoveSystem(entt::registry& registry, float dt);

	void RotateSystem(entt::registry& registry, float dt);

	void PhysicsSystem(entt::registry& registry, float dt);

	// Collisions:
	void ShipCollisionSystem(entt::registry& registry, float dt);

	void BulletCollisionSystem(entt::registry& registry);

	void CollisionSystem(entt::registry& registry, float dt);

	// Hitbox:
	void HitboxSystem(entt::registry& registry);

	void InvencibilitySystem(entt::registry& registry, float dt);
}