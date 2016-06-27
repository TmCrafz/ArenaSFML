#include "World.hpp"
#include "Collision/CollisionShape.hpp"
#include "Collision/CollisionCircle.hpp"
#include "Collision/CollisionRect.hpp"
#include "Components/SpriteNode.hpp"
#include "Components/Warrior.hpp"
#include "Calc.hpp"
#include <memory>

World::World(sf::RenderWindow *window, const ResourceHolder<sf::Font, Fonts> &FontHolder, const ResourceHolder<sf::Texture, Textures> &TextureHolder
    /*,QueueHelper<Input> *inputQueue*/)
: m_window{window}
, m_worldView{m_window->getDefaultView()}
, m_FontHolder{FontHolder}
, m_TextureHolder{TextureHolder}
//, m_inputQueue{inputQueue}
, m_worldBounds{ 0.f, 0.f, 800.f, 480.f }
, m_playerWarrior{nullptr}
{

}

void World::buildScene()
{
    std::cout << "World::buildScene" << std::endl;
    for (std::size_t i = { 0 }; i < Layers::COUNT; i++)
    {
        // Use std::unique_ptr<SceneNode>
        SceneNode::Ptr layer(new SceneNode());
        m_sceneLayers[i] = layer.get();
        m_sceneGraph.attachChild(std::move(layer));
    }
    /*
    std::unique_ptr<Warrior> wizard(new Warrior(100, Textures::WIZARD, m_TextureHolder));
    SceneNode *wizardEnemyTmp = wizard.get();
    std::unique_ptr<CollisionShape> collisionShapeWizard(new CollisionRect({ 40.f, 20.f }));
    wizard->setCollisionShape(std::move(collisionShapeWizard));
    //wizard->setPosition(800 / 2.f + 100.f, 480 / 2.f);
    wizard->setPosition(0.f, 0.f);
    wizard->setVelocity(30.f, 30.f);
    wizard->setType(WorldObjectTypes::ENEMY);
    m_sceneLayers[Layers::MAIN]->attachChild(std::move(wizard));

    std::unique_ptr<Warrior> wizard2(new Warrior(100, Textures::WIZARD, m_TextureHolder));
    SceneNode *wizardEnemyTmp2 = wizard2.get();
    std::unique_ptr<CollisionShape> collisionShapeWizard2(new CollisionRect({ 30.f, 20.f }));
    wizard2->setCollisionShape(std::move(collisionShapeWizard2));
    //wizard->setPosition(800 / 2.f + 100.f, 480 / 2.f);
    wizard2->setPosition(25.f, -10.f);
    wizard2->setVelocity(30.f, 30.f);
    wizard2->setType(WorldObjectTypes::ENEMY);
    m_sceneLayers[Layers::MAIN]->attachChild(std::move(wizard2));


    wizardEnemyTmp->isColliding(*wizardEnemyTmp2);
    */

    sf::Texture &texture = m_TextureHolder.get(Textures::CHESS_WHITE);
    sf::IntRect textureRect(m_worldBounds);
    texture.setRepeated(true);
    std::unique_ptr<SpriteNode> background(new SpriteNode(texture, textureRect));
    m_sceneLayers[Layers::BACKGROUND]->attachChild(std::move(background));

    std::unique_ptr<Warrior> wizard(new Warrior(100, Textures::WIZARD, m_TextureHolder));
    SceneNode *wizardEnemyTmp = wizard.get();
    std::unique_ptr<CollisionShape> collisionShapeWizard(new CollisionCircle(12.f));
    wizard->setCollisionShape(std::move(collisionShapeWizard));
    //wizard->setPosition(800 / 2.f + 100.f, 480 / 2.f);
    wizard->setPosition(800 / 2.f - 60.f, 480 / 2.f - 10.f);
    wizard->setVelocity(30.f, 30.f);
    wizard->setType(WorldObjectTypes::ENEMY);
    m_sceneLayers[Layers::MAIN]->attachChild(std::move(wizard));


    std::unique_ptr<Warrior> warriorEnemy(new Warrior(100, Textures::KNIGHT, m_TextureHolder));
    SceneNode *warriorEnemyTmp = warriorEnemy.get();
    std::unique_ptr<CollisionShape> collisionShapeWarriorEnemy(new CollisionRect({ 32.f, 32.f }));
    warriorEnemy->setCollisionShape(std::move(collisionShapeWarriorEnemy));
    warriorEnemy->setPosition(800 / 3.f + 100.f, 480 / 3.f);
    warriorEnemy->setVelocity(30.f, 30.f);
    warriorEnemy->setType(WorldObjectTypes::ENEMY);
    m_sceneLayers[Layers::MAIN]->attachChild(std::move(warriorEnemy));


    std::unique_ptr<Warrior> warriorEnemy2(new Warrior(100, Textures::KNIGHT, m_TextureHolder));
    SceneNode *warriorEnemy2Tmp = warriorEnemy2.get();
    std::unique_ptr<CollisionShape> collisionShapeWarriorEnemy2(new CollisionRect({ 32.f, 32.f }));
    warriorEnemy2->setCollisionShape(std::move(collisionShapeWarriorEnemy2));
    warriorEnemy2->setPosition(800 / 3.f + 300.f, 480 / 3.f);
    warriorEnemy2->setVelocity(30.f, 30.f);
    warriorEnemy2->setType(WorldObjectTypes::NONE);
    m_sceneLayers[Layers::MAIN]->attachChild(std::move(warriorEnemy2));


    std::unique_ptr<Warrior> warriorEnemy3(new Warrior(100, Textures::KNIGHT, m_TextureHolder));
    SceneNode *warriorEnemy3Tmp = warriorEnemy3.get();
    std::unique_ptr<CollisionShape> collisionShapeWarriorEnemy3(new CollisionRect({ 32.f, 32.f }));
    warriorEnemy3->setCollisionShape(std::move(collisionShapeWarriorEnemy3));
    warriorEnemy3->setPosition(800 / 3.f + 300.f, 380);
    warriorEnemy3->setVelocity(30.f, 30.f);
    warriorEnemy3->setType(WorldObjectTypes::NONE);
    warriorEnemy3->setRotation(50.f);
    m_sceneLayers[Layers::MAIN]->attachChild(std::move(warriorEnemy3));



    std::unique_ptr<Warrior> warrior(new Warrior(100, Textures::KNIGHT, m_TextureHolder));
    m_playerWarrior = warrior.get();
    //std::unique_ptr<CollisionShape> collisionShapeWarrior(new CollisionCircle(12.f));
    std::unique_ptr<CollisionShape> collisionShapeWarrior(new CollisionRect({ 32.f, 32.f }));
    m_playerWarrior->setCollisionShape(std::move(collisionShapeWarrior));
    m_playerWarrior->setPosition(800 / 2.f, 480 / 2.f);
    m_playerWarrior->setVelocity(60.f, 60.f);
    m_playerWarrior->setType(WorldObjectTypes::PLAYER);
    m_sceneLayers[Layers::MAIN]->attachChild(std::move(warrior));

    // Collision Tests
    /*
    if (m_playerWarrior->getCollisionShape()->isColliding(*wizardEnemyTmp->getCollisionShape()))
    {
        std::cout << "Colliding" << std::endl;
    }
    */
    //m_playerWarrior->getCollisionShape()->isColliding(*warriorEnemyTmp->getCollisionShape());
    std::cout << "World::buildScene End" << std::endl;
}

void World::safeSceneNodeTrasform()
{
    m_sceneGraph.safeTransform();
}

void World::translateInput(Input input, float dt)
{
    switch (input.getInputType())
    {
        case InputTypes::CURSOR_POS :
        {
            m_commandQueue.push({ CommandTypes::LOOK_AT, WorldObjectTypes::PLAYER, input.getValues() });
            break;
        }
        case InputTypes::TRANSLATED_CURSOR_POS :
        {
            /*
            const sf::Vector2f UnitVecX(1.0, 0.f);
            // The angle which should be calculated have the coordinate systems midpoint at the center of the window,
            // so we have to use the translated mouse position so its relativ to the center of the window
            const sf::Vector2f TranslatedMousePos = { input.getValues() };
            const float Angle = { Calc::radToDeg(Calc::getVec2Angle<sf::Vector2f, sf::Vector2f>(UnitVecX, TranslatedMousePos)) };
            const float AngleSigned = TranslatedMousePos.y < 0.f ? -Angle : Angle;
            m_commandQueue.push({ CommandTypes::ROTATE, WorldObjectTypes::Player, { AngleSigned, 0.f } });
            */
            break;
        }
        case InputTypes::UP :
            m_commandQueue.push({ CommandTypes::MOVE_UP, WorldObjectTypes::PLAYER });
            break;
        case InputTypes::DOWN :
            m_commandQueue.push({ CommandTypes::MOVE_DOWN, WorldObjectTypes::PLAYER });
            break;
        case InputTypes::LEFT :
            m_commandQueue.push({ CommandTypes::MOVE_LEFT, WorldObjectTypes::PLAYER });
            break;
        case InputTypes::RIGHT :
            m_commandQueue.push({ CommandTypes::MOVE_RIGHT, WorldObjectTypes::PLAYER });
            break;
    }
    /*
    if ( != m_lastMousePos)
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
    }*/
}

// Tmp (better implemention later)
void World::controlWorldEntities()
{
    // Let Enemy look to player
    m_commandQueue.push({ CommandTypes::LOOK_AT, WorldObjectTypes::ENEMY, m_playerWarrior->getPosition() });
}

void World::handleCommands(float dt)
{
    while(!m_commandQueue.isEmpty())
    {
        m_sceneGraph.onCommand(m_commandQueue.pop(), dt);
    }
}

void World::update(float dt)
{
    m_sceneGraph.update(dt);
}

void World::handleCollision(float dt)
{
    // Store the collisionPairs, so we can later check if the same collision is already stored, so we only
    // store the Collision information once.
    std::set<SceneNode::Pair> collisionPairs;
    // Here are the collision information stored, which we use later and the affected SceneNodes
    std::vector<std::pair<SceneNode::Pair, CollisionInfo>> collisionData;

    m_sceneGraph.checkSceneCollision(m_sceneGraph, collisionPairs, collisionData);
    for (std::pair<SceneNode::Pair, CollisionInfo> collisionDataTmp : collisionData)
    {
        SceneNode::Pair sceneNodes = { collisionDataTmp.first };
        CollisionInfo collisionInfo = { collisionDataTmp.second };
        if (matchesCategories(sceneNodes, WorldObjectTypes::PLAYER, WorldObjectTypes::ENEMY))
        {
            std::cout << "Player Enemy Collision" << std::endl;
            //pairTmp.first->restoreLastTransform();
            //pairTmp.second->restoreLastTransform();
        }
        std::cout << "Collision: " << colCnt++ << std::endl;
    }
    /*
    for (SceneNode::Pair pairTmp : collisionPairs)
    {
        if (matchesCategories(pairTmp, WorldObjectTypes::PLAYER, WorldObjectTypes::ENEMY))
        {
            std::cout << "Player Enemy Collision" << std::endl;
            //pairTmp.first->restoreLastTransform();
            //pairTmp.second->restoreLastTransform();
        }
        std::cout << "Collision: " << colCnt++ << std::endl;
    }
    */
}
/*
bool World::matchesCategories(SceneNode::Pair &colliders, WorldObjectTypes worldObjectType1, WorldObjectTypes worldObjectType2)
{
    unsigned int category1 = static_cast<unsigned int>(colliders.first->getType());
    unsigned int category2 = static_cast<unsigned int>(colliders.second->getType());
    unsigned int type1 = static_cast<unsigned int>(worldObjectType1);
    unsigned int type2 = static_cast<unsigned int>(worldObjectType2);
    std::cout << "Cat1: " << category1 << " Cat2: " << category2 << " Type1: " << type1 << " Type2: " << type2 << std::endl;
    if (type1 & category1 && type2 & category2)
    {
        return true;
    }
    else if (type1 & category2 && type2 & category1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    return false;
}
*/
bool World::matchesCategories(SceneNode::Pair &colliders, WorldObjectTypes type1, WorldObjectTypes type2)
{
    WorldObjectTypes category1 = colliders.first->getType();
    WorldObjectTypes category2 = colliders.second->getType();

    //std::cout << "Cat1: " << category1 << " Cat2: " << category2 << " Type1: " << type1 << " Type2: " << type2 << std::endl;
    if (type1 == category1 && type2 == category2)
    {
        return true;
    }
    else if (type1 == category2 && type2 == category1)
    {
        std::swap(colliders.first, colliders.second);
        return true;
    }
    return false;
}

void World::render()
{
    //std::cout << "Render" << std::endl;
    //m_window->clear();
    m_window->draw(m_sceneGraph);
    //m_window->display();
}

