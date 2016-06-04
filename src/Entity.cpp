#include "Entity.hpp"
#include <iostream>

Entity::Entity()
{

}

void Entity::setVelocity(sf::Vector2f velocity)
{
    m_velocity = velocity;
}

void Entity::setVelocity(float velX, float velY)
{
    m_velocity.x = velX;
    m_velocity.y = velY;
}

sf::Vector2f Entity::getVelocity() const
{
    return m_velocity;
}

void Entity::updateCurrent(float dt)
{
    /*
    m_currentVelocity.x = 0.f;
    m_currentVelocity.y = 0.f;
    if (m_command == Commands::MOVE_UP)
    {
        m_currentVelocity.y = m_velocity.y;
    }
    // Move is the same as setPosition(getPosition() + offset) of the sf::Transformable class
    move(m_currentVelocity * dt);
    */
}

void Entity::onCommandCurrent(const Command &command, float dt)
{
    std::cout << "onCommandCurrent" << std::endl;
    if (command.getWorldObjectType() == m_type)
    {
        m_currentVelocity.x = 0.f;
        m_currentVelocity.y = 0.f;
        if (command.getCommandType() == CommandTypes::MOVE_UP)
        {
            m_currentVelocity.y = -m_velocity.y;
        }
        else if (command.getCommandType() == CommandTypes::MOVE_DOWN)
        {
            m_currentVelocity.y = m_velocity.y;
        }
         else if (command.getCommandType() == CommandTypes::MOVE_LEFT)
        {
            m_currentVelocity.x = -m_velocity.x;
            //std::cout << "MOVE_LEFT Entity" << std::endl;
        }
        else if (command.getCommandType() == CommandTypes::MOVE_RIGHT)
        {
            m_currentVelocity.x = m_velocity.x;
        }
        // Move is the same as setPosition(getPosition() + offset) of the sf::Transformable class
        move(m_currentVelocity * dt);
    }
}
