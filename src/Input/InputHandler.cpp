#include "Input/InputHandler.hpp"
#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include "Calc.hpp"

InputHandler::InputHandler(sf::RenderWindow *window)
: m_window{window}
{

}

void InputHandler::handleInput(QueueHelper<Input> &inputQueue)
{
    handleEvents(inputQueue);
    handleRealTimeInput(inputQueue);
}

void InputHandler::handleEvents(QueueHelper<Input> &inputQueue)
{
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            m_window->close();
        }
        else if (event.type == sf::Event::Resized)
        {
            // Do nothing at the moment
        }
        else if (event.type == sf::Event::LostFocus)
        {
            // Do nothing at the moment
        }
        else if (event.type == sf::Event::GainedFocus)
        {
            // Do nothing at the moment
        }

    }
}

void InputHandler::handleRealTimeInput(QueueHelper<Input> &inputQueue)
{
    const sf::Vector2i CurrentMousePos = { sf::Mouse::getPosition(*m_window) };
    if (CurrentMousePos != m_lastMousePos)
    {
        const sf::Vector2i WindowCenter(static_cast<int>(m_window->getSize().x / 2), static_cast<int>(m_window->getSize().y / 2));
        const sf::Vector2i TranslatedMousePos = { CurrentMousePos - WindowCenter };
        const sf::Vector2f TranslatedMousePosF(static_cast<sf::Vector2f>(TranslatedMousePos));
        inputQueue.push({ InputTypes::TRANSLATED_CURSOR_POS, TranslatedMousePosF });
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        inputQueue.push({InputTypes::UP});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        inputQueue.push({InputTypes::DOWN});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        inputQueue.push({InputTypes::LEFT});
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        inputQueue.push({InputTypes::RIGHT});
    }

    m_lastMousePos.x = CurrentMousePos.x;
    m_lastMousePos.y = CurrentMousePos.y;
    /*
    const sf::Vector2i UnitVecX(1, 0);
    const sf::Vector2i WindowCenter(static_cast<int>(m_window->getSize().x / 2), static_cast<int>(m_window->getSize().y / 2));
    const sf::Vector2i CurrentMousePos = { sf::Mouse::getPosition(*m_window) };
    // The angle which should be calculated have the coordinate systems midpoint a the center of the window,
    // so we have to translate the mouse position so its relativ to the center of the window
    const sf::Vector2i TranslatedMousePos = { CurrentMousePos - WindowCenter };
    // Calculate angle between the unit vector and the translated mouse position
    const float Angle = { Calc::radToDeg(Calc::getVec2Angle<sf::Vector2i, sf::Vector2i>(UnitVecX, TranslatedMousePos)) };
    const float AngleSigned = TranslatedMousePos.y < 0 ? -Angle : Angle;
    if (CurrentMousePos != m_lastMousePos)
    {
        commandQueue.push({ CommandTypes::ROTATE, WorldObjectTypes::Player, { AngleSigned, 0.f } });
    }

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        commandQueue.push({ CommandTypes::MOVE_UP, WorldObjectTypes::Player });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        commandQueue.push({ CommandTypes::MOVE_DOWN, WorldObjectTypes::Player });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        commandQueue.push({ CommandTypes::MOVE_LEFT, WorldObjectTypes::Player });
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        commandQueue.push({ CommandTypes::MOVE_RIGHT, WorldObjectTypes::Player });
    }

    m_lastMousePos.x = CurrentMousePos.x;
    m_lastMousePos.y = CurrentMousePos.y;
    */
}
