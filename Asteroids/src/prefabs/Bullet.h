#pragma once
#include "pch.h"
#include "components/Components.h"
#include "application/Core.h"

namespace ast
{
	namespace
	{
		sf::VertexArray CreateShape(const BulletData& data)
		{
			unsigned int points = 100;
			double angleSkip = 2 * 3.14159 / points;

			sf::VertexArray shape;
			shape.setPrimitiveType(sf::PrimitiveType::TriangleFan);
			shape.resize(points);

			for (size_t i = 0; i < points - 1; ++i)
				shape[i].position = { cosf((float)(angleSkip * i)) * data.radius,
									  sinf((float)(angleSkip * i)) * data.radius };

			shape[points - 1].position = { data.radius, 0.f };

			for (size_t i = 0; i < shape.getVertexCount(); ++i)
				shape[i].color = data.color;

			return shape;
		}
	}

	inline entt::entity CreateBullet(entt::registry& registry, const sf::Vector2f& position, float rotation, const BulletData& data)
	{
		auto bullet = registry.create();	
		registry.emplace<Bullet>(bullet);


		// Shape:
		sf::VertexArray vertices = CreateShape(data);
		registry.emplace<Shape>(bullet, vertices);

		// Kinematics:
		sf::Vector2f direction;
		float angle = (rotation - 90.f) * 3.1415f / 180.f;
		direction = { cosf(angle), sinf(angle) };
		Kinematics kinematics(data.speed, -data.speed, data.speed, 0.f, 0.f, 0.f, direction);
		registry.emplace<Kinematics>(bullet, kinematics);

		
		// Transform:
		Transformable transformable;
		transformable.transformable.setPosition(position);
		transformable.transformable.setOrigin({ 0, 0 });
		registry.emplace<Transformable>(bullet, transformable);

		// Hitbox:
		sf::FloatRect bounds = vertices.getBounds();
		sf::Vector2f bulletSize = sf::Vector2f(data.radius * 2, data.radius * 2);
		auto hitboxOrigin = sf::Vector2f(bounds.width / 2, bounds.height / 2);
		registry.emplace<Hitbox>(bullet, bulletSize, hitboxOrigin);

		// Destoy:
		registry.emplace<DestoyOnBounds>(bullet, 
										-data.radius * 2, 
										 WorldWidth + data.radius * 2,
										-data.radius,
									    WorldHeight - data.radius * 2);

		return bullet;
	}

}