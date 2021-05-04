#pragma once
#include "decs.h"
#include "ParticleComponent.h"
#include "PositionSystem.h"
#include <iostream>
#include <mutex>

class ParticleSystem : public decs::System<Particle>
{
public:
	ParticleSystem();
	~ParticleSystem();

    PositionSystem positionSystem;    

    sf::Vector2f m_emitter;



    void Create(int amount, sf::Vector2f emitter)
    {
        if (emitter.x < 0 || emitter.x > 800)
        {
            return;
        }
        if (emitter.y < 0 || emitter.y > 600)
        {
            return;
        }

        Particle::emitter = emitter;
        for (int i = 0; i < amount; i++)
        {
            int entID = decs::World::createNewID();
            this->addComponentWithID(entID);
        }
    }

private:

};

ParticleSystem::ParticleSystem()
{

}

ParticleSystem::~ParticleSystem()
{
}