#include "ship.h"
#include "settings.h"

Ship::Ship()
{
	createShip();
}

void Ship::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	states.transform *= getTransform();
	target.draw(m_vertices, states);
}

void Ship::update(float dt)
{
    // Hitbox:
    m_hitbox.setPosition({
        getPosition().x,
        getPosition().y
    });

    m_hitbox.setOrigin({
        getBounds().width / 2,
        getBounds().height / 2
    });

    m_hitbox.setSize({
        getBounds().width,
        getBounds().height
    });

    m_hitbox.setRotation(getRotation());

    // Shooting Cooldown:
    if (m_shootingCooldown < m_shootinCooldownMax)
       m_shootingCooldown += dt;

    // Hitbox Cooldown:
    if (!m_hitBoxEnabled)
    {
        if (m_hitboxCooldown < m_hitboxCooldownMax)
        {
            m_hitboxCooldown += dt;
            setColor(sf::Color::Red);
        }
        else
        {
            m_hitBoxEnabled = true;
            m_hitboxCooldown = 0;
            setColor(sf::Color::White);
        }
    }

    // Movement:
    if (m_accelerationDirection == 1)
        accelerate(dt);
    else if (m_accelerationDirection == -1)
        brake(dt, 2.f);
    else
        brake(dt);

    // Clamping:
    clampVelocity();
    clampPosition();

    rotate(m_rotationDirection * dt * m_angularSpeed);

    move(m_currentVelocity * dt);
}

void Ship::handleInput()
{
    // Position:
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        m_accelerationDirection = 1;

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_accelerationDirection = -1;

    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::W) && !sf::Keyboard::isKeyPressed(sf::Keyboard::S))
        m_accelerationDirection = 0;

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

void Ship::setColor(const sf::Color& color)
{
    for (size_t i = 0; i < m_vertices.getVertexCount(); ++i)
        m_vertices[i].color = color;
}

void Ship::accelerate(float dt)
{
    m_currentVelocity.x += m_accelerationDirection * m_acceleration * dt * cosf((getRotation() + 90.f) * 3.1415f / 180.f);
    m_currentVelocity.y += m_accelerationDirection * m_acceleration * dt * sinf((getRotation() + 90.f) * 3.1415f / 180.f);
}

void Ship::clampVelocity()
{
    if (m_currentVelocity.x >= m_maximumSpeed)
        m_currentVelocity.x = m_maximumSpeed;

    else if (m_currentVelocity.x <= -m_maximumSpeed)
        m_currentVelocity.x = -m_maximumSpeed;

    if (m_currentVelocity.y >= m_maximumSpeed)
        m_currentVelocity.y = m_maximumSpeed;

    else if (m_currentVelocity.y <= -m_maximumSpeed)
        m_currentVelocity.y = -m_maximumSpeed;
}

void Ship::clampPosition()
{
    if (getPosition().x <= -m_width && m_currentVelocity.x < 0)
        setPosition(game::windowWidth + m_width, getPosition().y);

    if (getPosition().x > game::windowWidth + m_width && m_currentVelocity.x > 0)
        setPosition(-m_width, getPosition().y);

    float total_height = (m_extraHeight + m_mainHeight) / 2;
    if (getPosition().y <= -total_height && m_currentVelocity.y < 0)
        setPosition(getPosition().x, game::windowHeight + total_height);

    if (getPosition().y > game::windowHeight + total_height && m_currentVelocity.y > 0)
        setPosition(getPosition().x, -total_height);
}

void Ship::brake(float dt, float multiplier)
{
    if (m_currentVelocity.y >= 0)
        m_currentVelocity += { 0, -m_breakingConstant * dt * multiplier };

    if (m_currentVelocity.y <= 0)
        m_currentVelocity += { 0, m_breakingConstant* dt * multiplier };

    if (m_currentVelocity.x >= 0)
        m_currentVelocity += { -m_breakingConstant * dt * multiplier, 0 };

    if (m_currentVelocity.x <= 0)
        m_currentVelocity += { m_breakingConstant* dt * multiplier, 0};
}

void Ship::createShip()
{
    m_hitbox.setOutlineColor(sf::Color::Green);
    m_hitbox.setFillColor(sf::Color::Transparent);
    m_hitbox.setOutlineThickness(1);

    // Origin:
    setOrigin({ 0, -(m_mainHeight + m_extraHeight) / 2 });

    // Shape:
    m_vertices.setPrimitiveType(sf::PrimitiveType::LineStrip);
    m_vertices.resize(5);

    m_vertices[0].position = { 0.f, 0.f };
    m_vertices[1].position = { -m_width, -(m_mainHeight + m_extraHeight) };
    m_vertices[2].position = { 0.f, -m_mainHeight };
    m_vertices[3].position = { m_width, -(m_mainHeight + m_extraHeight) };
    m_vertices[4].position = { 0.f, 0.f };

    for (size_t i = 0; i < m_vertices.getVertexCount(); ++i)
        m_vertices[i].color = sf::Color::White;

    // Bounding box:
    m_boundingBox.left = -m_width;
    m_boundingBox.width = 2 * m_width;
    m_boundingBox.top = -(m_mainHeight + m_extraHeight);
    m_boundingBox.height = m_mainHeight + m_extraHeight;
}
