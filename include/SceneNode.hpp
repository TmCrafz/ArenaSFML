#ifndef SCENENODE_HPP
#define SCENENODE_HPP
#include <SFML/Graphics.hpp>
#include <memory>
#include <vector>


class SceneNode : public sf::Transformable, public sf::Drawable, public sf::NonCopyable
{
    public:
        typedef std::unique_ptr<SceneNode> Ptr;

    protected:
    private:
        std::vector<Ptr> m_children;
        SceneNode *m_parent;

    public:
        SceneNode();
        void attachChild(Ptr child);
        Ptr detachChild(const SceneNode& node);

    private:
        // draw should not get overridden
        virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const final;
        virtual void drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const;

};

#endif // SCENENODE_HPP
