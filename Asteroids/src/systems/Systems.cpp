#include "pch.h"
#include "utils/Vector.h"
#include "systems/Systems.h"
#include "prefabs/Bullet.h"
#include "prefabs/Asteroid.h"
#include "prefabs/Trail.h"
#include "../utils/Vector.h"


void ast::TrailSystem(entt::registry& registry, float dt)
{
	auto shipView = registry.view<Transformable, Hitbox, Input>();
	for (const auto& [shipEntity, transformable, hitbox, input] : shipView.each())
	{
		const auto& rotation = float(transformable.transformable.getRotation());
		const auto& rad = (rotation + 90.f) * 3.1415f / 180.f;
		const auto& width = hitbox.hitbox.getGlobalBounds().width;
		const auto& height = hitbox.hitbox.getGlobalBounds().height;

		// Bounds:
		if (input.accelerating)
		{
			int size = 10;
			CreateTrail(registry, size, sf::Color::White,
						transformable.transformable.getPosition() +
						sf::Vector2f
						(
							(width / 2) * cosf(rad), (height / 2) * sinf(rad)
						), { 5.f, height / 2.f - 15.f });
		}

		auto trailView = registry.view<Trail>();
		for (const auto& [trailEntity, trail] : trailView.each())
		{
			// Follow the player
			trail.trailShape.setRotation(rotation);
			trail.trailShape.move({ 0.1f * cosf(rotation), 0.1f * sinf(rotation) });

			//Color transition:
			trail.trailShape.TransitionTo(dt, 1.5f, 500.f);

			// Freeing:
			if (trail.trailShape.Finished())
				registry.destroy(trailEntity);
		}
	}
}

void ast::RenderSystem(entt::registry& registry, sf::RenderWindow& window)
{
	// Draw Entities:
	auto view = registry.view<Transformable, Shape>();
	for (auto&& [entity, transformable, shape] : view.each())
		window.draw(shape, transformable.transformable.getTransform());

	// Others Wireframe:
	/*auto wireView = registry.view<Hitbox>();
	for (auto&& [entity, hitbox] : wireView.each())
		window.draw(hitbox.hitbox);*/

	// Trail:
	auto trailView = registry.view<Trail>();
	for (auto&& [entity, trail] : trailView.each())
		window.draw(trail.trailShape.vertices(), trail.trailShape.getTransform());
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
