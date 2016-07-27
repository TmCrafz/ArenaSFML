#include "Components/SceneNode.hpp"
#include <algorithm>
#include <cassert>
#include <iostream>

SceneNode::SceneNode()
: m_parent{ nullptr }
, m_collisionShape{ nullptr }
, m_type{ WorldObjectTypes::NONE }
, m_isActive{ true }
{

}

SceneNode::SceneNode(WorldObjectTypes type)
: m_parent{ nullptr }
, m_collisionShape{ nullptr }
, m_type{ type }
, m_isActive{ true }
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

void SceneNode::setCollisionShape(std::unique_ptr<CollisionShape> collisionShape)
{
    m_collisionShape = std::move(collisionShape);
    m_collisionShape->setParent(this);
}

void SceneNode::draw(sf::RenderTarget &target, sf::RenderStates states) const
{
        states.transform *= getTransform();
        drawCurrent(target, states);
        drawCollisionShape(target, states);
        drawChildren(target, states);
}

void SceneNode::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    //Do nothing by default
}

void SceneNode::drawCollisionShape(sf::RenderTarget &target, sf::RenderStates states) const
{
    // Only draw collision shape when it is not nullptr
    if (m_collisionShape)
    {
        m_collisionShape->draw(target, states);
    }
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

void SceneNode::onCommand(const Command &command, float dt)
{
    onCommandCurrent(command, dt);
    onCommandChildren(command, dt);
}

void SceneNode::onCommandCurrent(const Command &command, float dt)
{
    // Do nothing by default
}

void SceneNode::onCommandChildren(const Command &command, float dt)
{
    for (const Ptr &child : m_children)
    {
        child->onCommand(command, dt);
    }
}

sf::Transform SceneNode::getWorldTransform() const
{
    sf::Transform trform = { sf::Transform::Identity };
    for (const SceneNode *node = this; node != nullptr; node = node->m_parent)
    {
        trform = node->getTransform() * trform;
    }
    return trform;
}

sf::Vector2f SceneNode::getWorldPosition() const
{
    return getWorldTransform() * sf::Vector2f();
}



float SceneNode::getWorldRotation() const
{
    float rotation = 0.f;
    for (const SceneNode *node = this; node != nullptr; node = node->m_parent)
    {
        rotation += node->getRotation();
    }
    return rotation;
}

WorldObjectTypes SceneNode::getType() const
{
    return m_type;
}

void SceneNode::setType(WorldObjectTypes type)
{
    m_type = type;
}

CollisionShape* SceneNode::getCollisionShape() const
{
    return m_collisionShape.get();
}

bool SceneNode::isActive() const
{
    return m_isActive;
}
void SceneNode::setIsActive(bool isActive)
{
    m_isActive = isActive;
}

CollisionInfo SceneNode::isColliding(SceneNode &node) const
{
    // If there is no collision shape specified there can not be a collision
    if (m_collisionShape == nullptr || node.getCollisionShape() == nullptr)
    {
        return CollisionInfo(false);
    }
    return m_collisionShape->isColliding(*node.getCollisionShape());
}

void SceneNode::checkSceneCollision(SceneNode &sceneGraph, std::vector<CollisionInfo> &collisionData)
{
    // Store the collisionPairs, so we can later check if the same collision is already stored, so we only
    // store the Collision information once.
    std::set<Pair> collisionPairs;
    checkSceneCollision(sceneGraph, collisionPairs, collisionData);
}

void SceneNode::checkSceneCollision(SceneNode &sceneGraph, std::set<Pair> &collisionPairs, std::vector<CollisionInfo> &collisionData)
{
    checkNodeCollision(sceneGraph, collisionPairs, collisionData);

    for (Ptr &child: sceneGraph.m_children)
    {
        checkSceneCollision(*child, collisionPairs, collisionData);
    }

}

void SceneNode::checkNodeCollision(SceneNode &node, std::set<Pair> &collisionPairs, std::vector<CollisionInfo> &collisionData)
{
    // If the actual SceneNode is passive we dont have to check if it colliding with something.
    // If the other node is active it will check if it is colliding with this node.
    if (this != &node && m_isActive)
    {

        // std::minmax return always the same pair independent of the order of the parameters, where the first is the smallest and the second the greater one
        // (The smaller one have the lower address in this case). In std::set unique objects are stored as key and dont add a new key if there is already the same
        // so with std::minmax we ensure that we only store a collison once (a collides with b and b collisdes with a,
        // but we only need to check the collision between the two once). So when we store the SceneNodes between the collison was checked,
        // we dont have to check the collision twice
        Pair sceneNodePair = std::minmax(this, &node);
        auto inserted = collisionPairs.insert(sceneNodePair);
        // Only check for collision when we dont have already check collision between this pair of nodes.
        // (When inserted is false, the pair was not inserted in the set container, so we have already checked the collision between them)
        if (inserted.second)
        {
            CollisionInfo collisionInfo = { isColliding(node) };
            if (collisionInfo.isCollision())
            {
                // Add collision information and SceneNodes to collisionData vector only when the pair of SceneNodes were successfully added to collisionPairs set,
                // so we can ensure that we add the collisio only once.
                collisionData.push_back(collisionInfo);
            }
        }
    }

    for (Ptr &child: m_children)
    {
        child->checkNodeCollision(node, collisionPairs, collisionData);
    }
}

void SceneNode::safeTransform()
{
    safeCurrentTransform();
    safeChildTransform();
}

void SceneNode::safeChildTransform()
{
    for (const Ptr &child : m_children)
    {
        child->safeTransform();
    }
}

void SceneNode::safeCurrentTransform()
{
    m_lastPos = getPosition();
    m_lastRot = getRotation();
    m_lastScal = getScale();
}

void SceneNode::restoreLastTransform()
{
    setPosition(m_lastPos);
    setRotation(m_lastRot);
    setScale(m_lastScal);
}


