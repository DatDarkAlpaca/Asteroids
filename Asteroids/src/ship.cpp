#include "ship.h"
#include "settings.h"

Ship::Ship()
{
	createShip();

    // Initializing physics:
    m_physics.acceleration = { 350.f, 350.f };
    m_physics.angularSpeed = 200.f;
    m_physics.drag = 150.f;
    m_physics.maxSpeed = 150.f;
    m_physics.minSpeed = -150.f;
}

void Ship::update(float dt)
{
    // Hitbox:
    updateHitbox();

    // Shooting Cooldown:
    if (m_shootingCooldown < m_shootinCooldownMax)
       m_shootingCooldown += dt;

    // Hitbox Cooldown:
    if (!m_hitBoxEnabled)
    {
        if (m_hitboxCooldown < m_hitboxCooldownMax)
        {
            m_hitboxCooldown += dt;
            setOutlineColor(sf::Color::Red);
        }
        else
        {
            m_hitBoxEnabled = true;
            m_hitboxCooldown = 0;
            setOutlineColor(sf::Color::White);
        }
    }

    // Movement:
    if (m_accelerating)
        m_physics.accelerate(dt, getRotation());
    else if (m_braking)
        m_physics.brake(dt, 2.f);
    else
        m_physics.brake(dt);

    // Clamping:
    m_physics.clampVelocity();
    clampPosition();

    rotate(m_rotationDirection * dt * m_physics.angularSpeed);

    move(m_physics.velocity * dt);
}

void Ship::handleInput()
{
    m_accelerating = sf::Keyboard::isKeyPressed(sf::Keyboard::W);

    m_braking = sf::Keyboard::isKeyPressed(sf::Keyboard::S);   

    // Rotation:
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        m_rotationDirection = 1;
    else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        m_rotationDirection = -1;
    else
        m_rotationDirection = 0;

    // Shooting:
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !m_shooting)
        m_shooting = true;
    else
        m_shooting = false;
}

void Ship::updateHitbox()
{
    // Hitbox:
    m_hitbox.setPosition(getPosition());
    m_hitbox.setRotation(getRotation());
}

void Ship::clampPosition()
{
    if (getPosition().x <= -m_width && m_physics.velocity.x < 0)
        setPosition(game::windowWidth + m_width, getPosition().y);

    if (getPosition().x > game::windowWidth + m_width && m_physics.velocity.x > 0)
        setPosition(-m_width, getPosition().y);

    float total_height = (m_extraHeight + m_mainHeight) / 2;
    if (getPosition().y <= -total_height && m_physics.velocity.y < 0)
        setPosition(getPosition().x, game::windowHeight + total_height);

    if (getPosition().y > game::windowHeight + total_height && m_physics.velocity.y > 0)
        setPosition(getPosition().x, -total_height);
}

void Ship::createShip()
{
    // Origin:
    setOrigin({ 0, -(m_mainHeight + m_extraHeight) / 2 });

    // Shape:
    std::vector<sf::Vector2f> points;
 
    points.push_back({ 0.f, 0.f });
    points.push_back({ -m_width, -(m_mainHeight + m_extraHeight) });
    points.push_back({ 0.f, -m_mainHeight });
    points.push_back({ m_width, -(m_mainHeight + m_extraHeight) });

    // Setting the shape:
    setFillColor(sf::Color::Transparent);
    setOutlineColor(sf::Color::White);
    setOutlineThickness(1.f);

    setPointCount(points.size());
    for (size_t i = 0; i < points.size(); ++i)
        setPoint(i, points[i]);

    // Hitbox:
    m_hitbox.setOrigin({ m_width, (m_mainHeight + m_extraHeight) / 2 });
    m_hitbox.setSize({ m_width * 2, m_mainHeight + m_extraHeight });

    m_hitbox.setOutlineColor(sf::Color::Green);
    m_hitbox.setFillColor(sf::Color::Transparent);
    m_hitbox.setOutlineThickness(1);
}

void Ship::resetShip()
{
    setShootCooldown(0.4f);

    m_hitBoxEnabled = true;
    m_hitboxCooldown = 0;

    m_physics.stop();

    setOutlineColor(sf::Color::White);
}
