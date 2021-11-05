#pragma once
#include "pch.h"
#include "RawData.h"

namespace ast
{
	struct Position
	{
		Position(sf::Vector2f& position) : position(position) { }
		
		Position() = default;

		operator sf::Vector2f& () { return position; }

		sf::Vector2f position;
	};

	struct Rotation
	{
		Rotation(float rotation) : rotation(rotation) { }
		
		Rotation() = default;

		operator float () { return rotation; }
		float rotation = 0.f;
	};

	struct Scale
	{
		Scale(sf::Vector2f& scale) : scale(scale) { }
		
		Scale() = default;

		operator sf::Vector2f() { return scale; }
		sf::Vector2f scale = { 1.f, 1.f };
	};

	struct Shape
	{
		Shape(sf::VertexArray& vertices) : shape(vertices) { }
		
		Shape() = default;

		operator sf::VertexArray& () { return shape; }

		void setOrigin(sf::Vector2f origin) { this->origin = origin; }

		sf::Vector2f origin;
		sf::VertexArray shape;
	};

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

	struct Hitbox
	{
		Hitbox(sf::FloatRect& localBounds)
			: localBounds(localBounds) { }

		Hitbox() = default;

		inline sf::FloatRect globalBounds(const sf::Transform& transform) 
		{ 
			return transform.transformRect(localBounds); 
		}
		inline float size(const sf::Transform& transform) 
		{
			return std::min(globalBounds(transform).width / 2, globalBounds(transform).height / 2);
		}

		sf::FloatRect localBounds;
	};

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
