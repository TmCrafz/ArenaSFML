#include "Components/Weapon.hpp"
#include "Calc.hpp"
#include <iostream>
#include <cmath>
#include <Helpers.hpp>

Weapon::Weapon(RenderLayers layer, const float damage, Textures textureId, const ResourceHolder<sf::Texture, Textures> &textureHolder)
: Item(layer, textureId, textureHolder)
, m_damage{ damage }
, m_damageMultiplicator{ 1.f }
{

}


Weapon::Weapon(RenderLayers layer, const float damage, const sf::Texture &texture, const sf::IntRect &rect)
: Item(layer, texture, rect)
, m_damage{ damage }
, m_damageMultiplicator{ 1.f }
{

}

Weapon::~Weapon()
{

}

float Weapon::getTotalDamage() const
{
    return m_damage * m_damageMultiplicator;
}

void Weapon::setStandartDamage(const float damage)
{
    m_damage = damage;
}

void Weapon::setDamageMultiplicator(float multiplicator)
{
    m_damageMultiplicator = multiplicator;
}

const std::string& Weapon::getAttackID() const
{
    return m_attackID;
}

void Weapon::startNewAttack()
{
    // Create a random attack id
    m_attackID = Helpers::getRandomAlphaNumString(30);
}
