#include "Powerup.h"
#include <iostream>
#include <Windows.h>

Powers::Powers(int winWidth, int winHeight, map *Map)
{
    this->Map = Map;
    speed = 120.f;

    if (!powerTex.loadFromFile("Sprites/Obstacles/Power_icon.png"))
        std::cout << "Could not load spike sprite sheet\n";

}

void Powers::randomSpawn()
{   
    
    if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval)
    {
        float randomX =
            roadLeft + static_cast<float>(rand()) / RAND_MAX *
            (roadRight - roadLeft);

        float randomZ =
            spawnZMin + static_cast<float>(rand()) / RAND_MAX *
            (spawnZMax - spawnZMin);

            spawnIcon(randomX+15 , randomZ);
        // int r = rand() % 3;
        // // if (r == 0)
        // //     spawnSpike(randomX, randomZ);
        // // else if(r == 1)
        // //     spawnSnake(randomX, randomZ);
        // // else if(r==2)
        // //     spawnConfusion(randomX,randomZ);

        spawnClock.restart();
    }
}

void Powers::spawnIcon(float worldX, float worldZ)
{
    PowerData Icon ;
    Icon.type = PowerType::icon;
    Icon.worldX = worldX;
    Icon.worldZ = worldZ;

    Icon.powerup.setTexture(powerTex);

    Icon.frameCount  = 6;
    Icon.frameWidth  = 194;
    Icon.frameHeight = 261;
    Icon.animSpeed   = 0.08f;

    Icon.powerup.setTextureRect(sf::IntRect(0, 0, Icon.frameWidth, Icon.frameHeight));

    Icon.powerup.setOrigin((Icon.frameWidth / 2) , Icon.frameHeight); // base on ground
    powerup.push_back(Icon);
}

void Powers::checkPickup(Player &player, float dt)
{

        float PlayerZ = player.getWorldZ();
        const float collisionRange = 30.f; // 

    for (auto &i : powerup)
    {   
        if(i.collected)
         continue;

        if (i.worldZ < PlayerZ)
        {
            continue;
        }
        float distancez = std::abs(i.worldZ - PlayerZ);

        if (distancez > collisionRange)//If the obstacle is too far away (in either direction), ignore it
        {
            continue;
        }

        // Check rectangle overlap
        if (i.powerup.getGlobalBounds().intersects(player.getBounds())){   
            player.addPowerUp(i.type);
            i.collected = true;   
        }
}
}

void Powers::update(float dt, const int window)
{
    //Randomly spawns the obstacle
    randomSpawn();

    //Moves the obstacle forward
    for (auto& s : powerup){
    // Move forward in WORLD space
    s.worldZ -= speed * dt;

    
    //animation of obstacles
          s.animTimer += dt; 
    if (s.animTimer >= s.animSpeed)
    {
        s.animTimer = 0.f;
        s.currentFrame = (s.currentFrame + 1) % s.frameCount;

        for (auto& s : powerup)
        {
            s.powerup.setTextureRect(sf::IntRect(
                s.currentFrame * s.frameWidth ,
                0,
                s.frameWidth,
                s.frameHeight));
        }
    }

    // Project every frame
    sf::Vector2f screenPos = Map->transformPerspective(
        Map->perspectiveX + s.worldX,
        s.worldZ);


        //scale based on depth
    s.powerup.setPosition(screenPos);

    float depth = s.worldZ / 20000.f;
    depth = std::clamp(depth, 0.5f, 0.5f);
    s.powerup.setScale(depth, depth);
    }
}

void Powers::draw(sf::RenderWindow &window)
{
    for (auto& s : powerup)
        window.draw(s.powerup);
}
