#include "scene_main_menu.h"
#include "game.h"

void SceneMainMenu::initialize()
{
    // Initial Asteroids:
    for(size_t i = 0; i < 3; ++i)
        m_asteroids.push_back(new Asteroid());

    // Font:
    m_font = new sf::Font;
    if (!m_font->loadFromFile("free-pixel.ttf"))
        return;
    
    // Text:    
    m_gameTitle.setString("Asteroids");
    m_gameTitle.setCharacterSize(60);

    float w = m_gameTitle.getCharacterSize() * m_gameTitle.getString().getSize();
    m_gameTitle.setPosition({ float(game::windowWidth / 2) - (w / 4), 75 });
    m_gameTitle.setFont(*m_font);
    
    // Buttons:
    m_singlePlayButton = new Button(
        { float((game::windowWidth / 2) - 125), float(game::windowHeight / 2) },
        { 250, 40}, m_font, "Play");

    m_exitButton = new Button(
        { float((game::windowWidth / 2) - 125), float(game::windowHeight / 2) + 150 },
        { 250, 40 }, m_font, "Exit");
}

void SceneMainMenu::update(float dt)
{
    // Buttons:
    m_singlePlayButton->update(sf::Mouse::getPosition(*m_sceneManager->game->getWindow()));
    m_exitButton->update(sf::Mouse::getPosition(*m_sceneManager->game->getWindow()));

    if (m_singlePlayButton->isPressed())
        m_sceneManager->selectScene("playing");

    if (m_exitButton->isPressed())
        m_sceneManager->game->getWindow()->close();

    // Asteroids:
    size_t asteroidCounter = 0;
    for (auto* asteroid : m_asteroids)
    {
        asteroid->update(dt);

        if (asteroid->isOutOfBounds())
        {
            delete m_asteroids.at(asteroidCounter);

            m_asteroids.erase(m_asteroids.begin() + asteroidCounter);

            --asteroidCounter;

            m_asteroids.push_back(new Asteroid());
        }

        ++asteroidCounter;
    }
}

void SceneMainMenu::render(sf::RenderWindow* m_window)
{
    // Text:
    m_window->draw(m_gameTitle);

    // Buttons:
    m_singlePlayButton->render(*m_window);
    m_exitButton->render(*m_window);

    // Asteroids:
    for (auto* asteroid : m_asteroids)
        m_window->draw(*asteroid);
}
