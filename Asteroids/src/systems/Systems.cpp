#include "pch.h"
#include "utils/Vector.h"
#include "systems/Systems.h"
#include "prefabs/Bullet.h"

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
	auto view = registry.view<Position, Rotation, Shooting, Input>();

	for (auto&& [entity, position, rotation, shooting, input] : view.each())
	{
		input.canShoot = shooting.cooldown >= shooting.cooldownMax;

		if(!input.canShoot)
			shooting.cooldown += dt;

		if (input.canShoot && input.shooting)
		{
			shooting.cooldown = 0;

			CreateBullet(registry, position, rotation, regularBullet);
		}
	}
}

// NPC Systems:
void ast::DestroyOnBounds(entt::registry& registry)
{
	auto view = registry.view<DestoyOnBounds, Position>();

	for (auto&& [entity, destroy, position] : view.each())
	{
		if (position.position.x < destroy.left)
			registry.destroy(entity);

		if (position.position.x > destroy.right)
			registry.destroy(entity);

		if (position.position.y < destroy.top)
			registry.destroy(entity);

		if (position.position.y > destroy.bottom)
			registry.destroy(entity);
	}

	// Todo: move to a bound system:
	auto shipView = registry.view<StayInBounds, Position, Kinematics>();

	for (auto&& [entity, stay, position, kinematics] : shipView.each())
	{
		if (position.position.x <= stay.left && kinematics.velocity.x < 0)
			position.position = { stay.right, position.position.y };

		if (position.position.x > stay.right && kinematics.velocity.x > 0)
			position.position = { stay.left, position.position.y };

		if (position.position.y <= stay.top && kinematics.velocity.y < 0)
			position.position = { position.position.x, stay.bottom };

		if (position.position.y > stay.bottom && kinematics.velocity.y > 0)
			position.position = { position.position.x, stay.top };
	}
}

// Physics Systems:
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
