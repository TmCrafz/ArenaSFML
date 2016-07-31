#ifndef ANIMATIONSTEPMOVEMENT_HPP
#define ANIMATIONSTEPMOVEMENT_HPP
#include <SFML/Graphics.hpp>
#include "Animation/AnimationStep.hpp"

class AnimationStepMovement : public AnimationStep
{
    private:
        sf::Vector2f m_startPosition;
        float m_totalDistance;
        sf::Vector2f m_direction;
        // The movement speed per second
        sf::Vector2f m_movementPerSecond;

    public:
        AnimationStepMovement(sf::Vector2f startPosition, float totalDistance, sf::Vector2f direction, float duration);

        sf::Vector2f getStartPosition() const;
        float getTotalDistance() const;
        sf::Vector2f getMovementSpeed() const;
};

#endif // ANIMATIONSTEPMOVEMENT_HPP
