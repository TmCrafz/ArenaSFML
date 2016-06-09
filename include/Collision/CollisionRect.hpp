#ifndef COLLISIONRECT_HPP
#define COLLISIONRECT_HPP
#include <SFML/Graphics.hpp>
#include "Collision/CollisionShape.hpp"
#include "Components/SceneNode.hpp"

class CollisionRect : public CollisionShape
{
    private:
        sf::RectangleShape m_rectShape;
    public:

        explicit CollisionRect(sf::Vector2f rectSize);

        sf::Vector2f getPosition() const;

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        virtual bool isColliding(const CollisionShape &collider) const;

    private:
        virtual bool isColliding(const CollisionCircle &collider) const;
        virtual bool isColliding(const CollisionRect &collider) const;
};

#endif // COLLISIONRECT_HPP
