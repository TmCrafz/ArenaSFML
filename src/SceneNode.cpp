#include "SceneNode.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

SceneNode::SceneNode()
:m_parent(nullptr)
{

}

void SceneNode::attachChild(Ptr child)
{
    child->m_parent = this;
    m_children.push_back(std::move(child));
}

SceneNode::Ptr SceneNode::detachChild(const SceneNode& node)
{
    auto found = std::find_if(m_children.begin(), m_children.end(), [&] (Ptr &p) -> bool { return p.get() == &node; });
    // There is an error when we try to detach a child which does not exists,so stop execution in debug mode
    assert(found != m_children.end());

    Ptr result = std::move(*found);
    result->m_parent = nullptr;
    m_children.erase(found);
    return result;
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
        states.transform *= getTransform();
        drawCurrent(target, states);
        drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    //Do nothing by default
}

void SceneNode::drawChildren(sf::RenderTarget &target, sf::RenderStates states) const
{
    for (const Ptr &child : m_children)
    {
        child->draw(target, states);
    }
}

void SceneNode::update(float dt)
{
    updateCurrent(dt);
    updateChildren(dt);
}

void SceneNode::updateCurrent(float dt)
{
    // Do nothing by default
}

void SceneNode::updateChildren(float dt)
{
    for (const Ptr &child : m_children)
    {
        child->update(dt);
    }
}
