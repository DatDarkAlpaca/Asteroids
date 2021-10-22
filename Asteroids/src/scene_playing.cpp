#include <fstream>
#include <cmath>

#include "scene_playing.h"
#include "random.h"
#include "game.h"

static bool collides(const Asteroid* asteroid, const Bullet* bullet)
{
	if(asteroid != nullptr && bullet != nullptr)
		return asteroid->getGlobalBounds().intersects(bullet->getGlobalBounds());
	return false;
}

static bool collides(const Asteroid* asteroid, const Ship* ship)
{
	return ship->getHitbox().getGlobalBounds().intersects(asteroid->getHitbox().getGlobalBounds());
}

void ScenePlaying::initialize()
{
	using namespace game;

	// Get highscore:
	std::fstream highScoreFile("data.d", std::ios_base::in);
	if(highScoreFile.good())
		highScoreFile >> m_highScoreValue;
	highScoreFile.close();

	// Font:
	m_font = new sf::Font;
	if (!m_font->loadFromFile("free-pixel.ttf"))
		std::cout << "Could not load font\n";

	// Text:
	m_highScore.setString("Highscore: " + std::to_string(m_highScoreValue));
	m_scoreText.setString("Score: " + std::to_string(m_score));
	m_lifeText.setString("Lifes: ");
	m_gameOverText.setString("Gameover");

	m_highScore.setCharacterSize(24);
	m_scoreText.setCharacterSize(24);
	m_lifeText.setCharacterSize(24);

	m_highScore.setFont(*m_font);
	m_lifeText.setFont(*m_font);
	m_scoreText.setFont(*m_font);
	m_gameOverText.setFont(*m_font);

	m_scoreText.setPosition({ 0, 24 });
	m_lifeText.setPosition({ 0, 48 });
	m_gameOverText.setPosition({ 0, 72 });

	// Buttons:
	m_restartButton = new Button({ 0, 124 }, { 30 * 8, 30 }, m_font, "Restart?", false);

	// Initial asteroids:
	for (size_t i = 0; i < 3; i++)
		m_asteroidArray.push_back(new Asteroid(true));

	// Player:
	ship = new Ship();
	ship->setColor(sf::Color::Cyan);

	ship->setPosition({ float(rand() % 100 + 400), float(rand() % 100 + 400) });
	ship->setRotation((float)intDis(gen, param_int_t(0, 359)));
}

void ScenePlaying::update(float dt)
{
	// Score & Lifes:
	m_highScore.setString("Highscore: " + std::to_string(m_highScoreValue));
	m_scoreText.setString("Score: " + std::to_string((int)m_score));
	m_lifeText.setString("Lifes: " + std::to_string(m_shipLifes));

	// Gameover:
	if (m_gameover)
	{
		m_restartButton->update(sf::Mouse::getPosition(*m_sceneManager->game->getWindow()));

		if (m_restartButton->isPressed())
		{
			// Saving highscore:
			if (m_score >= m_highScoreValue)
			{
				m_highScoreValue = m_score;

				std::ofstream  highScoreFile("data.d", std::ios_base::trunc);
				if (highScoreFile.good())
					highScoreFile << m_highScoreValue;
				highScoreFile.close();
			}

			ship->setShootCooldown(0.4f);
			ship->setBrakingConstant(50.f);

			m_difficulty = 0;
			m_score = 0;
			ship->resetCooldown();
			ship->stop();

			m_gameover = false;
			ship->setHealth(3);
			ship->setColor(sf::Color::White);

			// Restart asteroids:
			for (size_t i = 0; i < m_asteroidArray.size(); ++i)
			{
				delete m_asteroidArray.at(i);
				m_asteroidArray.erase(m_asteroidArray.begin() + i);
				--i;
			}

			// Restart bullets:
			for (size_t i = 0; i < m_bulletArray.size(); ++i)
			{
				delete m_bulletArray.at(i);
				m_bulletArray.erase(m_bulletArray.begin() + i);
				--i;
			}

			// Restart Ship:
			using namespace game;
			ship->setPosition({ float(rand() % 100 + 400), float(rand() % 100 + 400) });
			ship->setRotation((float)intDis(gen, param_int_t(0, 359)));

			// New Asteroids:
			for(size_t i = 0; i < 3; ++i)
				m_asteroidArray.push_back(new Asteroid(true));
		}
		
		return;
	}

	// Increase difficulty:
	if (m_difficultyIncreaseCooldown <= m_difficultyIncreaseCooldownLimiar && m_difficulty < 3)
		m_difficultyIncreaseCooldown += dt;
	else
	{
		if (m_difficulty < 3)
		{
			std::cout << "Dificculty increased\n";
			m_difficultyIncreaseCooldown = 0;
			setDificulty(m_difficulty + 1);
		}
	}

	// Ship:
	ship->update(dt);
	if (ship->getHealth() <= 0)
		m_gameover = true;

	// Shooting:
	if (ship->isShooting() && ship->canShoot())
		m_bulletArray.push_back(new Bullet(ship->getPosition(), ship->direction()));

	// Bullet update:
	size_t bulletCounter = 0;
	for (auto* bullet : m_bulletArray)
	{
		bullet->update(dt);

		// Out of screen bullets:
		if (bullet->isOutOfScreen())
		{
			delete m_bulletArray.at(bulletCounter);
			m_bulletArray.erase(m_bulletArray.begin() + bulletCounter);
			--bulletCounter;
		}

		++bulletCounter;
	}

	// Asteroid update:
	size_t asteroidCounter = 0;
	for (auto* asteroid : m_asteroidArray)
	{
		asteroid->update(dt);
		asteroid->followPlayer(ship->getPosition(), dt);

		// Out of screen bullets:
		if (asteroid->isOutOfBounds())
		{
			delete m_asteroidArray.at(asteroidCounter);
			m_asteroidArray.erase(m_asteroidArray.begin() + asteroidCounter);
			--asteroidCounter;
		}

		++asteroidCounter;
	}

	// Collisions:
	for (size_t asteroidCounter = 0; asteroidCounter < m_asteroidArray.size(); ++asteroidCounter)
	{
		// Asteroid-Ship collision:
		if (collides(m_asteroidArray[asteroidCounter], ship) && ship->isHitBoxEnabled())
		{
			// Score:
			m_score -= 10;

			// Delete the ship:
			delete m_asteroidArray.at(asteroidCounter);
			m_asteroidArray.erase(m_asteroidArray.begin() + asteroidCounter);
			--asteroidCounter;

			// Decrease the health:
			ship->takeHit();

			// Disable the ship's hitbox:
			ship->disableHitbox();
			continue;
		}

		// Asteroid-bullet collision:
		bulletCounter = 0;
		for (auto* bullet : m_bulletArray)
		{
			if (collides(m_asteroidArray[asteroidCounter], bullet))
			{
				// Score:
				m_score += int(50 / m_asteroidArray[asteroidCounter]->getSize());

				// Spawn two more asteroids:
				if (m_asteroidArray[asteroidCounter]->getSize() > 1)
				{
					m_asteroidArray.push_back(new Asteroid(
						m_asteroidArray[asteroidCounter]->getSize() - 1,
						m_asteroidArray[asteroidCounter]->getPosition(),
						{
							m_asteroidArray[asteroidCounter]->getVelocity().x * 2,
							m_asteroidArray[asteroidCounter]->getVelocity().y * -2
						}
					));

					m_asteroidArray.push_back(new Asteroid(
						m_asteroidArray[asteroidCounter]->getSize() - 1,
						m_asteroidArray[asteroidCounter]->getPosition(),
						{
							m_asteroidArray[asteroidCounter]->getVelocity().x * -2,
							m_asteroidArray[asteroidCounter]->getVelocity().y * 2
						}
					));
				}
				
				// Destroy the asteroid:
				delete m_asteroidArray.at(asteroidCounter);
				m_asteroidArray.erase(m_asteroidArray.begin() + asteroidCounter);
				--asteroidCounter;

				// Spawn extra asteroids:
				if (game::intDis(game::gen, game::param_int_t(0, 1)))
				{
					m_asteroidArray.push_back(new Asteroid());
					asteroidCounter += 2;
				}
				
				// Spawned two asteroids:
				asteroidCounter += 2;

				// Destroy the bullet:
				delete m_bulletArray.at(bulletCounter);
				m_bulletArray.erase(m_bulletArray.begin() + bulletCounter);
				--bulletCounter;

				break;
			}

			++bulletCounter;
		}
	}

	// New asteroids:
	bool shallFollow = game::intDis(game::gen, game::param_int_t(0, 1));
	if (m_currentAsteroidCooldown >= m_AsteroidCooldownMax && m_asteroidArray.size() <= (m_maxAsteroids * size_t(m_difficulty + 1)))
	{
		m_currentAsteroidCooldown = 0;
		int asteroidAmount = 1;
		asteroidAmount = game::intDis(game::gen, game::param_int_t((m_difficulty + 1), 2 * (m_difficulty + 1)));

		for(size_t i = 0; i < asteroidAmount; ++i)
			m_asteroidArray.push_back(new Asteroid(shallFollow));

		std::cout << "Spawned " << asteroidAmount << " entities - D: " << m_difficulty << '\n';
	}
	else
		m_currentAsteroidCooldown += dt;

	// Update ship life:
	m_shipLifes = ship->getHealth();
	
	// Update highscore:
	if (m_score > m_highScoreValue)
		m_highScoreValue = m_score;

	// PassiveScore:
	m_score += m_scoreIncrease;

	if (m_asteroidArray.size() <= 3)
	{
		m_asteroidArray.push_back(new Asteroid(true));
		m_asteroidArray.push_back(new Asteroid(true));
	}

	// Auto-highscore:
	if (m_score >= m_highScoreValue)
	{
		m_highScoreValue = m_score;

		std::ofstream  highScoreFile("data.d", std::ios_base::trunc);
		if (highScoreFile.good())
			highScoreFile << m_highScoreValue;
		highScoreFile.close();
	}
}

void ScenePlaying::handleInput()
{
	ship->handleInput();
}

void ScenePlaying::render(sf::RenderWindow* m_window)
{
	// Ship:
	if(ship->getHealth() > 0)
		m_window->draw(*ship);
	else
	{
		m_window->draw(m_gameOverText);
		m_restartButton->render(*m_window);
	}

	// Score & Life:
	m_window->draw(m_highScore);
	m_window->draw(m_scoreText);
	m_window->draw(m_lifeText);

	// Bullets:
	for (auto *bullet : m_bulletArray)
		m_window->draw(*bullet);

	// Asteroids:
	for (auto* asteroid : m_asteroidArray)
		m_window->draw(*asteroid);
}
