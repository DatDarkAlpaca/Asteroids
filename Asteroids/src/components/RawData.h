#pragma once
#include "pch.h"
#include "utils/Random.h"

namespace ast
{

	// Asteroid Data:
	struct AsteroidData
	{
		AsteroidData(int minVary, int maxVary, int granularity, bool followPlayer)
			: minVary(minVary), maxVary(maxVary), granularity(granularity), followPlayer(followPlayer)
		{
			PrepareRadius();
		}

		AsteroidData() { PrepareRadius(); }

		inline void PrepareRadius() { minRadius = 10 * size; maxRadius = 15 * size; }

		int radius = 2147483647, minRadius, maxRadius, size = 1;
		int minVary, maxVary, granularity;
		bool followPlayer;
	};

	inline AsteroidData regularAsteroid(0, 100, 10, RandomBool());
	inline AsteroidData weirdAsteroid(0, 70, 5, RandomBool());

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