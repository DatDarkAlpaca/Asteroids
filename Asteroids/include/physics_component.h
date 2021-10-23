#pragma once
#include <SFML/System/Vector2.hpp>

class PhysicsComponent
{
public:
	PhysicsComponent(sf::Vector2f velocity, sf::Vector2f acceleration, float angularSpeed, float drag)
		: velocity(velocity), acceleration(acceleration), angularSpeed(angularSpeed), drag(drag) { }
	PhysicsComponent() = default;

public:
	void clampVelocity()
	{
		if (velocity.x >= maxSpeed)
			velocity.x = maxSpeed;

		else if (velocity.x <= minSpeed)
			velocity.x = minSpeed;

		if (velocity.y >= maxSpeed)
			velocity.y = maxSpeed;

		else if (velocity.y <= minSpeed)
			velocity.y = minSpeed;
	}
	
	void accelerate(float dt, float rotation)
	{
		velocity.x += acceleration.x * cosf((rotation + 90.f) * 3.1415f / 180.f) * dt;
		velocity.y += acceleration.y * sinf((rotation + 90.f) * 3.1415f / 180.f) * dt;
	}

	void brake(float dt, float multiplier = 1.f)
	{
		if (velocity.y >= 0)
			velocity += { 0, -drag * dt * multiplier };
		else
			velocity += { 0, drag * dt* multiplier };

		if (velocity.x >= 0)
			velocity += { -drag * dt * multiplier, 0 };
		else
			velocity += { drag* dt * multiplier, 0};
	}

	inline void stop() { velocity = { 0.f, 0.f }; }

public:
	sf::Vector2f velocity = { 0.f, 0.f };
	sf::Vector2f acceleration = { 350.f, 350.f };

	float minSpeed = 0.f, maxSpeed = 1000.f;
	float angularSpeed = 0.f;

	float drag = 0.f;
};