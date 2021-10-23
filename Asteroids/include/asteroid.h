#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>

#include "physics_component.h"
#include "vector_utils.h"
#include "settings.h"
#include "random.h"

class Asteroid : public sf::ConvexShape
{
public:
    Asteroid(bool followPlayer = false);
    Asteroid(unsigned size, const sf::Vector2f position,
        const sf::Vector2f velocity = { 1.f, 1.f }, bool followPlayer = false);

    ~Asteroid() = default;

public:
    void update(float dt);

    void updateHitbox();

    void followPlayer(const sf::Vector2f& playerPos, float dt);

    bool isOutOfBounds();

    void spawn();

private:
    void createAsteroid();

public:
    inline int getSize() const { return m_size; }

    inline sf::Vector2f getVelocity() const { return m_physics.velocity; }
    
    inline sf::RectangleShape getHitbox() const { return m_hitbox; }

private:
   enum class SpawnPosition { Left = 0, Right, Top, Bottom };
   PhysicsComponent m_physics;

private:   
    sf::Color m_normalColor = sf::Color::White,
              m_hitColor = sf::Color::Red;

private:
    sf::RectangleShape m_hitbox;

    int m_size = 3, m_radius;
    int m_boundOffset = 30;
    
private:
    bool m_followPlayer = false;
};
