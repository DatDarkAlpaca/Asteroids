#pragma once
#include <SFML/System/Vector2.hpp>
#include <cmath>

namespace game
{
	template<class T>
	static T magnitude(const sf::Vector2<T>& vec)
	{
		return sqrt(vec.x * vec.x + vec.y * vec.y);
	}

	template<class T>
	static T quadratic_magnitude(const sf::Vector2<T>& vec)
	{
		return vec.x * vec.x + vec.y * vec.y;
	}

	template<class T>
	static void normalize(sf::Vector2<T>& vec)
	{
		T m = magnitude(vec);

		if (m != 0)
		{
			vec.x /= m;
			vec.y /= m;
		}
	}

	template<class T>
	static T dotProduct(const sf::Vector2f& lhs, const sf::Vector2f& rhs)
	{
		return lhs.x * rhs.x + rhs.y * lhs.y;
	}

	template<class T>
	static T angle(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs)
	{
		return acos(dotProduct(lhs, rhs) / (magnitude(lhs) * magnitude(rhs)));
	}

	template<class T>
	static T angleDegrees(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs)
	{
		return angle(lhs, rhs) * 180 / 3.14159265;
	}
}