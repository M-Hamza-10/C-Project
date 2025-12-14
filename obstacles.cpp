#include "obstacles.h"
#include <windows.h>
#include <iostream>

obstacles::obstacles(int winWidth, int winHeight, map* Map)
{
    this->Map = Map;
    speed = 100.f;

    if (!spikeTex.loadFromFile("Sprites/Obstacles/Sprite.png"))
        std::cout << "Could not load spike sprite sheet\n";
}



void obstacles::spawnSpike(float worldX, float worldZ)
{
    ObstacleData spike ;

    spike.worldX = worldX;
    spike.worldZ = worldZ;

    spike.spikes.setTexture(spikeTex);
    spike.spikes.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));

    spike.spikes.setOrigin((frameWidth / 2 ) , frameHeight); // base on ground
    spikes.push_back(spike);
}

void obstacles::collisonDetection(Player& player)
{   
        if(player.Invincible())
            return;
        float PlayerZ = player.getWorldZ();
        const float collisionRange = 30.f; // 

        for (auto i = spikes.begin(); i != spikes.end(); )
    {
        if (i->worldZ < PlayerZ)
        {
            ++i;
            continue;
        }
        float distancez = std::abs(i->worldZ - PlayerZ);

        if (distancez > collisionRange)//If the obstacle is too far away (in either direction), ignore it
        {
            ++i;
            continue;
        }

        // Check rectangle overlap
        if (i->spikes.getGlobalBounds().intersects(player.getBounds()))
        {   
            // Damage player
            player.takeDamage(20);
            // Remove trap so it doesn't hit again
            i = spikes.erase(i);
            
        }
        else
        {
            ++i;
        }
    }
}

void obstacles::update(float dt ,const int winHeight)
{   //Randomly spawns the obstacle
    randomSpawn();
    //animates the obstacle
    animate(dt);

    //Moves the obstacle forward
    for (auto& s : spikes)
{
    // Move forward in WORLD space
    s.worldZ -= speed * dt;
    

    // Project every frame
    sf::Vector2f screenPos = Map->transformPerspective(
        Map->perspectiveX + s.worldX,
        s.worldZ);

    s.spikes.setPosition(screenPos);

    // Scale based on depth
    float depth = s.worldZ / 3000.f;
    depth = std::clamp(depth, 0.8f, 1.0f);
    s.spikes.setScale(depth, depth);
    }


}
void obstacles::randomSpawn()
{
    if (spawnClock.getElapsedTime().asSeconds() >= spawnInterval)
    {
        float randomX =
            roadLeft + static_cast<float>(rand()) / RAND_MAX *
            (roadRight - roadLeft);

        float randomZ =
            spawnZMin + static_cast<float>(rand()) / RAND_MAX *
            (spawnZMax - spawnZMin);

        spawnSpike(randomX, randomZ);
        spawnClock.restart();
    }
}

void obstacles::animate(float dt)
{
       animTimer += dt;
    if (animTimer >= animSpeed)
    {
        animTimer = 0.f;
        currentFrame = (currentFrame + 1) % frameCount;

        for (auto& s : spikes)
        {
            s.spikes.setTextureRect(sf::IntRect(
                currentFrame * frameWidth ,
                0,
                frameWidth,
                frameHeight));
        }
    }
}

void obstacles::draw(sf::RenderWindow& window)
{
    for (auto& s : spikes)
        window.draw(s.spikes);
}
