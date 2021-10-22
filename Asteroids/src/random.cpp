#include "random.h"

namespace game
{
	std::random_device rd;
	std::mt19937_64 gen = std::mt19937_64(rd());
	std::uniform_int_distribution<> intDis;
}