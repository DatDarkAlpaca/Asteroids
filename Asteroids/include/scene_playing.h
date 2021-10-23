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
    // Todo: wave handler
    ScenePlaying(int difficulty = 0);

public:
    void initialize() override;

    void update(float dt) override;

    void handleInput() override;

    void render(sf::RenderWindow* m_window) override;

private:
    // Todo: wave handler
    void setDificulty(int difficulty);

    void resetScene();

// UI:
private:
    sf::Text m_highScore, m_scoreText, m_lifeText, m_gameOverText;
    Button *m_restartButton, *m_returnButton;
    sf::Font* m_font = nullptr;

// GameObjects:
private:
    std::vector<Asteroid*> m_asteroidArray;
    std::vector<Bullet*> m_bulletArray;
    Ship* ship = nullptr;

// Game Variables:
private:
    int m_highScoreValue = 0;
    float m_scoreIncrease = 0;
    float m_score;
 
    bool m_gameover = false;
    int m_shipLifes = 3;

// Difficulty:
private:
    int m_difficulty = 0;
    size_t m_maxAsteroids = 15;

// Asteroid spawn:
private:
    float m_currentAsteroidCooldown = 0.f, m_AsteroidCooldownMax = 5.f;

// Difficulty cooldown:
private:
    float m_difficultyIncreaseCooldown = 0.f, m_difficultyIncreaseCooldownLimiar = 10.f;
};