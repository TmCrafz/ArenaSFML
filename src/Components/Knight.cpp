#include "Components/Knight.hpp"
#include "Components/Weapon.hpp"
#include "Components/Item.hpp"
#include "Collision/CollisionRect.hpp"
#include "Collision/CollisionHandler.hpp"
#include <iostream>

Knight::Knight(RenderLayers layer, const int health, Textures textureId, 
        const ResourceHolder<sf::Texture, Textures> &textureHolder,
        const SpriteSheetMapHolder &spriteSheetMapHolder, 
        std::vector<Warrior*> &possibleTargetsInWord)
: Warrior(layer, health, textureId, textureHolder, spriteSheetMapHolder, 
        possibleTargetsInWord)
, m_animCloseAttack( nullptr, false )
, m_animStrongAttack( nullptr, false )
, m_closeAttackStanima{ 10.f }
, m_closeAttackDamageMul{ 1.f }
, m_strongAttackStanima{ 30.f }
, m_strongAttackDamageMul{ 3.f }
{
    // Animation
    std::vector<AnimationStepRotation>  swordRoationStepsCloseAtt;
    swordRoationStepsCloseAtt.push_back({ 0.f, -60.f,  0.3f });
    m_animCloseAttack.setRotationSteps(swordRoationStepsCloseAtt);
    
    
    std::vector<AnimationStepMovement>  swordMovementStepsStrongAtt;
    swordMovementStepsStrongAtt.push_back({ -5.f, { 1, 0 },  0.3f });
    swordMovementStepsStrongAtt.push_back({ 5.f, { 1, 0 },  0.3f });
    m_animStrongAttack.setMovementSteps(swordMovementStepsStrongAtt);
    
    std::unique_ptr<Weapon> sword(new Weapon(RenderLayers::WEAPON, 60.f, 
                textureHolder.get(textureId), 
                spriteSheetMapHolder.getRectData(textureId, "sword")));
    sword->setType(WorldObjectTypes::WEAPON);
    sword->setPosition(0.f, 0.f);
    //swordPlayer->setRotationPoint(0.f, swordPlayer->getSpriteHeight() / 2.f);
    sword->setOrigin(-10.f, 0.f);
    std::unique_ptr<CollisionShape> collisionShapeSword(new CollisionRect(
                { sword->getSpriteWidth(), sword->getSpriteHeight() }));
    sword->setCollisionShape(std::move(collisionShapeSword));
    setWeapon(sword.get());
    attachChild(std::move(sword));
    

    std::unique_ptr<Item> shield(new Item(RenderLayers::WEAPON, 
                textureHolder.get(textureId), 
                spriteSheetMapHolder.getRectData(textureId, "shield")));
    shield->setType(WorldObjectTypes::SHIELD);
    //shield->setPosition(0.f, 0.f);
    shield->setOrigin(0.f, shield->getSpriteHeight() / 2.f);
    //shield->setEquipPoint(0.f, shield->getSpriteHeight() / 2.f);
    std::unique_ptr<CollisionShape> collisionShapeShield(
            new CollisionRect({ shield->getSpriteWidth(), 
                shield->getSpriteHeight() }));
    shield->setCollisionShape(std::move(collisionShapeShield));
    //setWeapon(sword.get());
    // Shield pos
    m_shieldEquipPos = sf::Vector2f{ 0.f, -getHeight() / 2.f + 3.f };
    //shield->equip(m_shieldEquipPos);
    shield->setPosition(m_shieldEquipPos);
    m_shield = shield.get();
    attachChild(std::move(shield));

    std::unique_ptr<CollisionShape> closeCombatArea(new CollisionCircle(9.f));
    m_closeCombatArea = std::move(closeCombatArea);
    m_closeCombatArea->setParent(this);
    m_closeCombatArea->setPosition(9.f, 0.f);
    m_closeCombatArea->setDraw(true);
    /*
    std::vector<AnimationStepMovement>  swordMovementSteps;
    swordMovementSteps.push_back({ sf::Vector2f(0.f, 0.f), 100.f, sf::Vector2f(10.f, 0.f) , 1.5f });
    m_animationSword.setMovementSteps(swordMovementSteps);
    */
}

Knight::~Knight()
{
    std::cout << "Destructor Knight" << std::endl;
}

void Knight::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    Warrior::drawCurrent(target, states);
}

void Knight::updateCurrent(float dt)
{
    Warrior::updateCurrent(dt);
    if (m_weapon)
    {
        if (m_animCloseAttack.isRunning())
        {
            m_animCloseAttack.update(dt);
        }
        else if (m_animStrongAttack.isRunning())
        {
            m_animStrongAttack.update(dt);
        }
        else
        {
            m_weapon->setIsCollisionCheckOn(false);
        }
    }
}

void Knight::onCommandCurrent(const Command &command, float dt)
{
    // Do command handling of parent class
    //Entity::onCommand(command, dt);
    Warrior::onCommandCurrent(command, dt);
    if (command.getWorldObjectType() & m_type)
    {
        m_currentVelocity = 0.f;
        m_currentDirection.x = 0.f;
        m_currentDirection.y = 0.f;
        switch (command.getCommandType())
        {
            case CommandTypes::ATTACK1:
                startCloseAttack();
                break;
            case CommandTypes::ATTACK2:
                startStrongAttack();
                break;
            case CommandTypes::START_BLOCKING:
                startBlocking();
                break;
            case CommandTypes::STOP_BLOCKING:
                stopBlocking();
                break;
            default:
                break;

        }

        // Move is the same as setPosition(getPosition() + offset) of the sf::Transformable class
        //move(m_currentVelocity * dt);
        //moveInDirection(m_currentDirection, m_currentVelocity * dt);
        moveInActualDirection(m_currentVelocity * dt);
    }
}

void Knight::updateAI(float dt)
{
    m_isMoving = false;
    Warrior::updateAI(dt);
    if (!m_actualTarget)
    {
        return;
    }

    lookAt(m_actualTarget->getPosition());
    CollisionInfo collisionInfo = m_closeCombatArea->isColliding(*m_actualTarget->getCollisionShape());
    if (collisionInfo.isCollision())
    {
        startCloseAttack();
    }
    else
    {
        // Follow target
        m_currentVelocity = m_velocity;
        m_currentDirection = m_actualTarget->getWorldPosition() - getWorldPosition();
        m_isMoving = true;
        moveInActualDirection(m_currentVelocity * dt);
    }
}

void Knight::startCloseAttack()
{
    if (m_weapon != nullptr)
    {
        std::cout << "Weapon is not null\n";
    }
    if (m_weapon && !m_animCloseAttack.isRunning() && 
            m_currentStamina >= m_closeAttackStanima)
    {
        m_animCloseAttack.start();
        m_weapon->setIsCollisionCheckOn(true);
        m_weapon->setDamageMultiplicator(m_closeAttackDamageMul);
        removeStanima(m_closeAttackStanima);
    }
}

void Knight::startStrongAttack()
{
    if (m_weapon && !m_animStrongAttack.isRunning() && 
            m_currentStamina >= m_strongAttackStanima)
    {
        m_weapon->setRotation(0.f);
        m_animStrongAttack.start();
        m_weapon->setIsCollisionCheckOn(true);
        m_weapon->setDamageMultiplicator(m_strongAttackDamageMul);
        removeStanima(m_strongAttackStanima);
    }
}

void Knight::startBlocking()
{
    m_animCloseAttack.stop();
    // Knight can only block with a weapon
    /*
    if (m_weapon && !m_isBlocking)
    {
        m_isBlocking = true;
        m_weapon->setRotation(-30.f);
    }
    */
    if (m_shield && !m_isBlocking)
    {
        m_isBlocking = true;
        m_shield->setRotation(270.f);
        m_shield->setPosition(10.f, 0.f);
    }
}

void Knight::stopBlocking()
{
    m_isBlocking = false;
    /*
    if (m_weapon)
    {
        m_weapon->setRotation(0.f);
    }
    */
    if (m_weapon)
    {
        m_shield->setRotation(0.f);
        m_shield->setPosition(m_shieldEquipPos);
    }
}

void Knight::weaponAdded()
{
    m_animCloseAttack.setParent(m_weapon);
    m_animStrongAttack.setParent(m_weapon);
}
