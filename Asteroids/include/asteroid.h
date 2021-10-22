#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "settings.h"
#include "random.h"
#include "vector_utils.h"

class Asteroid : public sf::ConvexShape
{
public:
    Asteroid(bool followPlayer = false) : m_followPlayer(followPlayer) { createAsteroid(); spawn(); }
    Asteroid(unsigned size, const sf::Vector2f position, 
             const sf::Vector2f velocity = {1.f, 1.f}, bool followPlayer=false) : m_size(size), m_followPlayer(followPlayer)
    {
        using namespace game;
        createAsteroid();

        setPosition(position);
        m_velocity = velocity;

        m_angularSpeed = (float)(intDis(gen, param_int_t(-4, 3)) + 1);
    }

    ~Asteroid() { }

public:
    void update(float dt)
    {
        // Hitbox:
        m_hitbox.setPosition({ getPosition().x, getPosition().y });

        float size = std::min(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
        m_hitbox.setSize({
            size,
            size
        });

        m_hitbox.setOrigin({
            getGlobalBounds().width / 4,
            getGlobalBounds().height / 4,
        });

        m_hitbox.setRotation(getRotation());

        move(m_velocity * dt);
        rotate(m_angularSpeed * dt);
    }

    void followPlayer(const sf::Vector2f& playerPos, float dt)
    {
        if (!m_followPlayer)
            return;

        sf::Vector2f dir = playerPos - getPosition();
        game::normalize(dir);

        m_velocity.x = dir.x * m_speed;
        m_velocity.y = dir.y * m_speed;
    }

    bool isOutOfBounds()
    {
        using namespace game;

        if (getPosition().x < -(getGlobalBounds().width + m_boundOffset))
            return true;

        if (getPosition().x > windowWidth + getGlobalBounds().width + m_boundOffset)
            return true;

        if (getPosition().y < -(getGlobalBounds().height + m_boundOffset))
            return true;

        if (getPosition().y > windowHeight + getGlobalBounds().height + m_boundOffset)
            return true;

        return false;
    }

    void spawn()
    {
        using namespace game;

        SpawnPosition spawnPosition = SpawnPosition(intDis(gen, param_int_t(0, 3)));

        int halfWidth = (int)(getGlobalBounds().width / 2);
        int halfHeight = (int)(getGlobalBounds().height / 2);

        int x = 0, y = 0;

        bool invertVelocity = intDis(gen, param_int_t(0, 1)) == 0 ? 1 : 0;
        switch (spawnPosition)
        {
        case SpawnPosition::Left:
        {
            x = -(m_boundOffset + halfWidth);

            param_int_t(0, 1);
            y = intDis(gen, param_int_t(
                -(m_boundOffset + halfHeight),
                windowHeight + m_boundOffset - halfHeight)
            );

            if (invertVelocity)
                m_velocity.y *= -1;

            break;
        }

        case SpawnPosition::Right:
        {
            x = windowWidth + halfWidth + m_boundOffset;

            y = intDis(gen, param_int_t(
                -(m_boundOffset + halfHeight),
                windowHeight + m_boundOffset - halfHeight)
            );

            if(invertVelocity)
                m_velocity.y *= -1;
            
            m_velocity.x *= -1;
            break;
        }

        case SpawnPosition::Top:
        {
            x = intDis(gen, param_int_t
            (
                halfWidth,
                windowWidth - halfWidth)
            );

            y = -(halfHeight + m_boundOffset);

            if (invertVelocity)
                m_velocity.x *= -1;

            break;
        }

        case SpawnPosition::Bottom:
        {
            x = intDis(gen, param_int_t
            (
                halfWidth,
                windowWidth - halfWidth)
            );

            y = windowHeight + m_boundOffset + halfHeight;

            if(invertVelocity)
                m_velocity.x *= -1;

            m_velocity.y *= -1;
            break;
        }

        default:
            break;
        }

        setPosition({ float(x), float(y) });
    }

    void invertVelocity()
    {
        m_velocity.x *= -1;
        m_velocity.y *= -1;
    }

private:
	void createAsteroid()
	{
        using namespace game;

        int minRadius = 10 * m_size, maxRadius = 15 * m_size;
        int granularity = 10;
        int minVary = 0, maxVary = 100;

        std::vector<sf::Vector2f> points;

        // Generate the points
        for (double angle = 0; angle < 2 * 3.1415; angle += 2 * 3.1415 / granularity)
        {
            int angleVaryPc = intDis(gen, param_int_t(minVary, maxVary));

            double angleVaryRadians = (2 * 3.1415 / granularity) * static_cast<double>(angleVaryPc) / 100;
            double angleFinal = angle + angleVaryRadians - (3.1415 / granularity);
            
            m_radius = intDis(gen, param_int_t(minRadius, maxRadius));
            
            float x = static_cast<float>(sin(angleFinal) * m_radius);
            float y = static_cast<float>(-cos(angleFinal) * m_radius);
            
            points.push_back({x, y});
        }

        // Create the shape:
        setFillColor(sf::Color::Transparent);
        setOutlineColor(m_normalColor);
        setOutlineThickness(1.f);

        setPointCount(points.size());
        for (size_t i = 0; i < points.size(); ++i)
            setPoint(i, points[i]);

        // Random velocity:
        m_angularSpeed = (float)(intDis(gen, param_int_t(-4, 3)) + 1);
        m_speed = intDis(gen, param_int_t(m_minSpeed, m_maxSpeed));

        m_velocity.x = (float)m_speed;
        m_velocity.y = (float)m_speed;

        // Hitbox color:
        m_hitbox.setOutlineColor(sf::Color::Green);
        m_hitbox.setFillColor(sf::Color::Transparent);
        m_hitbox.setOutlineThickness(1);
    }

public:
    int getSize() const { return m_size; }

    sf::Vector2f getVelocity() const { return m_velocity; }
    
    sf::RectangleShape getHitbox() const { return m_hitbox; }

private:
   enum class SpawnPosition { Left = 0, Right, Top, Bottom };

private:
    float m_angularSpeed;
    sf::Vector2f m_velocity;
    
    sf::Color m_normalColor = sf::Color::White,
              m_hitColor = sf::Color::Red;

    sf::RectangleShape m_hitbox;

    int m_size = 3, m_radius;
    int m_minSpeed = 10, m_maxSpeed = 70, m_speed = 0;
    int m_boundOffset = 30;
    
    bool m_followPlayer = false;
};
