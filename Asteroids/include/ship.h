#pragma once
#include <SFML/Graphics.hpp>
#include "bullet.h"

class Ship : public sf::Transformable, public sf::Drawable
{
public:
    Ship();

private:
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

public:
    void update(float dt);

    void handleInput();

public:
    void setColor(const sf::Color& color);

    void setShootCooldown(float cooldown) { m_shootinCooldownMax = cooldown; }

    void setBrakingConstant(float braking) { m_breakingConstant = braking; }

private:
    void accelerate(float dt);

    void clampVelocity();

    void clampPosition();

    void brake(float dt, float multiplier = 1.f);

private:
    void createShip();

public:
    bool isShooting() const { return m_shooting; }

    bool canShoot() 
    { 
        if (m_shootingCooldown >= m_shootinCooldownMax)
        {
            m_shootingCooldown = 0;
            return true;
        }
        return false;
    }

    sf::Vector2f direction() const
    {
        float convert = (3.1415f / 180.f);
        return {cosf((getRotation() + 90) * convert), sinf((getRotation() + 90) * convert)};
    }

    sf::FloatRect getBounds() const
    {        
        return m_boundingBox;
    }

    sf::RectangleShape getHitbox() const { return m_hitbox; }

    void takeHit() { m_health -= 1; }

    bool isHitBoxEnabled() const { return m_hitBoxEnabled; }

    void disableHitbox() { m_hitBoxEnabled = false; }

    int getHealth() const { return m_health; }

    void setHealth(int health) { m_health = health; }

    void resetCooldown() { m_hitBoxEnabled = true; m_hitboxCooldown = 0; }

    void stop() { m_currentVelocity = { 0.f, 0.f }; }

private:
    sf::RectangleShape m_hitbox;
    sf::FloatRect m_boundingBox;
    sf::VertexArray m_vertices;

private:
    sf::Vector2f m_currentVelocity = { 0.f, 0.f };
    int m_rotationDirection = 0;

    int m_accelerationDirection = 0;
    float m_acceleration = 350.f;

    float m_angularSpeed = 200.f;

    float m_breakingConstant = 50.f;
    float m_maximumSpeed = 150.f;

private:
    float m_mainHeight = 20.f, m_extraHeight = 6.f, m_width = 10.f;

private:
    float m_shootingCooldown = 0.f, m_shootinCooldownMax = 0.4f;
    bool m_shooting = false;

private:
    int m_health = 3;
    bool m_hitBoxEnabled = true;
    float m_hitboxCooldown = 0.f, m_hitboxCooldownMax = 1.3f;
};