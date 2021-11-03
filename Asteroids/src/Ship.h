#pragma once
#include "pch.h"
#include "Components.h"

namespace ast
{
	enum class ShipForm
	{
		Standard = 0,
		Starrogue
	};

	namespace
	{
		struct ShapeOriginPack
		{
			ShapeOriginPack(sf::VertexArray& vertices, sf::Vector2f& origin)
				: vertices(vertices), origin(origin) { }

			sf::VertexArray vertices;
			sf::Vector2f origin;
		};
	
		static ShapeOriginPack determineShipShape(ShipForm form, bool fill)
		{
			// Determine the form:
			sf::VertexArray vertices;

			if (fill)
				vertices.setPrimitiveType(sf::PrimitiveType::TrianglesFan);
			else
				vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);

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
			}

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
			}

			return ShapeOriginPack(vertices, origin);
		}
	}

	entt::entity CreateShip(entt::registry& registry, ShipForm form = ShipForm::Standard, sf::Color color = sf::Color::White, bool fill = false)
	{
		auto player = registry.create();

		registry.emplace<Kinematics>(player, 0.f, 150.f, -150.f, -350.f, 150.f, 200.f);
		registry.emplace<Shooting>(player, 0.2f);
		registry.emplace<Rotation>(player);
		registry.emplace<Input>(player);
		registry.emplace<Scale>(player);

		// Starting position:
		sf::Vector2f position = { 200.f, 200.f };
		registry.emplace<Position>(player, position);

		// Shape:
		ShapeOriginPack shipPack = determineShipShape(form, fill);
		
		// Color:
		for (size_t i = 0; i < shipPack.vertices.getVertexCount(); ++i)
			shipPack.vertices[i].color = color;

		// Origin:
		Shape shipShape(shipPack.vertices);
		shipShape.setOrigin(shipPack.origin);

		// Shape Component:
		registry.emplace<Shape>(player, shipShape);

		return player;
	}
}