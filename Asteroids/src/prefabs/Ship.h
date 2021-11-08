#pragma once
#include "pch.h"
#include "components/Components.h"

namespace ast
{
	enum class ShipForm
	{
		Standard = 0,
		Starrogue,
		Lemon
	};

	namespace
	{
		struct ShapeOriginPack
		{
			ShapeOriginPack(sf::VertexArray& vertices, sf::Vector2f& origin)
				: vertices(vertices), origin(origin) { }

			sf::VertexArray vertices;
			sf::Vector2f origin, tip;
		};
	
		static ShapeOriginPack determineShipShape(const ShipForm& form, bool fill, const sf::Color& color)
		{
			// Determine the form:
			sf::VertexArray vertices;

			if (fill)
				vertices.setPrimitiveType(sf::PrimitiveType::TrianglesFan);
			else
				vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);

			// Determine the shape:
			switch (form)
			{
				case ShipForm::Standard: {
					vertices.resize(5);

					vertices[0].position = { 0, 0 };
					vertices[1].position = { 10, 26 };
					vertices[2].position = { 0, 20 };
					vertices[3].position = { -10, 26 };
					vertices[4].position = { 0, 0 };
					break;
				}

				case ShipForm::Starrogue: {
					vertices.resize(7);

					vertices[0].position = { 0, 0 };
					vertices[1].position = { 10, 10 };
					vertices[2].position = { 6, 20 };
					vertices[3].position = { 0, 15 };
					vertices[4].position = { -6, 20 };
					vertices[5].position = { -10, 10 };
					vertices[6].position = { 0, 0 };
					break;
				}

				case ShipForm::Lemon: {
					vertices.resize(7);

					vertices[0].position = { 0, 0 };
					vertices[1].position = { -10, 10 };
					vertices[2].position = { -10, 20 };
					vertices[3].position = { 0, 30 };
					vertices[4].position = { 10, 20 };
					vertices[5].position = { 10, 10 };
					vertices[6].position = { 0, 0 };

				}
			}

			// Determine the color:
			for (size_t i = 0; i < vertices.getVertexCount(); ++i)
				vertices[i].color = color;

			// Determine the origin:
			sf::Vector2f origin;
			switch (form)
			{
				case ShipForm::Standard: {
					origin = { 0, 13 };
					break;
				}

				case ShipForm::Starrogue: {
					origin = { 0, 10 };
					break;
				}

				case ShipForm::Lemon: {
					origin = { 0, 15 };
					break;
				}
			}

			return ShapeOriginPack(vertices, origin);
		}
	}

	inline entt::entity CreateShip(entt::registry& registry, const ShipForm& form = ShipForm::Standard, sf::Color color = sf::Color::White, bool fill = false)
	{
		auto ship = registry.create();

		registry.emplace<Kinematics>(ship, 0.f, -100.f, 100.f, -350.f, 150.f, 200.f);
		registry.emplace<Shooting>(ship, 0.3f);
		registry.emplace<Input>(ship);

		registry.emplace<Ship>(ship);

		// Shape:
		ShapeOriginPack shipPack = determineShipShape(form, fill, color);
		sf::FloatRect bounds = shipPack.vertices.getBounds();

		// Starting position:
		Transformable transformable;
		transformable.transformable.setPosition({ (float)WorldWidth / 2, (float)WorldHeight / 2 });
		transformable.transformable.setOrigin(shipPack.origin);
		registry.emplace<Transformable>(ship, transformable);

		// Origin:
		Shape shipShape(shipPack.vertices);
		
		// Shape Component:
		registry.emplace<Shape>(ship, shipShape);
		sf::Vector2f shipSize = sf::Vector2f(bounds.width, bounds.height);
		auto hitboxOrigin = sf::Vector2f(bounds.width / 2, shipPack.origin.y);
		registry.emplace<Hitbox>(ship, shipSize, hitboxOrigin);

		registry.emplace<StayInBounds>(ship, -bounds.width,  // left
											  WorldWidth + bounds.width, //right 
			                                 -bounds.height, // top
			                                  bounds.height + WorldHeight // bottom
			                           );

		return ship;
	}
}