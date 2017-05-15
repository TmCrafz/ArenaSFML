#include "Components/Wizard.hpp"
#include "Components/Weapon.hpp"
#include "Components/Item.hpp"
#include "Collision/CollisionRect.hpp"
#include "Collision/CollisionHandler.hpp"
#include "Calc.hpp"
#include <iostream>

Wizard::Wizard(RenderLayers layer, const int health, Textures textureId, 
        const ResourceHolder<sf::Texture, Textures> &textureHolder,
        const SpriteSheetMapHolder &spriteSheetMapHolder, 
        std::vector<Warrior*> &possibleTargetsInWord)
: Warrior(layer, health, textureId, textureHolder, spriteSheetMapHolder, 
        possibleTargetsInWord)

, m_animFireballAttack( nullptr, false )
//, m_animStrongAttack( nullptr, false )
// Close Attack
, m_fireballAttackStanima{ 10.f }
, m_fireballDamage{ 15.f }
, m_isHealing{ false }
, m_healRestoreRate{ 10.f }
, m_healStanimaRate{ 40.f }
/*
, m_closeAttackDamageMul{ 1.f }
// Round Attack
, m_isRoundAttacking{ false }
, m_roundAttackStanima{ 40.f }
, m_roundAttackDamageMul{ 1.5f }
, m_roundAttackCurRot{ 0.f }
, m_roundAttackTotalRot{ 360.f }
, m_roundAttackAngleVel{ 720.f }
, m_startRotationRoundAttack{ 0.f }
//, m_strongAttackStanima{ 30.f }
//, m_strongAttackDamageMul{ 3.f }
, m_isDodging{ false }
, m_dodgeStanima{ 30.f }
, m_dodgeVelocity{ 300.f }
, m_totalDodgeTime{ 0.2 }
, m_curDodgeTime{ 0.f }
*/
{
    setVelocity(70.f);
    // Animation
    std::vector<AnimationStepMovement>  stickMovementStepsFireball;
    stickMovementStepsFireball.push_back({ -2.f, { 0, 1 },  0.1f });
    stickMovementStepsFireball.push_back({ 2.f, { 0, 1 },  0.1f });
    m_animFireballAttack.setMovementSteps(stickMovementStepsFireball);
    /*
    std::vector<AnimationStepRotation>  swordRoationStepsCloseAtt;
    swordRoationStepsCloseAtt.push_back({ 0.f, -60.f,  0.3f });
    m_animFireballAttack.setRotationSteps(swordRoationStepsCloseAtt);
    */
    std::unique_ptr<Weapon> stick(new Weapon(RenderLayers::WEAPON, 10.f, 
                textureHolder.get(textureId), 
                spriteSheetMapHolder.getRectData(textureId, "stick")));
    stick->setType(WorldObjectTypes::WEAPON);
    stick->setPosition(0.f, 0.f);
    stick->setOrigin(0.f, -10.f);
    std::unique_ptr<CollisionShape> collisionShapeSword(new CollisionRect(
                { stick->getSpriteWidth(), stick->getSpriteHeight() }));
    stick->setCollisionShape(std::move(collisionShapeSword));
    setWeapon(stick.get());
    attachChild(std::move(stick));

    std::unique_ptr<CollisionShape> closeCombatArea(new CollisionCircle(9.f));
    m_closeCombatArea = std::move(closeCombatArea);
    m_closeCombatArea->setParent(this);
    m_closeCombatArea->setPosition(9.f, 0.f);
    m_closeCombatArea->setDraw(true);
    // Load frame rects of fireball
    m_fireballFrameRects.push_back(
        m_spriteSheetMapHolder.getRectData(Textures::FIREBALL, "fireball_1"));
    m_fireballFrameRects.push_back(
        m_spriteSheetMapHolder.getRectData(Textures::FIREBALL, "fireball_2"));
    m_fireballFrameRects.push_back(
        m_spriteSheetMapHolder.getRectData(Textures::FIREBALL, "fireball_3"));
    m_fireballFrameRects.push_back(
        m_spriteSheetMapHolder.getRectData(Textures::FIREBALL, "fireball_4"));
    m_fireballFrameRects.push_back(
        m_spriteSheetMapHolder.getRectData(Textures::FIREBALL, "fireball_5"));
    m_fireballFrameRects.push_back(
        m_spriteSheetMapHolder.getRectData(Textures::FIREBALL, "fireball_6"));
}

Wizard::~Wizard()
{

}

void Wizard::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    Warrior::drawCurrent(target, states);
}

void Wizard::updateCurrent(float dt)
{
    Warrior::updateCurrent(dt);
    if (m_animFireballAttack.isRunning())
    {
        m_animFireballAttack.update(dt);
    }
    if (m_isHealing)
    {
        float healVal{ m_healRestoreRate * dt };
        float removeStanimaVal{ m_healStanimaRate * dt };
        float currentStanima{ getCurrentStanima() };
        float newStanima{ currentStanima - removeStanimaVal };
        // If warrior has not enough stanima, only restore as much health as 
        // stanima is there
        if (newStanima < 0.f)
        {
            healVal = currentStanima / m_healRestoreRate;
            removeStanimaVal = currentStanima;
        }        
        removeStanima(removeStanimaVal);
        heal(healVal);
    }

    /*
    if(m_isDodging) 
    {
        m_curDodgeTime += dt;
        moveInDirection(m_dodgeDir, m_dodgeVelocity * dt);
        if (m_curDodgeTime > m_totalDodgeTime)
        {
            stopDodging();
        }
    }
    else if(m_isRoundAttacking) 
    {
        m_roundAttackCurRot += m_roundAttackAngleVel * dt;
        setRotation(m_startRotationRoundAttack + m_roundAttackCurRot);
        if (m_roundAttackCurRot >= m_roundAttackTotalRot)
        {
            stopRoundAttack();
        }
    }
    else
    {
        Warrior::updateCurrent(dt);
        if (m_weapon)
        {
            if (m_animCloseAttack.isRunning())
            {
                m_animCloseAttack.update(dt);
            }
            else
            {
                m_weapon->setIsCollisionCheckOn(false);
            }
        }
    }
    */
}

void Wizard::onCommandCurrent(const Command &command, float dt)
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
            case CommandTypes::ACTION1:
                startFireballAttack();
                break;
            case CommandTypes::ACTION2:
                break;
            case CommandTypes::ACTION3:
                //startRoundAttack();
                break;
            // Not really blocking, but the same key
            case CommandTypes::START_BLOCKING:
                startHealing();
                break;
            case CommandTypes::STOP_BLOCKING:
                stopHealing();
                break;
            /*
            case CommandTypes::START_BLOCKING:
                startBlocking();
                break;
            case CommandTypes::STOP_BLOCKING:
                stopBlocking();
                break;
            */
            default:
                break;

        }

        // Move is the same as setPosition(getPosition() + offset) of the sf::Transformable class
        //move(m_currentVelocity * dt);
        //moveInDirection(m_currentDirection, m_currentVelocity * dt);
        moveInActualDirection(m_currentVelocity * dt);
    }
}

void Wizard::updateAI(float dt)
{
    /*
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
    */
}

void Wizard::startFireballAttack()
{
    if (m_weapon && !m_animFireballAttack.isRunning() &&  
            m_currentStamina >= m_fireballAttackStanima && !m_isHealing)
    {
        m_animFireballAttack.start();
        SceneNode* rootNode{ getRootSceneNode() };

        std::unique_ptr<Weapon> fireball{ 
            std::make_unique<Weapon>(RenderLayers::WEAPON,
                    m_fireballDamage,
                    m_textureHolder.get(Textures::FIREBALL), 
                    m_fireballFrameRects, 
                    true,
                    0.5f) };
        
        // Add id of wizard to "HitID", so the weapon asume that the wizard was
        // already attacked, so the wizard is not damaged by colliding with fireball
        fireball->addHitID(getID());
        fireball->setDebugName("Fireball");
        fireball->setVelocity(200.f);
        fireball->setPosition(getWorldPosition());
        fireball->setRotationDefault(getRotation());
        fireball->setCurrentDirection(
                Calc::degAngleToDirectionVector(fireball->getRotation() + 90.f));
        fireball->setType(WorldObjectTypes::WEAPON | WorldObjectTypes::PROJECTILE);
        std::unique_ptr<CollisionShape> collisionShape{
            std::make_unique<CollisionRect>(
                    sf::Vector2f(fireball->getWidth(), fireball->getHeight()))};
        fireball->setCollisionShape(std::move(collisionShape));
        fireball->setIsCollisionCheckOn(true);
        rootNode->attachChild(std::move(fireball));
        removeStanima(m_fireballAttackStanima);
    }
    /*
    if (m_weapon && !m_animCloseAttack.isRunning() && 
            m_currentStamina >= m_closeAttackStanima)
    {
        m_animCloseAttack.start();
        m_weapon->setIsCollisionCheckOn(true);
        m_weapon->setDamageMultiplicator(m_closeAttackDamageMul);
        m_weapon->startNewAttack();
        removeStanima(m_closeAttackStanima);
    }
    */
}

void Wizard::startHealing()
{
    m_isHealing = true;
}

void Wizard::stopHealing()
{
    m_isHealing = false;
}

/*
void Wizard::startCloseAttack()
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
        m_weapon->startNewAttack();
        removeStanima(m_closeAttackStanima);
    }
}
void Wizard::startCloseAttack()
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
        m_weapon->startNewAttack();
        removeStanima(m_closeAttackStanima);
    }
}

void Wizard::startRoundAttack()
{
    if (!m_isDodging && !m_isRoundAttacking && 
            m_currentStamina >= m_roundAttackStanima && m_weapon)
    {
        m_isRoundAttacking = true;
        m_roundAttackCurRot = 0.f;
        m_startRotationRoundAttack = getRotation();
        m_weapon->setRotation(270.f);
        m_weapon->setIsCollisionCheckOn(true);
        m_weapon->setDamageMultiplicator(m_roundAttackDamageMul);
        m_weapon->startNewAttack();
        removeStanima(m_roundAttackStanima);
    }
}

void Wizard::stopRoundAttack()
{
    m_isRoundAttacking = false;
    m_weapon->setRotation(0.f);
    m_weapon->setIsCollisionCheckOn(false);
}

void Wizard::startDodging()
{
    if (!m_isRoundAttacking && m_currentStamina >= m_dodgeStanima)
    {
        makeTransparent();
        setIsCollisionCheckOn(false);
        m_isDodging = true;
        m_curDodgeTime = 0.f;
        // The dodge direction is the direction where the runner looking at
        m_dodgeDir = Calc::degAngleToDirectionVector(getRotation());
        removeStanima(m_dodgeStanima);
    }
}

void Wizard::stopDodging()
{
    m_isDodging = false;
    setIsCollisionCheckOn(true);
    makeInTransparent();
}
*/
void Wizard::weaponAdded()
{
    m_animFireballAttack.setParent(m_weapon);
}
