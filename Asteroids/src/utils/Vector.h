#pragma once
#include "pch.h"

namespace ast
{
	static float magnitude(const sf::Vector2f& vec)
	{
		return sqrtf(vec.x * vec.x + vec.y * vec.y);
	}

	static void normalize(sf::Vector2f& vec)
	{
		float m = magnitude(vec);

		if (m == 0.f || m == 1.f)
			return;

		vec.x /= m;
		vec.y /= m;
	}

	static sf::Vector2f normalized(const sf::Vector2f& vec)
	{
		sf::Vector2f temp = { 0, 0 };

		float m = magnitude(vec);
		if (m == 0.f)
			return temp;

		temp.x = vec.x / m;
		temp.y = vec.y / m;

		return temp;
	}

	static sf::Transform ApplyTransform(const sf::Vector2f& position, const sf::Vector2f& origin, const sf::Vector2f& scale, float rotation)
	{
		float angle = -rotation * 3.141592654f / 180.f;
		float cosine = float(std::cos(angle));
		float sine = float(std::sin(angle));

		float sxc = scale.x * cosine;
		float syc = scale.y * cosine;
		float sxs = scale.x * sine;
		float sys = scale.y * sine;

		float tx = -origin.x * sxc - origin.y * sys + position.x;
		float ty = origin.x * sxs - origin.y * syc + position.y;

		sf::Transform transform = sf::Transform(sxc, sys, tx,
			-sxs, syc, ty,
			0.f, 0.f, 1.f);

		return transform;
	}
}
