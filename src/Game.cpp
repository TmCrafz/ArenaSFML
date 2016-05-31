#include "Game.hpp"
#include <iostream>
#include "Warrior.hpp"

Game::Game(bool showStats)
: m_screenHeight{800}
, m_screenWidth{480}
, m_showStats{showStats}
, m_window{sf::VideoMode{m_screenHeight, m_screenWidth}, "ArenaSFML"}
, m_isRunning{true}
, m_dt{0}
, m_fps{0}
, m_timePoint1{CLOCK::now()}
{
    //m_shape.setFillColor(sf::Color::Green);
    m_window.setFramerateLimit(60);
    if (!m_fontDefault.loadFromFile("fonts/UbuntuMono-R.ttf"))
	{
		std::cout << "Error cannot load Font" << std::endl;
	}
	m_txtStatFPS.setFont(m_fontDefault);
	m_txtStatFPS.setCharacterSize(12);
	m_txtStatFPS.setColor(sf::Color::White);

    //sf::Rect<int> rect;
    //m_textureHolder.load<sf::Rect<int>>(Textures::KNIGHT, "assets/sprites/knight.png", rect);
    m_textureHolder.load(Textures::KNIGHT, "assets/sprites/knight.png");
    std::unique_ptr<Warrior> warrior(new Warrior(Textures::KNIGHT, m_textureHolder));
    warrior->setPosition(m_screenHeight / 2.f, m_screenWidth / 2.f);
    warrior->setVelocity(0.f, -30.f);
    m_sceneGraph.attachChild(std::move(warrior));
    //m_testSprite.setTexture(m_textureHolder.get(Textures::KNIGHT));
    //m_testSprite.setOrigin(m_testSprite.getTextureRect().width / 2.f, m_testSprite.getTextureRect().height / 2.f);
    //m_testSprite.setPosition(m_screenHeight / 2.f, m_screenWidth / 2.f);
}

void Game::run()
{
    while (m_window.isOpen())
    {
        determineDeltaTime();
        processEvents();
        update();
        render();
    }
}

void Game::determineDeltaTime() {
    CLOCK::time_point timePoint2 = { CLOCK::now() };
    std::chrono::duration<float> timeSpan = { timePoint2 - m_timePoint1 };
    m_timePoint1 = CLOCK::now();
    // Get deltaTime as float in seconds
    m_dt = std::chrono::duration_cast<std::chrono::duration<float,std::ratio<1>>> (timeSpan).count();
    m_fps = 1.f / m_dt;
    m_txtStatFPS.setString("FPS: " + std::to_string(m_fps));
}

void Game::processEvents()
{
    sf::Event event;
    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window.close();
        }
    }
}

void Game::update()
{
    m_sceneGraph.update(m_dt);
}

void Game::render()
{
    std::cout << "Render" << std::endl;
    m_window.clear();
    m_window.draw(m_sceneGraph);
    //m_window.draw(m_testSprite);
    m_window.draw(m_txtStatFPS);
    m_window.display();
}



