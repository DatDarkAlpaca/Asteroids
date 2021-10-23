#pragma once
#include <SFML/Graphics.hpp>

#include "physics_component.h"
#include "bullet.h"

class Ship : public sf::ConvexShape
{
public:
    Ship();

public:
    void update(float dt);

    void handleInput();

private:
    void updateHitbox();

public:
    void setShootCooldown(float cooldown) { m_shootinCooldownMax = cooldown; }

private:
    void clampPosition();

private:
    void createShip();

public:
    inline bool isShooting() const { return m_shooting; }

    inline bool canShoot() 
    { 
        if (m_shootingCooldown >= m_shootinCooldownMax)
        {
            m_shootingCooldown = 0;
            return true;
        }
        return false;
    }

public:
    inline sf::Vector2f direction() const { return {cosf((getRotation() + 90) * 3.1415f / 180.f), sinf((getRotation() + 90) * 3.1415f / 180.f)}; }

    inline sf::RectangleShape getHitbox() const { return m_hitbox; }

public:
    void takeHit() { m_hitBoxEnabled = false; }

    bool isHitBoxEnabled() const { return m_hitBoxEnabled; }

    void resetShip();

private:
    sf::RectangleShape m_hitbox;

// Physics:
private:
    PhysicsComponent m_physics;
    int m_rotationDirection = 0; // Input
    bool m_accelerating = false, m_braking = false; // Input

// Dimensions:
private:
    float m_mainHeight = 20.f, m_extraHeight = 6.f, m_width = 10.f;

// Shooting:
private:
    float m_shootingCooldown = 0.f, m_shootinCooldownMax = 0.4f;
    bool m_shooting = false;

// Hitbox:
private:
    bool m_hitBoxEnabled = true;
    float m_hitboxCooldown = 0.f, m_hitboxCooldownMax = 1.3f;
};