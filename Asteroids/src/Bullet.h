#pragma once
#include "pch.h"
#include "Components.h"

namespace ast
{
	entt::entity CreateBullet(entt::registry& registry, sf::Vector2f& position, float radius)
	{
		auto bullet = registry.create();
		
		registry.emplace<Kinematics>(bullet, 650.f, 650.f, 650.f);
		registry.emplace<Scale>(bullet);
		
		// Starting position:
		registry.emplace<Position>(bullet, position);

		// Shape:
		unsigned int points = 100;
		double angleSkip = 2 * 3.14159 / points;

		sf::VertexArray shape;
		shape.setPrimitiveType(sf::PrimitiveType::TriangleFan);
		shape.resize(points);

		for (size_t i = 0; i < points - 1; ++i)
			shape[i].position = { cosf((float)(angleSkip * i)) * radius, sinf((float)(angleSkip * i)) * radius };
		shape[points - 1].position = { radius, 0.f };

		// Color:
		for (size_t i = 0; i < shape.getVertexCount(); ++i)
			shape[i].color = sf::Color::White;

		registry.emplace<Shape>(bullet, shape);

		return bullet;
	}
}