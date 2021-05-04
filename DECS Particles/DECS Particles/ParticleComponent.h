#pragma once
#include "decs.h"
#include <SFML/Graphics.hpp>
#include "PositionSystem.h"
#include "SpriteSystem.h"
#include "PhysicsSystem.h"

class Particle : public decs::Component
{
public:
    Particle();
    ~Particle();

    PositionSystem positionSystem;
    PhysicsSystem physicsSystem;
    SpriteSystem spriteSystem;

    static sf::Vector2f emitter;

    void initialise() override 
    {  
        // give a random velocity and lifetime to the particle
        lifetime = (1.0f + 1) + (((float)rand()) / (float)RAND_MAX) * (2.0f - (1.0f + 1));

        positionSystem.addComponentWithID(belongsTo);
        positionSystem.getComponentWithID(belongsTo).position = emitter;

        spriteSystem.addComponentWithID(belongsTo);
        physicsSystem.addComponentWithID(belongsTo);


        float angle = (std::rand() % 360) * 3.14f / 180.f;
        float speed = (std::rand() % 500) + 300;
        physicsSystem.getComponentWithID(belongsTo).velocity = 
            sf::Vector2f(std::cos(angle) * speed, std::sin(angle) * speed);
        
    }

    float lifetime;


    static sf::Time elapsed;

    void update() override
    {
        lifetime -= decs::World::getDeltaTime();

        if (lifetime <= 0)
        {
            decs::World::destroyEntity(belongsTo, true);
            return;
        }
        if (isActive() == false)
        {
            decs::World::destroyEntity(belongsTo, true);
            return;
        }

        // Check particle out of bounds
        PositionComponent* pc = positionSystem.getPtrComponentWithID(belongsTo);
        if (pc->position.x < 0 || pc->position.x > 800)
        {
            decs::World::destroyEntity(belongsToID(), true);
            return;
        }
        if (pc->position.y > 600)
        {
            decs::World::destroyEntity(belongsToID(), true);
            return;
        }
    }

};

Particle::Particle()
{

}

Particle::~Particle()
{
}

sf::Vector2f Particle::emitter;