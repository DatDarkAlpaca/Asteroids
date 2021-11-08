#pragma once
#include "pch.h"
#include "RawData.h"

namespace ast
{
	// Identifiers:
	struct Bullet { char id; };

	struct Asteroid { char id; };

	struct Ship { char id; };

	// Transformable:
	struct Transformable
	{
		sf::Transformable transformable;
	};

	// Shape:
	// Todo: Refactor that shit. Use another component as the color holder.
	struct Shape
	{
		Shape(const sf::VertexArray& vertices, sf::Color idleColor = sf::Color::White, sf::Color hitColor = sf::Color::White)
			: shape(vertices), idleColor(idleColor), hitColor(hitColor) { }
		
		Shape() = default;

		operator sf::VertexArray& () { return shape; }

		void setOrigin(sf::Vector2f origin) { this->origin = origin; }

		void SetHit(bool hit)
		{
			if (hit)
			{
				for (size_t i = 0; i < shape.getVertexCount(); ++i)
					shape[i].color = hitColor;
			}
			else
			{
				for (size_t i = 0; i < shape.getVertexCount(); ++i)
					shape[i].color = idleColor;
			}
		}

		sf::Color idleColor, hitColor;
		sf::Vector2f origin;
		sf::VertexArray shape;
	};

	class TrailShape : public sf::Transformable
	{
	public:
		TrailShape(float length, sf::Color color, sf::Vector2f startPosition, sf::Vector2f origin)
		{
			m_Vertices.resize(4);
			m_Vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);

			m_Vertices[0].position = { length, 0 };
			m_Vertices[1].position = { cosf(120) * length, sinf(120) * length };
			m_Vertices[1].position = { cosf(240) * length, sinf(240) * length };
			m_Vertices[3].position = { length, 0 };

			for (size_t i = 0; i < m_Vertices.getVertexCount(); ++i)
				m_Vertices[i].color = color;

			setOrigin(origin);
			setPosition(startPosition);
		}

	public:
		void TransitionTo(float dt, float scaleMultplier = 1.f, float colorMultiplier = 1.f)
		{
			setScale(getScale() - sf::Vector2f(scaleMultplier * dt, scaleMultplier * dt));
			
			for (size_t i = 0; i < m_Vertices.getVertexCount(); ++i)
				m_Vertices[i].color -= sf::Color(0, 0, 0, dt * colorMultiplier);

			if (getScale().x <= 0.f || m_Vertices[0].color.a <= 0.f)
				m_Finished = true;
		}

		inline bool Finished() { return m_Finished; }

		const sf::VertexArray& vertices() const { return m_Vertices; }

	private:
		sf::VertexArray m_Vertices;
		bool m_Finished = false;
	};

	// Syncronize the trail color | Make the trail color custom?
	struct Trail
	{
		Trail(float length, sf::Color color, sf::Vector2f startPosition, sf::Vector2f origin)
			: trailShape(length, color, startPosition, origin) { }

		TrailShape trailShape;
	};

	// Input:
	struct Input
	{
		Input() : accelerating(false), braking(false), shooting(false), canShoot(true) { }

		short movementX = 0, movementY = 0, rotationDirection = 0;
		bool accelerating, braking, shooting, canShoot;
	};

	struct Shooting
	{
		Shooting(float cooldown) : cooldownMax(cooldown) { }

		Shooting() = default;

		float cooldown = 0.f, cooldownMax;
	};

	// Health:
	struct Health
	{
		Health(size_t maxHealth) : maxHealth(maxHealth) { health = maxHealth; }

		Health() = default;

		size_t health, maxHealth;
	};

	struct Invencibility
	{
		Invencibility(float duration)
			: duration(duration) { }

		bool currentlyInvencible = false;
		float currentTime = 0.f, duration = 0.f;
	};

	// Physics:
	struct Kinematics
	{
		Kinematics(float speed = 0.f, float minSpeed = 0.f, float maxSpeed = 0.f, float acceleration = 0.f, float drag = 0.f, float angularSpeed = 0.f, const sf::Vector2f& direction = sf::Vector2f(0, 0)) :
			speed(speed), acceleration(acceleration), drag(drag), angularSpeed(angularSpeed),
			maxSpeed(maxSpeed), minSpeed(minSpeed)
		{
			velocity = direction * speed;
		}

		Kinematics() = default;

		sf::Vector2f velocity = { 0.f, 0.f };
		float speed, acceleration, drag, angularSpeed, maxSpeed, minSpeed;
	};
	
	class HitboxShape : public sf::RectangleShape
	{
	public:
		HitboxShape() { create(); }

	public:
		void update(sf::Vector2f position, float rotation)
		{
			setPosition({ position.x, position.y });
			setRotation(rotation);
		}

		// Todo: disable on release:
		void create()
		{
			setOutlineColor(sf::Color::Green);
			setFillColor(sf::Color::Transparent);
			setOutlineThickness(1);
		}
	};

	struct Hitbox
	{
		Hitbox(const sf::Vector2f& rect, const sf::Vector2f& origin) : hitbox() 
		{ 
			hitbox.setSize(rect);
			hitbox.setOrigin(origin);
		}

		HitboxShape hitbox;

		bool enabled = true;
	};

	// Asteroid:
	struct MayFollow
	{
		bool follow = RandomBool();
	};

	struct AsteroidSize
	{
		AsteroidSize(int size) : size(size) { }

		AsteroidSize() = default;

		int size = 3;
	};

	// Border factors:
	struct DestoyOnBounds
	{
		DestoyOnBounds(float left, float right, float top, float bottom)
			: right(right), left(left), top(top), bottom(bottom) { }

		float right, left, top, bottom;
	};

	struct StayInBounds
	{
		StayInBounds(float left, float right, float top, float bottom)
			: left(left), right(right), top(top), bottom(bottom) { }

		float left, right, top, bottom;
	};
}
