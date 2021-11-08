#include "pch.h"
#include "PhysicsSystems.h"
#include "../components/Components.h"
#include "../prefabs/Asteroid.h"

// Physics:
void ast::AccelerateSystem(entt::registry& registry, float dt)
{
	auto view = registry.view<Transformable, Kinematics>();

	for (auto&& [entity, transformable, kinematics] : view.each())
	{
		const auto& rotation = transformable.transformable.getRotation();
		kinematics.velocity.x += kinematics.acceleration * cosf((rotation + 90.f) * 3.1415f / 180.f) * dt;
		kinematics.velocity.y += kinematics.acceleration * sinf((rotation + 90.f) * 3.1415f / 180.f) * dt;
	}
}

void ast::BrakeSystem(entt::registry& registry, float dt, float multiplier)
{
	auto view = registry.view<Kinematics>();

	for (auto&& [entity, kinematics] : view.each())
	{
		auto& velocity = kinematics.velocity;
		const auto multipliedDrag = kinematics.drag * dt * multiplier;

		// Applying drag:
		if (velocity.y > 0)
			velocity -= { 0, multipliedDrag };
		else if (velocity.y < 0)
			velocity += { 0, multipliedDrag };

		if (velocity.x > 0)
			velocity -= { multipliedDrag, 0 };

		else if (velocity.x < 0)
			velocity += { multipliedDrag, 0};

		// Normalizing:
		if (velocity.x > -0.001f && velocity.x < 0.001f)
			velocity.x = 0.f;
		if (velocity.y > -0.001f && velocity.y < 0.001f)
			velocity.y = 0.f;
	}
}

void ast::VelocityClampSystem(entt::registry& registry)
{
	auto view = registry.view<Kinematics>();

	for (auto&& [entity, kinematics] : view.each())
	{
		auto& velocity = kinematics.velocity;
		const auto maxSpeed = kinematics.maxSpeed;
		const auto minSpeed = kinematics.minSpeed;

		if (velocity.x >= maxSpeed)
			velocity.x = maxSpeed;

		if (velocity.x <= minSpeed)
			velocity.x = minSpeed;

		if (velocity.y >= maxSpeed)
			velocity.y = maxSpeed;

		if (velocity.y <= minSpeed)
			velocity.y = minSpeed;
	}
}

void ast::MoveSystem(entt::registry& registry, float dt)
{
	auto view = registry.view<Transformable, Kinematics, Shape>();

	for (auto&& [entity, transformable, kinematics, shape] : view.each())
	{
		auto& transf = transformable.transformable;
		transf.move(kinematics.velocity.x * dt, kinematics.velocity.y * dt);
	}
}

void ast::RotateSystem(entt::registry& registry, float dt)
{
	auto shipView = registry.view<Transformable, Kinematics, Input>();

	// Ship rotation:
	for (auto&& [entity, transformable, kinematics, input] : shipView.each())
	{
		auto& transf = transformable.transformable;
		transf.rotate(kinematics.angularSpeed * dt * input.rotationDirection);
	}

	// Todo: npc rotation.
	auto view = registry.view<Transformable, Kinematics>(entt::exclude<Ship>);
	for (auto&& [entity, transformable, kinematics] : view.each())
	{
		auto& transf = transformable.transformable;
		transf.rotate(kinematics.angularSpeed * dt);
	}
}

void ast::PhysicsSystem(entt::registry& registry, float dt)
{
	auto view = registry.view<Input>();
	for (const auto& [entity, input] : view.each())
	{
		if (input.accelerating)
			AccelerateSystem(registry, dt);
		else if (input.braking)
			BrakeSystem(registry, dt, 2.0f);
		else
			BrakeSystem(registry, dt, 1.0f);
	}

	InvencibilitySystem(registry, dt);

	VelocityClampSystem(registry);

	CollisionSystem(registry, dt);

	// FollowPlayerSystem(registry);

	RotateSystem(registry, dt);

	MoveSystem(registry, dt);

	HitboxSystem(registry);	
}

// Collisions:
void ast::ShipCollisionSystem(entt::registry& registry, float dt)
{
	auto shipView = registry.view<Transformable, Hitbox, Health, Invencibility, Ship>();
	auto asteroidView = registry.view<Transformable, Kinematics, Hitbox, AsteroidSize>();

	for (auto&& [entityShip, shipTransform, shipHitbox, health, invencibility, shipID] : shipView.each())
	{
		for (auto&& [entityAsteroid, asteroidTransform, kinematics, asteroidHitbox, asteroidSize] : asteroidView.each())
		{
			// Invencibility:
			if (invencibility.currentlyInvencible)
				return;

			if (shipHitbox.hitbox.getGlobalBounds().intersects(asteroidHitbox.hitbox.getGlobalBounds()))
			{
				invencibility.currentlyInvencible = true;

				// Lose health:
				health.health -= 1;

				// Destroy the asteroids:
				auto position = asteroidTransform.transformable.getPosition();
				auto direction = kinematics.velocity / kinematics.speed;

				if (asteroidSize.size > 1)
				{
					auto dirDown = sf::Vector2f(direction.x, -direction.y);
					auto dirLeft = sf::Vector2f(-direction.x, direction.y);

					AsteroidFactory::CreateAsteroid(registry, position, dirDown, regularAsteroid, asteroidSize.size - 1);
					AsteroidFactory::CreateAsteroid(registry, position, dirLeft, regularAsteroid, asteroidSize.size - 1);
				}

				registry.destroy(entityAsteroid);

				break;
			}
		}
	}
}

void ast::BulletCollisionSystem(entt::registry& registry)
{
	auto bulletView = registry.view<Transformable, Hitbox, Bullet>();
	auto asteroidView = registry.view<Transformable, Kinematics, Hitbox, AsteroidSize>();

	// Bullet-Asteroid collision:
	for (auto&& [entityBullet, bulletTransform, bulletHitbox, bulletID] : bulletView.each())
	{
		for (auto&& [entityAsteroid, asteroidTransform, kinematics, asteroidHitbox, asteroidSize] : asteroidView.each())
		{
			const auto& bt = bulletTransform.transformable.getTransform();
			const auto& at = asteroidTransform.transformable.getTransform();

			if (bulletHitbox.hitbox.getGlobalBounds().intersects(asteroidHitbox.hitbox.getGlobalBounds()))
			{
				auto position = asteroidTransform.transformable.getPosition();
				auto direction = kinematics.velocity / kinematics.speed;

				if (asteroidSize.size > 1)
				{
					auto dirDown = sf::Vector2f(direction.x, -direction.y);
					auto dirLeft = sf::Vector2f(-direction.x, direction.y);

					AsteroidFactory::CreateAsteroid(registry, position, dirDown, regularAsteroid, asteroidSize.size - 1);
					AsteroidFactory::CreateAsteroid(registry, position, dirLeft, regularAsteroid, asteroidSize.size - 1);
				}

				registry.destroy(entityBullet);
				registry.destroy(entityAsteroid);

				break;
			}
		}
	}
}

void ast::CollisionSystem(entt::registry& registry, float dt)
{
	ShipCollisionSystem(registry, dt);

	BulletCollisionSystem(registry);
}

// Hitbox:
void ast::HitboxSystem(entt::registry& registry)
{
	auto view = registry.view<Transformable, Hitbox>();

	for (auto&& [entity, transformable, hitbox] : view.each())
	{
		auto transform = transformable.transformable;

		hitbox.hitbox.setPosition(transform.getPosition());
		hitbox.hitbox.setRotation(transform.getRotation());
	}
}

void ast::InvencibilitySystem(entt::registry& registry, float dt)
{
	auto invencibilityView = registry.view<Invencibility>();
	auto shipView = registry.view<Shape>();

	for (auto&& [entityInvencibility, invencibility] : invencibilityView.each())
	{
		for (auto&& [entityShip, shape] : shipView.each())
		{
			if (invencibility.currentlyInvencible)
				shape.SetHit(true);
			else
				shape.SetHit(false);
		}

		if (invencibility.currentlyInvencible)
		{
			if (invencibility.currentTime <= invencibility.duration)
				invencibility.currentTime += dt;

			else
			{
				invencibility.currentTime = 0;
				invencibility.currentlyInvencible = false;
			}
		}
	}
}
