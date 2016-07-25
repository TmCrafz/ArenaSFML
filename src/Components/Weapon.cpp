#include "Components/Weapon.hpp"
#include "Calc.hpp"
#include <iostream>

Weapon::Weapon(const int damage, Textures textureId, const ResourceHolder<sf::Texture, Textures> &textureHolder)
: m_damage { damage }
, m_sprite{ textureHolder.get(textureId) }
{
    sf::FloatRect bounds = m_sprite.getLocalBounds();
    m_sprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);
}

int Weapon::getDamage() const
{
    return m_damage;
}

void Weapon::setDamage(const int damage)
{
    m_damage = damage;
}

void Weapon::drawCurrent(sf::RenderTarget &target, sf::RenderStates states) const
{
    target.draw(m_sprite, states);
}



