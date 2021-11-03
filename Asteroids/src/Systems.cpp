#include "pch.h"
#include "vector_utils.h"
#include "Systems.h"

void ast::RenderSystem(entt::registry& registry, sf::RenderWindow& window)
{
	auto view = registry.view<Position, Rotation, Scale, Shape>();

	for (auto&& [entity, position, rotation, scale, shape] : view.each())
	{
		sf::Transform transform = ApplyTransform(position, shape.origin, scale, rotation);
		sf::RenderStates states(transform);

		window.draw(shape, states);
	}
}

void ast::InputSystem(entt::registry& registry)
{
	auto view = registry.view<Input>();

	for (auto&& [entity, input] : view.each())
	{
		input.accelerating = sf::Keyboard::isKeyPressed(sf::Keyboard::W);

		input.braking = sf::Keyboard::isKeyPressed(sf::Keyboard::S);

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			input.rotationDirection = 1;
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
			input.rotationDirection = -1;
		else
			input.rotationDirection = 0;

		input.shooting = sf::Keyboard::isKeyPressed(sf::Keyboard::Space);
	}
}

void ast::ShootingSystem(entt::registry& registry, float dt)
{
	auto view = registry.view<Shooting, Input>();

	for (auto&& [entity, shooting, input] : view.each())
	{
		input.canShoot = shooting.cooldown >= shooting.cooldownMax;

		if (!input.canShoot)
			shooting.cooldown += dt;
		else
		{
			shooting.cooldown = 0.f;
			input.canShoot = true;
		}
	}
}

void ast::AccelerateSystem(entt::registry& registry, float dt)
{
	auto view = registry.view<Kinematics, Rotation>();

	for (auto&& [entity, kinematics, rotation] : view.each())
	{
		kinematics.velocity.x += kinematics.acceleration * cosf((rotation + 90.f) * 3.1415f / 180.f) * dt;
		kinematics.velocity.y += kinematics.acceleration * sinf((rotation + 90.f) * 3.1415f / 180.f) * dt;
	}
}

void ast::BrakeSystem(entt::registry& registry, float dt, float multiplier)
{
	auto view = registry.view<Kinematics, Rotation>();

	for (auto&& [entity, kinematics, rotation] : view.each())
	{
		auto& velocity = kinematics.velocity;
		const auto drag = kinematics.drag;

		if (velocity.y >= 0)
			velocity += { 0, -drag * dt * multiplier };
		else
			velocity += { 0, drag* dt* multiplier };

		if (velocity.x >= 0)
			velocity += { -drag * dt * multiplier, 0 };
		else
			velocity += { drag* dt* multiplier, 0};
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

		else if (velocity.x <= minSpeed)
			velocity.x = minSpeed;

		if (velocity.y >= maxSpeed)
			velocity.y = maxSpeed;

		else if (velocity.y <= minSpeed)
			velocity.y = minSpeed;
	}
}

void ast::MoveSystem(entt::registry& registry, float dt)
{
	auto view = registry.view<Position, Kinematics, Shape>();

	for (auto&& [entity, position, kinematics, shape] : view.each())
	{
		position.position = { position.position.x + kinematics.velocity.x * dt,
					          position.position.y + kinematics.velocity.y * dt };
	}
}

void ast::RotateSystem(entt::registry& registry, float dt)
{
	auto shipView = registry.view<Rotation, Kinematics, Input>();

	// Ship rotation:
	for (auto&& [entity, rotation, kinematics, input] : shipView.each())
		rotation.rotation += kinematics.angularSpeed * dt * input.rotationDirection;
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
