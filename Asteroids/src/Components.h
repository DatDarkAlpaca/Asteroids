#pragma once
#include "pch.h"

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
		Kinematics(float speed = 0.f, float maxSpeed = 0.f, float minSpeed = 0.f, 
			       float acceleration = 0.f, float drag = 0.f, float angularSpeed = 0.f)
			: speed(speed), acceleration(acceleration), drag(drag), angularSpeed(angularSpeed), maxSpeed(maxSpeed),
			minSpeed(minSpeed) { }

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
}
