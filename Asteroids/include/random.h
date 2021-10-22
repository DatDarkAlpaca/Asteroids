#pragma once
#include <random>

namespace game
{
	using param_int_t = std::uniform_int_distribution<>::param_type;
	
	extern std::random_device rd;
	extern std::mt19937_64 gen;
	extern std::uniform_int_distribution<> intDis;
}