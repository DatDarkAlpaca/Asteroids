#include "asteroid.h"

Asteroid::Asteroid(bool followPlayer)
    :m_followPlayer(followPlayer)
{
    {
        m_physics.minSpeed = 10.f;
        m_physics.maxSpeed = 70.f;

        createAsteroid();
        spawn();
    }
}

Asteroid::Asteroid(unsigned size, const sf::Vector2f position, const sf::Vector2f velocity, bool followPlayer)
    :m_size(size), m_followPlayer(followPlayer)
{
    m_physics.minSpeed = 10.f;
    m_physics.maxSpeed = 70.f;

    using namespace game;
    createAsteroid();

    setPosition(position);
    m_physics.velocity = velocity;

    m_physics.angularSpeed = (float)(intDis(gen, param_int_t(-4, 3)) + 1);
}

void Asteroid::update(float dt)
{
    // Hitbox:
    updateHitbox();

    move(m_physics.velocity * dt);
    rotate(m_physics.angularSpeed * dt);
}

void Asteroid::updateHitbox()
{
    m_hitbox.setPosition({ getPosition().x, getPosition().y });
    m_hitbox.setRotation(getRotation());
}

void Asteroid::followPlayer(const sf::Vector2f& playerPos, float dt)
{
    if (!m_followPlayer)
        return;

    sf::Vector2f dir = playerPos - getPosition();
    game::normalize(dir);

    using namespace game;
    m_physics.velocity.x = dir.x * intDis(gen, param_int_t((int)m_physics.minSpeed, (int)m_physics.maxSpeed));
    m_physics.velocity.y = dir.y * intDis(gen, param_int_t((int)m_physics.minSpeed, (int)m_physics.maxSpeed));
}

bool Asteroid::isOutOfBounds()
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

void Asteroid::spawn()
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
            m_physics.velocity.y *= -1;

        break;
    }

    case SpawnPosition::Right:
    {
        x = windowWidth + halfWidth + m_boundOffset;

        y = intDis(gen, param_int_t(
            -(m_boundOffset + halfHeight),
            windowHeight + m_boundOffset - halfHeight)
        );

        if (invertVelocity)
            m_physics.velocity.y *= -1;

        m_physics.velocity.x *= -1;
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
            m_physics.velocity.x *= -1;

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

        if (invertVelocity)
            m_physics.velocity.x *= -1;

        m_physics.velocity.y *= -1;
        break;
    }

    default:
        break;
    }

    setPosition({ float(x), float(y) });
}

void Asteroid::createAsteroid()
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

        points.push_back({ x, y });
    }

    // Create the shape:
    setFillColor(sf::Color::Transparent);
    setOutlineColor(m_normalColor);
    setOutlineThickness(1.f);

    setPointCount(points.size());
    for (size_t i = 0; i < points.size(); ++i)
        setPoint(i, points[i]);

    // Random velocity:
    m_physics.angularSpeed = (float)(intDis(gen, param_int_t(-4, 3)) + 1);

    m_physics.velocity.x = (float)intDis(gen, param_int_t((int)m_physics.minSpeed, (int)m_physics.maxSpeed));
    m_physics.velocity.y = (float)intDis(gen, param_int_t((int)m_physics.minSpeed, (int)m_physics.maxSpeed));

    // Hitbox color:
    float size = std::min(getGlobalBounds().width / 2, getGlobalBounds().height / 2);
    m_hitbox.setSize({ size, size });

    m_hitbox.setOrigin({
        getGlobalBounds().width / 4,
        getGlobalBounds().height / 4,
        });

    m_hitbox.setOutlineColor(sf::Color::Green);
    m_hitbox.setFillColor(sf::Color::Transparent);
    m_hitbox.setOutlineThickness(1);
}
