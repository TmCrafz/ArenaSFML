#ifndef KNIGHT_HPP
#define KNIGHT_HPP
#include <SFML/Graphics.hpp>
#include "Components/Warrior.hpp"
#include <iostream>

class Item;

class Knight : public Warrior
{
    private:
        Animation m_animCloseAttack;
        Animation m_animStrongAttack;
        
        sf::Vector2f m_shieldEquipPos;
        Item *m_shield;
        
        float m_closeAttackStanima;
        float m_closeAttackDamageMul;
        float m_strongAttackStanima;
        float m_strongAttackDamageMul;


    public:
        Knight(RenderLayers layer, const int health, Textures textureId, 
                const ResourceHolder<sf::Texture, Textures> &textureHolder,
                const SpriteSheetMapHolder &spriteSheetMapHolder, 
                std::vector<Warrior*> &possibleTargetsInWord);

        virtual ~Knight();

        virtual void drawCurrent(sf::RenderTarget &target, 
                sf::RenderStates states) const;
    protected:

        virtual void weaponAdded();

    private:
        virtual void updateCurrent(float dt);
        virtual void updateAI(float dt);
        virtual void onCommandCurrent(const Command &command, float dt);

        void startCloseAttack();
        void startStrongAttack();
        void startBlocking();
        void stopBlocking();
};

#endif // KNIGHT_HPP
