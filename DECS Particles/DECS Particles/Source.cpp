#pragma once
#include <SFML/Graphics.hpp>
#include "decs.h"
#include "ParticleSystem.h"
#include "PositionSystem.h"
#include "PhysicsSystem.h"
#include "SpriteSystem.h"

int main()
{
    // Declaration here works like an execution order

    // Same as PhysicsSystem physicsSystem; physicsSystem.setCanUpdate(true);
    decs::System<PhysicsComponent> phys;

    // Same as PositionSystem positionSystem; positionSystem.setCanUpdate(true);
    decs::System<PositionComponent> pos;

    ParticleSystem particleSystem;
    particleSystem.setCanUpdate(true);


    SpriteSystem spriteSystem;
    spriteSystem.setCanUpdate(false);


    sf::Font font;
    if (!font.loadFromFile("pressStart.ttf"))
    {
        std::cout << "Could not find font"<< std::endl;
        // error...
    }
    sf::Text entitiesCountText;
    entitiesCountText.setPosition(20, 20);
    sf::Text fpsText;
    fpsText.setPosition(40, 40);

    sf::Text highestID;
    highestID.setPosition(60, 60);

    // select the font
    entitiesCountText.setFont(font);
    fpsText.setFont(font);
    highestID.setFont(font);

    // set the string to display
    //entitiesCountText.setString("Entities: " + pSystem.entityManager.numberOfActiveComponents());

    // set the character size
    entitiesCountText.setCharacterSize(12);
    fpsText.setCharacterSize(12);
    highestID.setCharacterSize(12);

    // set the color
    entitiesCountText.setFillColor(sf::Color::Red);
    fpsText.setFillColor(sf::Color::Red);
    highestID.setFillColor(sf::Color::Red);


    // create the window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Particles");

    // create a clock to track the elapsed time
    sf::Clock clock;
    float lastTime = 0;
    float currentTime;

    window.setFramerateLimit(60);
    // run the main loop
    while (window.isOpen())
    {

        // handle events
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed)
            {
                // make the particle system emitter follow the mouse
                sf::Vector2i mouse = sf::Mouse::getPosition(window);
                particleSystem.Create(1000, window.mapPixelToCoords(mouse));
                break;
            }
            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::C)
                {
                }
            }
        }

        sf::Time elapsed = clock.restart();

        // update it
        decs::World::setDeltaTime(elapsed.asSeconds());
        decs::World::update();

        // set the string to display
        entitiesCountText.setString("Entities: " + std::to_string(particleSystem.getNumberOfActiveComponents()));
        fpsText.setString("FPS: " + std::to_string(1.0f / (elapsed.asSeconds())));
        highestID.setString("HighestID: " + std::to_string(decs::World::getNextAvailableEntityID()));
        
        // Rendering
        window.clear();
        spriteSystem.draw(window);
        window.draw(entitiesCountText);
        window.draw(fpsText);
        window.draw(highestID);
        window.display();
        lastTime = elapsed.asSeconds();
    }

    return 0;
}