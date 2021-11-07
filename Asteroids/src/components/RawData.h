#pragma once
#include "pch.h"
#include "utils/Random.h"

namespace ast
{
	// Bullet Data:
	struct BulletData
	{
		BulletData(float speed, float radius, const sf::Color& color)
			: speed(speed), radius(radius), color(color) { }

		BulletData() = default;

		float speed, radius;
		sf::Color color;
	};

	inline BulletData regularBullet(350.f, 3.f, sf::Color(255, 255, 255, 255));
	inline BulletData fastBullet(550.f, 3.f, sf::Color(255, 100, 255, 255));

}