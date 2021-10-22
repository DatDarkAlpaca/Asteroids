#pragma once
#include <vector>

#include "ship.h"
#include "scene.h"
#include "bullet.h"
#include "asteroid.h"
#include "button.h"

class ScenePlaying : public Scene
{
public:
    ScenePlaying(int difficulty = 0) : m_difficulty(difficulty)
    {
        setDificulty(difficulty);
    }

public:
    void initialize() override;

    void update(float dt) override;

    void handleInput() override;

    void render(sf::RenderWindow* m_window) override;

    void setDificulty(int difficulty)
    {
        m_difficulty = difficulty;

        switch (m_difficulty)
        {
        case 0:
            m_AsteroidCooldownMax = 7.f;
            m_scoreIncrease = 0.05;
            break;
        case 1:
            m_AsteroidCooldownMax = 5.f;
            m_scoreIncrease = 0.1f;

            if (ship != nullptr)
            {
                ship->setShootCooldown(0.35f);
                ship->setBrakingConstant(60.f);
            }
           
            break;
        case 2:
            m_AsteroidCooldownMax = 3.f;
            m_scoreIncrease = 0.2f;

            if (ship != nullptr)
            {
                ship->setShootCooldown(0.25f);
                ship->setBrakingConstant(70.f);
            }
            
            break;
        case 3:
            m_AsteroidCooldownMax = 2.f;
            m_scoreIncrease = 0.25;

            if (ship != nullptr)
            {
                ship->setShootCooldown(0.1f);
                ship->setBrakingConstant(80.f);
            }
            
            break;
        }
    }

private:
    sf::Text m_highScore, m_scoreText, m_lifeText, m_gameOverText;
    Button *m_restartButton, *m_returnButton;
    sf::Font* m_font = nullptr;

    std::vector<Asteroid*> m_asteroidArray;
    std::vector<Bullet*> m_bulletArray;
    Ship* ship = nullptr;

    int m_difficulty = 0;
    size_t m_maxAsteroids = 15;

    bool m_gameover = false;
    float m_scoreIncrease = 0;
    int m_shipLifes = 0, m_highScoreValue = 0;
    float m_score;

    float m_currentAsteroidCooldown = 0.f, m_AsteroidCooldownMax = 5.f;

private:
    float m_difficultyIncreaseCooldown = 0.f, m_difficultyIncreaseCooldownLimiar = 10.f;
};