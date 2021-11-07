#pragma once
#include "pch.h"
#include "utils/Random.h"
#include "components/Components.h"
#include "application/Core.h"

namespace ast
{

	enum class SpawnPosition { Left = 0, Right, Top, Bottom };

	struct AsteroidShapeData
	{
		AsteroidShapeData(int minVary, int maxVary, int granularity)
			: minVary(minVary), maxVary(maxVary), granularity(granularity){ }

		AsteroidShapeData() = default;

		int minVary, maxVary, granularity;
	} inline const regularAsteroid(0, 100, 10);

	class AsteroidFactory
	{
	public:
		static void SpawnAsteroid(entt::registry& registry, const AsteroidShapeData& data, int size)
		{
			// Asteroid ID:
			auto asteroid = registry.create();
			registry.emplace<Asteroid>(asteroid);

			// Kinematics:
			auto kinematics = RandomKinematics();

			// May Follow:
			registry.emplace<MayFollow>(asteroid);

			// Shape:
			sf::VertexArray vertices = GenerateShape(data, size);
			Shape asteroidShape(vertices);
			registry.emplace<Shape>(asteroid, asteroidShape);

			sf::FloatRect bounds = vertices.getBounds();
			float radius = std::min(vertices.getBounds().width / 2, vertices.getBounds().height / 2);
			// registry.emplace<Hitbox>(asteroid, bounds);

			Transformable transformable;
			transformable.transformable.setPosition(AsteroidSpawnPosition(kinematics, radius));
			registry.emplace<Transformable>(asteroid, transformable);

			registry.emplace<Kinematics>(asteroid, kinematics);

			// Destroying:
			/*registry.emplace<DestoyOnBounds>(asteroid, -(bounds.width + 30),
				WorldWidth + bounds.width + 30,
				-(bounds.height + 30),
				WorldHeight + bounds.height + 30);*/
		}

	private:
		// we need the radius somehow:
		static sf::VertexArray GenerateShape(const AsteroidShapeData& data, int size)
		{
			sf::VertexArray vertices;

			int minRadius = 10 * size;
			int maxRadius = 15 * size;

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

				int radius = RandomInt(minRadius, maxRadius);

				float x = static_cast<float>(sin(angleFinal) * radius);
				float y = static_cast<float>(-cos(angleFinal) * radius);

				if (!assignedFirstPoint)
				{
					assignedFirstPoint = true;
					firstPoint = { x, y };
				}

				vertices[i].position = { x, y };
				i++;
			}

			vertices[data.granularity - 1].position = firstPoint;

			return vertices;
		}

		static Kinematics RandomKinematics()
		{
			float speed = RandomFloat(40.f, 150.f);
			float angularSpeed = RandomFloat(-5, 5);

			Kinematics kinematics(speed, -50.f, 50.f, 0.f, 0.f, angularSpeed, { 1, 1 });

			return kinematics;
		}
		
		static sf::Vector2f AsteroidSpawnPosition(Kinematics& kinematics, float radius)
		{
			SpawnPosition spawnPosition = SpawnPosition(RandomInt(0, 3));

			int halfWidth = (int)(radius / 2);
			int halfHeight = (int)(radius / 2);
			int boundOffset = 30;

			int x = 0, y = 0;

			bool invertVelocity = RandomBool();
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

	private:
		int radius;
	};


	/*inline entt::entity CreateChildAsteroid(entt::registry& registry, AsteroidData data, int size, sf::Vector2f position, sf::Vector2f& direction)
	{
		auto asteroid = registry.create();
		registry.emplace<Asteroid>(asteroid);

		std::cout << size << '\n';

		// Size:
		data.size = size;

		// Kinematics:
		auto speed = RandomFloat(40.f, 150.f);
		float angularSpeed = RandomFloat(-5, 5);

		Kinematics kinematics(speed, -50.f, 50.f, 0.f, 0.f, angularSpeed, direction);

		// Shape:
		sf::VertexArray vertices;
		GenerateShape(data, vertices);
		Shape asteroidShape(vertices);
		registry.emplace<Shape>(asteroid, asteroidShape);

		// Bounds:
		sf::FloatRect bounds = vertices.getBounds();
		registry.emplace<Hitbox>(asteroid, bounds);

		// Transfom:
		Transformable transformable;
		transformable.transformable.setPosition(position);
		registry.emplace<Transformable>(asteroid, transformable);

		// Todo: most of the data is only needed to create the asteroid:
		registry.emplace<AsteroidData>(asteroid, data);
		registry.emplace<Kinematics>(asteroid, kinematics);

		// Destroying:
		registry.emplace<DestoyOnBounds>(asteroid, -(bounds.width + 30),
			WorldWidth + bounds.width + 30,
			-(bounds.height + 30),
			WorldHeight + bounds.height + 30);

		return asteroid;
	}*/

}