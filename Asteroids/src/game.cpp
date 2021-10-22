#include "game.h"

void Game::run()
{
    createWindow();

    handleAspectRatio();

    createScenes();

    sf::Clock clock;
    while (m_window->isOpen())
    {
        float dt = clock.restart().asSeconds();

        pollEvents();

        update(dt);

        render();
    }
}

void Game::pollEvents()
{
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
            m_window->close();
    }

    m_sceneManager->handleInput();
}

void Game::update(float dt)
{
    m_sceneManager->update(dt);

    std::string title = "Asteroids";
    m_window->setTitle(title);
}

void Game::render()
{
    m_window->clear(sf::Color::Black);

    m_sceneManager->render(m_window);

    m_window->display();
}

void Game::handleAspectRatio()
{
    // https://en.sfml-dev.org/forums/index.php?topic=20908.0

    sf::View theView = m_window->getView();
    const float windowRatio = static_cast<float>(m_window->getSize().x) / m_window->getSize().y;
    const float newViewWidth = theView.getSize().y * windowRatio;

    theView.setSize({ newViewWidth, theView.getSize().y });
    m_window->setView(theView);
}

void Game::createWindow()
{
    using namespace game;
    if (m_window != nullptr && m_window->isOpen())
        m_window->close();

    m_window = new sf::RenderWindow(sf::VideoMode(windowWidth, windowHeight), "Asteroids");
}

void Game::createScenes()
{
    m_sceneManager = new SceneManager(this);

    m_sceneManager->addScene("playing", new ScenePlaying());
    m_sceneManager->addScene("main_menu", new SceneMainMenu());

    m_sceneManager->selectScene("main_menu");
}
