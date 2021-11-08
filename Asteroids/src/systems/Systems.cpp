#include "pch.h"
#include "utils/Vector.h"
#include "systems/Systems.h"
#include "prefabs/Bullet.h"
#include "prefabs/Asteroid.h"
#include "../utils/Vector.h"

void ast::RenderSystem(entt::registry& registry, sf::RenderWindow& window)
{
	// Draw Entities:
	auto view = registry.view<Transformable, Shape>();
	for (auto&& [entity, transformable, shape] : view.each())
		window.draw(shape, transformable.transformable.getTransform());

	// Others Wireframe:
	auto wireView = registry.view<Hitbox>();
	for (auto&& [entity, hitbox] : wireView.each())
		window.draw(hitbox.hitbox);
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
		{
			registry.destroy(entity);
			break;
		}

		else if (x > destroy.right)
		{
			registry.destroy(entity);
			break;
		}
	
		if (y < destroy.top)
		{
			registry.destroy(entity);
			break;
		}
	
		else if (y > destroy.bottom)
		{
			registry.destroy(entity);
			break;
		}
	}
}

// TODO: Implement this:
void ast::FollowPlayerSystem(entt::registry& registry)
{
	/*sf::Vector2f shipPos;
	auto playerView = registry.view<Transformable, Ship>();
	for (auto&& [entity, transformable, shipID] : playerView.each())
		shipPos = transformable.transformable.getPosition();

	auto view = registry.view<Transformable, Kinematics, MayFollow>();

	for (auto&& [entity, transformabe, kinematics, follow] : view.each())
	{
		auto dir = normalized(shipPos - transformabe.transformable.getPosition());
		kinematics.velocity.x = dir.x * kinematics.speed;
		kinematics.velocity.y = dir.y * kinematics.speed;
	}*/
}

void ast::HitboxSystem(entt::registry& registry)
{
	auto view = registry.view<Transformable, Hitbox>();

	for (auto&& [entity, transformable, hitbox] : view.each())
	{
		auto transf = transformable.transformable;
		hitbox.hitbox.setPosition(transf.getPosition());
		hitbox.hitbox.setRotation(transf.getRotation());
	}
}

// Collision System:
void ast::ShipCollisionSystem(entt::registry& registry, float dt)
{
	auto shipView = registry.view<Transformable, Hitbox, Health, Invencibility, Ship>();
	auto asteroidView = registry.view<Transformable, Kinematics, Hitbox, AsteroidSize>();

	for (auto&& [entityShip, shipTransform, shipHitbox, health, invencibility, shipID] : shipView.each())
	{
		for (auto&& [entityAsteroid, asteroidTransform, kinematics, asteroidHitbox, asteroidSize] : asteroidView.each())
		{
			if (shipHitbox.hitbox.getGlobalBounds().intersects(asteroidHitbox.hitbox.getGlobalBounds()))
			{				
				// Invencibility:
				// Todo: find a decent place to handle these mechanics.
				if (invencibility.currentTime < invencibility.duration)
				{
					invencibility.currentTime += dt;
					invencibility.currentlyInvencible = true;
				}
				else
				{
					invencibility.currentTime = 0.f;
					invencibility.currentlyInvencible = false;
				}

				// Invencibility:
				if (invencibility.currentlyInvencible)
					return;

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

bool ast::GameoverHealthSystem(entt::registry& registry)
{
	// Additional variable for clarity's sake:
	bool gameover = false;

	auto view = registry.view<Health>();
	for (const auto& [entity, health] : view.each())
	{
		if (health.health == 0)
			gameover = true;
	}

	return gameover;
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

	VelocityClampSystem(registry);

	CollisionSystem(registry, dt);
		
	FollowPlayerSystem(registry);

	RotateSystem(registry, dt);
	
	MoveSystem(registry, dt);

	HitboxSystem(registry);
}
