#pragma once
#include "pch.h"

namespace ast
{
	inline std::mt19937_64 rand;

	inline int RandomInt(int min, int max)
	{
		std::uniform_int_distribution<int> uniform_dist(min, max);
		return uniform_dist(rand);
	}

	inline float RandomFloat(float min, float max)
	{
		std::uniform_real_distribution<float> uniform_dist(min, max);
		return uniform_dist(rand);
	}

	inline bool RandomBool()
	{
		std::uniform_int_distribution<int> uniform_dist(0, 1);
		return (bool)uniform_dist(rand);
	}
}