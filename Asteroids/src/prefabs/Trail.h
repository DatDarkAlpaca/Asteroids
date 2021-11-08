#pragma once
#include "pch.h"
#include "components/components.h"

namespace ast
{
	inline entt::entity& CreateTrail(entt::registry& registry, float length, sf::Color color, sf::Vector2f shipPosition, sf::Vector2f origin)
	{
		auto trail = registry.create();

		registry.emplace<Trail>(trail, length, color, shipPosition, origin);

		return trail;
	}
}