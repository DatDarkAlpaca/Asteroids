#pragma once
#include "pch.h"
#include "RawData.h"

namespace ast
{
	// Identifiers:
	struct Bullet { char id; };

	struct Asteroid { char id; };

	struct Ship { char id; };

	// Transformable:
	struct Transformable
	{
		sf::Transformable transformable;
	};

	// Shape:
	struct Shape
	{
		Shape(const sf::VertexArray& vertices) : shape(vertices) { }
		
		Shape() = default;

		operator sf::VertexArray& () { return shape; }

		void setOrigin(sf::Vector2f origin) { this->origin = origin; }

		sf::Vector2f origin;
		sf::VertexArray shape;
	};

	// Input:
	struct Input
	{
		Input() : accelerating(false), braking(false), shooting(false), canShoot(true) { }

		short movementX = 0, movementY = 0, rotationDirection = 0;
		bool accelerating, braking, shooting, canShoot;
	};

	struct Shooting
	{
		Shooting(float cooldown) : cooldownMax(cooldown) { }

		Shooting() = default;

		float cooldown = 0.f, cooldownMax;
	};

	// Health:
	struct Health
	{
		Health(size_t maxHealth) : maxHealth(maxHealth) { health = maxHealth; }

		Health() = default;

		size_t health, maxHealth;
	};

	// Physics:
	struct Kinematics
	{
		Kinematics(float speed = 0.f, float minSpeed = 0.f, float maxSpeed = 0.f, float acceleration = 0.f, float drag = 0.f, float angularSpeed = 0.f, const sf::Vector2f& direction = sf::Vector2f(0, 0)) :
			speed(speed), acceleration(acceleration), drag(drag), angularSpeed(angularSpeed),
			maxSpeed(maxSpeed), minSpeed(minSpeed)
		{
			velocity = direction * speed;
		}

		Kinematics() = default;

		sf::Vector2f velocity = { 0.f, 0.f };
		float speed, acceleration, drag, angularSpeed, maxSpeed, minSpeed;
	};
	
	class HitboxShape : public sf::RectangleShape
	{
	public:
		HitboxShape() { create(); }

	public:
		void update(sf::Vector2f position, float rotation)
		{
			setPosition({ position.x, position.y });
			setRotation(rotation);
		}

		// Todo: disable on release:
		void create()
		{
			setOutlineColor(sf::Color::Green);
			setFillColor(sf::Color::Transparent);
			setOutlineThickness(1);
		}
	};

	struct Hitbox
	{
		Hitbox(const sf::Vector2f& rect, const sf::Vector2f& origin) : hitbox() 
		{ 
			hitbox.setSize(rect);
			hitbox.setOrigin(origin);
		}

		HitboxShape hitbox;

		bool enabled = true;
	};

	// Asteroid:
	struct MayFollow
	{
		bool follow = RandomBool();
	};

	struct AsteroidSize
	{
		AsteroidSize(int size) : size(size) { }

		AsteroidSize() = default;

		int size = 3;
	};

	// Border factors:
	struct DestoyOnBounds
	{
		DestoyOnBounds(float left, float right, float top, float bottom)
			: right(right), left(left), top(top), bottom(bottom) { }

		float right, left, top, bottom;
	};

	struct StayInBounds
	{
		StayInBounds(float left, float right, float top, float bottom)
			: left(left), right(right), top(top), bottom(bottom) { }

		float left, right, top, bottom;
	};
}
