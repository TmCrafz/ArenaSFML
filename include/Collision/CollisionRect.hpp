#ifndef COLLISIONRECT_HPP
#define COLLISIONRECT_HPP
#include <SFML/Graphics.hpp>
#include "Collision/CollisionShape.hpp"
#include "Components/SceneNode.hpp"

class CollisionRect : public CollisionShape
{
    private:
        //sf::RectangleShape m_rectShape;
        float m_width;
        float m_height;
        std::vector<sf::Vector2f> m_vertices;
    public:

        explicit CollisionRect(sf::Vector2f rectSize);

        float getWidth() const;
        float getHeight() const;
        // Determine where the vertices of the rect are and put them into the m_vertices vector.
        void computeVertices();

        sf::Vector2f support();

        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

        virtual bool isColliding(CollisionShape &collider);

        sf::Vector2f getFarthestPointInDirection(sf::Vector2f dir) const;
    private:
        virtual bool isColliding(CollisionCircle &collider);
        virtual bool isColliding(CollisionRect &collider);

};

#endif // COLLISIONRECT_HPP
