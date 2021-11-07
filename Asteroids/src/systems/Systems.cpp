#include "pch.h"
#include "utils/Vector.h"
#include "systems/Systems.h"
#include "prefabs/Bullet.h"

void ast::RenderSystem(entt::registry& registry, sf::RenderWindow& window)
{
	auto view = registry.view<Transformable, Shape>();

	for (auto&& [entity, transformable, shape] : view.each())
		window.draw(shape, transformable.transformable.getTransform());
}

// Player Systems:
void ast::InputSystem(entt::registry& registry)
{
	auto view = registry.view<Input>();

	for (auto&& [entity, input] : view.each())
	{
		input.accelerating = sf::Keyboard::isKeyPressed(sf::Keyboard::W);

		input.braking = sf::Keyboard::isKeyPressed(sf::Keyboard::S);

		input.shooting = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			input.rotationDirection = 1;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			input.rotationDirection = -1;
		else
			input.rotationDirection = 0;		
	}
}

void ast::ShootingSystem(entt::registry& registry, float dt)
{
	auto view = registry.view<Transformable, Shooting, Input>();

	for (auto&& [entity, transformable, shooting, input] : view.each())
	{
		input.canShoot = shooting.cooldown >= shooting.cooldownMax;

		if(!input.canShoot)
			shooting.cooldown += dt;

		if (input.canShoot && input.shooting)
		{
			shooting.cooldown = 0;

			CreateBullet(registry, transformable.transformable.getPosition(), 
				                   transformable.transformable.getRotation(), regularBullet);
		}
	}
}

void ast::StayOnBounds(entt::registry& registry)
{
	auto shipView = registry.view<Transformable, StayInBounds, Kinematics>();

	for (auto&& [entity, transformable, stay, kinematics] : shipView.each())
	{
		auto& transf = transformable.transformable;
		const auto& x = transf.getPosition().x;
		const auto& y = transf.getPosition().y;

		if (x <= stay.left && kinematics.velocity.x < 0)
			transf.setPosition({ stay.right, y });

		if (x > stay.right && kinematics.velocity.x > 0)
			transf.setPosition({ stay.left, y });

		if (y <= stay.top && kinematics.velocity.y < 0)
			transf.setPosition({ x, stay.bottom });

		if (y > stay.bottom && kinematics.velocity.y > 0)
			transf.setPosition({ x, stay.top });
	}
}

// NPC Systems:
void ast::DestroyOnBounds(entt::registry& registry)
{
	auto view = registry.view<Transformable, DestoyOnBounds>();

	for (auto&& [entity, transformable, destroy] : view.each())
	{
		auto& transf = transformable.transformable;
		const auto& x = transf.getPosition().x;
		const auto& y = transf.getPosition().y;

		if (x < destroy.left)
			registry.destroy(entity);

		if (x > destroy.right)
			registry.destroy(entity);
	
		if (y < destroy.top)
			registry.destroy(entity);
	
		if (y > destroy.bottom)
			registry.destroy(entity);
	}
}

// Physics Systems:
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

	VelocityClampSystem(registry);

	RotateSystem(registry, dt);
	MoveSystem(registry, dt);
}
