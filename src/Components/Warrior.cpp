#include "Components/Warrior.hpp"
#include "Calc.hpp"
#include "Components/Weapon.hpp"
#include "Config/ConfigManager.hpp"
#include "Helpers.hpp"
#include <iostream>
#include <vector>

Warrior::Warrior(RenderLayers layer, const ConfigManager &config, 
        SoundPlayer &sound, 
        const float health, 
        const std::string &textureId, 
        const ResourceHolder<sf::Texture> &textureHolder,
    const SpriteSheetMapHolder &spriteSheetMapHolder, std::vector<Warrior*> &possibleTargetsInWord)
: Entity(layer)
, m_ID{ Helpers::createUniqueID(30) }
, m_sound{ sound }
, m_textureHolder{ textureHolder }
, m_spriteSheetMapHolder{ spriteSheetMapHolder }
, m_maxHealth{ health }
, m_currentHealth{ health }
, m_maxStamina{ 100.f }
, m_currentStamina{ 100.f }
, m_stanimaRefreshRate{ 5.f }
, m_isMoving{ false }
, m_isBlocking{ false }
, m_leftShoe{ nullptr }
, m_rightShoe{ nullptr }
, m_weapon{ nullptr }
, m_upperBody{ nullptr }
, m_weaponPos(-10.f, 0.f)
, m_animationLeftShoe{  nullptr ,true }
, m_animationRightShoe{  nullptr ,true }
, m_closeCombatArea{ nullptr }
, m_isAiActive{ false }
, m_possibleTargetsInWord{ possibleTargetsInWord }
, m_actualTarget{ nullptr }
{
    addType(WorldObjectTypes::WARRIOR);
    applyConfig(config);
    std::unique_ptr<SpriteNode> leftShoe =
        { std::make_unique<SpriteNode>(RenderLayers::SHOES, 
                textureHolder.get(textureId), 
                spriteSheetMapHolder.getRectData(textureId, "left_shoe"), true) };
    leftShoe->setPosition(-5.f, 8.f);
    std::unique_ptr<SpriteNode> rightShoe =
        { std::make_unique<SpriteNode>(RenderLayers::WEAPON, 
                textureHolder.get(textureId), 
                spriteSheetMapHolder.getRectData(textureId, "right_shoe"), true) };
    rightShoe->setPosition(5.f, 8.f);

    sf::IntRect upperBodyRect{ 
        spriteSheetMapHolder.getRectData(textureId, "upper_body") };
    std::unique_ptr<SpriteNode> upperBody =
        { std::make_unique<SpriteNode>(RenderLayers::UPPER_BODY, 
                textureHolder.get(textureId), 
                upperBodyRect, true) };
    setWidth(upperBodyRect.width);
    setHeight(upperBodyRect.height);

    // Start body animation
    setBodyParts(leftShoe.get(), rightShoe.get(), upperBody.get());

    attachChild(std::move(leftShoe));
    attachChild(std::move(rightShoe));
    attachChild(std::move(upperBody));
}

Warrior::~Warrior()
{

}

void Warrior::applyConfig(const ConfigManager &config)
{
    setVelocity(config.getFloat("velocity", 60.f));
    setMass(config.getFloat("mass", 80.f));
    m_maxHealth = config.getFloat("health", 100.f);
    setCurrentHealth(m_maxHealth);
    m_maxStamina = config.getFloat("stanima", 100.f);
    setCurrentStanima(m_maxStamina);
    m_stanimaRefreshRate = config.getFloat("stanima_refresh", 5.f);
}

const std::string& Warrior::getID()
{
    return m_ID;
}

float Warrior::getCurrentHealth() const
{
    return m_currentHealth;
}

void Warrior::setCurrentHealth(const float health)
{
    m_currentHealth = health;
}

float Warrior::getMaxHealth() const
{
    return m_maxHealth;
}

float Warrior::getCurrentStanima() const
{
    return m_currentStamina;
}

void Warrior::setCurrentStanima(const float stanima)
{
    m_currentStamina = stanima;
}

bool Warrior::isBlocking() const
{
    return m_isBlocking;
}

void Warrior::setBodyParts(SpriteNode *leftShoe, SpriteNode *rightShoe, SpriteNode *upperBody)
{
    m_leftShoe = leftShoe;
    m_rightShoe = rightShoe;
    m_upperBody = upperBody;
    m_animationLeftShoe.setParent(m_leftShoe);
    m_animationRightShoe.setParent(m_rightShoe);
    std::vector<AnimationStepMovement>  leftShoeMovementSteps;
    leftShoeMovementSteps.push_back({ 2.f, sf::Vector2f(0.f, 1.f) , 0.3f });
    leftShoeMovementSteps.push_back({ 2.f, sf::Vector2f(0.f, -1.f) , 0.3f });
    m_animationLeftShoe.setMovementSteps(leftShoeMovementSteps);
    m_animationLeftShoe.start();

    std::vector<AnimationStepMovement>  rightShoeMovementSteps;
    rightShoeMovementSteps.push_back({ 2.f, sf::Vector2f(0.f, 1.f) , 0.3f });
    rightShoeMovementSteps.push_back({ 2.f, sf::Vector2f(0.f, -1.f) , 0.3f });
    m_animationRightShoe.setMovementSteps(rightShoeMovementSteps);
    m_animationRightShoe.setStartTime(0.3f);
    m_animationRightShoe.start();
}

void Warrior::setWeapon(Weapon *weapon)
{
    m_weapon = weapon;
    m_weapon->setRotationPoint(m_weaponPos);
    m_weapon->setPosition(m_weaponPos);
    weaponAdded();
}

Weapon* Warrior::getWeapon() const
{
    return m_weapon;
}

sf::Vector2f Warrior::getWorldWeaponPos() const
{
    return getWorldTransform() * m_weaponPos;
}

void Warrior::setIsAiActive(bool isAiActive)
{
    m_isAiActive = isAiActive;
}

bool Warrior::isAlive() const
{
    return m_currentHealth > 0.f;
}

void Warrior::damage(const float damage)
{
    m_currentHealth -= damage;
    if (m_currentHealth <= 0.f)
    {
        m_status = WorldObjectStatus::DESTORYED;
    }

}

void Warrior::heal(const float health)
{
    m_currentHealth += health;
    if (m_currentHealth > m_maxHealth)
    {
        m_currentHealth = m_maxHealth;
    }
}

void Warrior::removeStanima(float stanima)
{
    m_currentStamina -= stanima;
    if (m_currentStamina < 0.f)
    {
        m_currentStamina = 0.f;
    }
}

void Warrior::addStanima(float stanima)
{
    m_currentStamina += stanima;
    if (m_currentStamina > m_maxStamina)
    {
        m_currentStamina = m_maxStamina;
    }
}

void Warrior::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{

}

void Warrior::updateCurrent(float dt)
{
    // Automatic fill up Stanima with the given rate
    addStanima(m_stanimaRefreshRate * dt);
    if (m_isAiActive)
    {
        updateAI(dt);
    }
    if (m_isMoving)
    {
        if (m_leftShoe)
        {
            m_animationLeftShoe.update(dt);
        }
        if (m_rightShoe)
        {
            m_animationRightShoe.update(dt);
        }
    }
    m_isMoving = false;
}

Warrior* Warrior::determineActualTarget() const
{
    if (m_possibleTargetsInWord.size() == 0)
    {
        return nullptr;
    }
    Warrior* nearestWar = nullptr;
    // Use very high value, so that the possible warrios are under this value
    float nearestDist = { 999999.f };
    for (std::size_t i = { 0 }; i != m_possibleTargetsInWord.size(); i++)
    {
        Warrior *warrior = { m_possibleTargetsInWord[i] };
        sf::Vector2f disVec = { getWorldPosition() - warrior->getWorldPosition() };
        float distance = Calc::getVec2Length<sf::Vector2f>(disVec);
        //std::cout << "dist: " << distance << std::endl;
        if (distance < nearestDist && warrior != this)
        {
            nearestDist = distance;
            nearestWar = warrior;
        }
    }
    return nearestWar;
}

void Warrior::updateAI(float dt)
{
    m_actualTarget = determineActualTarget();
}

void Warrior::onCommandCurrent(const Command &command, float dt)
{
    // Do command handling of parent class
    //Entity::onCommand(command, dt);
    if (command.getWorldObjectType() & m_type)
    {
        m_currentVelocity = 0.f;
        m_currentDirection.x = 0.f;
        m_currentDirection.y = 0.f;
        m_isMoving = false;

        switch (command.getCommandType())
        {
            case CommandTypes::LOOK_AT_ABSOLUTE :
                lookAt(command.getValues());
                break;
            case CommandTypes::LOOK_AT_RELATIVE :
                lookAt(getWorldPosition() + command.getValues());
                break;
            case CommandTypes::ROTATE :
                break;
            case CommandTypes::MOVE_IN_DIR:
                m_currentVelocity = m_velocity;
                m_currentDirection = command.getValues();
                m_isMoving = true;
                break;
            case CommandTypes::MOVE_UP:
                m_currentVelocity = m_velocity;
                m_currentDirection.y = -m_velocity;
                m_isMoving = true;
                break;
            case CommandTypes::MOVE_DOWN:
                m_currentVelocity = m_velocity;
                m_currentDirection.y = m_velocity;
                m_isMoving = true;
                break;
            case CommandTypes::MOVE_LEFT:
                m_currentVelocity = m_velocity;
                m_currentDirection.x = -m_velocity;
                m_isMoving = true;
                break;
            case CommandTypes::MOVE_RIGHT:
                m_currentVelocity = m_velocity;
                m_currentDirection.x = m_velocity;
                m_isMoving = true;
                break;
            case CommandTypes::MOVE_UP_LEFT:
                m_currentVelocity = m_velocity;
                m_currentDirection.x = -(m_velocity / 2.f);
                m_currentDirection.y = -(m_velocity / 2.f);
                m_isMoving = true;
                break;
            case CommandTypes::MOVE_UP_RIGHT:
                m_currentVelocity = m_velocity;
                m_currentDirection.x = (m_velocity / 2.f);
                m_currentDirection.y = -(m_velocity / 2.f);
                m_isMoving = true;
                break;
            case CommandTypes::MOVE_DOWN_LEFT:
                m_currentVelocity = m_velocity;
                m_currentDirection.x = -(m_velocity / 2.f);
                m_currentDirection.y = (m_velocity / 2.f);
                m_isMoving = true;
                break;
            case CommandTypes::MOVE_DOWN_RIGHT:
                m_currentVelocity = m_velocity;
                m_currentDirection.x = (m_velocity / 2.f);
                m_currentDirection.y = (m_velocity / 2.f);
                m_isMoving = true;
                break;
            default:
                break;
        }
        moveInActualDirection(m_currentVelocity * dt);
    }
}

void Warrior::lookAt(const sf::Vector2f LookPos)
{
    // only look to the LookPos when Warrior is not at the same position
    if (LookPos.x == getWorldPosition().x && LookPos.y == getWorldPosition().y)
    {
        return;
    }
    const sf::Vector2f UnitVecX(1.0, 0.f);
    // The angle which should be calculated have the coordinate systems midpoint at the players position,
    // so we have to translate the pos in relation to the player position
    const sf::Vector2f TranslatedLookPos = LookPos - getWorldPosition();
    const float Angle = { Calc::radToDeg(Calc::getVec2Angle<sf::Vector2f, sf::Vector2f>(UnitVecX, TranslatedLookPos)) };
    const float AngleSigned = TranslatedLookPos.y < 0.f ? -Angle : Angle;
    setRotation(AngleSigned - 90.f);
}

void Warrior::weaponAdded()
{
    // Do nothing by default
}


void Warrior::handleDamage(Weapon *weapon)
{
    damage(weapon->getTotalDamage());
    // To prevent multiple damage set attack to blocked attacks
    weapon->addHitID(getID());
}
