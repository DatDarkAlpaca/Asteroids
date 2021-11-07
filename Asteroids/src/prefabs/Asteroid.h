#pragma once
#include "pch.h"
#include "utils/Random.h"
#include "components/Components.h"
#include "application/Core.h"

namespace ast
{

	namespace
	{
        enum class SpawnPosition { Left = 0, Right, Top, Bottom };

		inline void GenerateShape(AsteroidData& data, sf::VertexArray& vertices)
		{
			data.PrepareRadius();

			vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);
			vertices.resize(data.granularity);

			bool assignedFirstPoint = false;
			sf::Vector2f firstPoint;

			int radius = 0, i = 0;
			for (double angle = 0; angle < 2 * 3.1415; angle += 2 * 3.1415 / (data.granularity - 1))
			{
				int angleVaryPc = RandomInt(data.minVary, data.maxVary);

				double angleVaryRadians = (2 * 3.1415 / data.granularity) * double(angleVaryPc) / 100;
				double angleFinal = angle + angleVaryRadians - (3.1415 / data.granularity);

				radius = RandomInt(data.minRadius, data.maxRadius);
				if (radius < data.radius)
					data.radius = radius;

				float x = static_cast<float>(sin(angleFinal) * data.radius);
				float y = static_cast<float>(-cos(angleFinal) * data.radius);

				if (!assignedFirstPoint)
				{
					assignedFirstPoint = true;
					firstPoint = { x, y };
				}

				vertices[i].position = { x, y };
				i++;
			}

			vertices[data.granularity - 1].position = firstPoint;
		}

		inline Kinematics RandomKinematics(entt::registry& registry)
		{
			float speed = RandomFloat(10.f, 50.f);
			float angularSpeed = RandomFloat(-5, 5);

			Kinematics kinematics(speed, -50.f, 50.f, 0.f, 0.f, angularSpeed);

			return kinematics;
		}

		inline sf::Vector2f PrepareAsteroid(Kinematics& kinematics, int radius)
		{
			SpawnPosition spawnPosition = SpawnPosition(RandomInt(0, 3));

            int halfWidth = (int)(radius / 2);
            int halfHeight = (int)(radius / 2);
			int boundOffset = 30;

            int x = 0, y = 0;

			bool invertVelocity = 1;
            switch (spawnPosition)
            {
                case SpawnPosition::Left:
                {
                    x = -(boundOffset + halfWidth);

                    y = RandomInt(-(boundOffset + halfHeight), WorldHeight + boundOffset - halfHeight);

					if (invertVelocity)
						kinematics.velocity.y *= -1;
                    break;
                }

                case SpawnPosition::Right:
                {
                    x = WorldWidth + halfWidth + boundOffset;

                    y = RandomInt(-(boundOffset + halfHeight), WorldHeight + boundOffset - halfHeight);

                    if (invertVelocity)
                        kinematics.velocity.y *= -1;

					kinematics.velocity.x *= -1;
                    break;
                }

                case SpawnPosition::Top:
                {
                    x = RandomInt(halfWidth, WorldWidth - halfWidth);
                    y = -(halfHeight + boundOffset);

                    if (invertVelocity)
						kinematics.velocity.x *= -1;

                    break;
                }

                case SpawnPosition::Bottom:
                {
					x = RandomInt(halfWidth, WorldWidth - halfWidth);
                    y = WorldHeight + boundOffset + halfHeight;

                    if (invertVelocity)
						kinematics.velocity.x *= -1;

					kinematics.velocity.y *= -1;
                    break;
                }

				default:
					break;
            }

			return sf::Vector2f((float)x, (float)y);
		}
	}

	inline entt::entity CreateAsteroid(entt::registry& registry, AsteroidData& data, int size = 3)
	{
		// Size:
		data.size = size;

		// Kinematics:
		auto kinematics = RandomKinematics(registry);
	
		// Shape:
		sf::VertexArray vertices;
		GenerateShape(data, vertices);
		Shape asteroidShape(vertices);
	
		// Bounds:
		sf::FloatRect bounds = vertices.getBounds();

		// Preparing and position:
		auto position = PrepareAsteroid(kinematics, data.radius);

		auto asteroid = registry.create();
		registry.emplace<Scale>(asteroid);
		registry.emplace<Rotation>(asteroid);
		registry.emplace<Shape>(asteroid, asteroidShape);
		registry.emplace<Hitbox>(asteroid, bounds);
		registry.emplace<Position>(asteroid, position);
		registry.emplace<AsteroidData>(asteroid, data);
		registry.emplace<Kinematics>(asteroid, kinematics);

		// Destroying:
		registry.emplace<DestoyOnBounds>(asteroid, -(bounds.width + 30),
									     WorldWidth + bounds.width + 30,
			                            -(bounds.height + 30),
										 WorldHeight + bounds.height + 30);

		return asteroid;
	}
}