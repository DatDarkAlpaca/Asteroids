#pragma once
#include <SFML/Graphics.hpp>
#include "settings.h"

class Bullet : public sf::CircleShape
{
public:
	Bullet(const sf::Vector2f& position, const sf::Vector2f& direction, float velocity = 650.f)
		: m_velocity(velocity), m_direction(direction)
	{
		setRadius(3.f);
		setFillColor(sf::Color::White);
		setPosition({ position.x - getRadius(), position.y - getRadius() });
	}

public:
	void update(float dt)
	{
		move(m_direction * dt * m_velocity);
	}

	bool isOutOfScreen()
	{
		if (getPosition().x <= -getRadius() * 2)
			return true;

		else if (getPosition().x >= game::windowWidth + getRadius() * 2)
			return true;

		if (getPosition().y <= 0)
			return true;

		else if (getPosition().y >= game::windowHeight - getRadius() * 2)
			return true;

		return false;
	}

private:
	sf::Vector2f m_direction;
	float m_velocity;
};