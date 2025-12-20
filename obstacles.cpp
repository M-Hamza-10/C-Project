#include "obstacles.h"
#include <windows.h>
#include <math.h>
#include <iostream>

obstacles::obstacles(int winWidth, int winHeight, map* Map)
{
    this->Map = Map;
    speed = 100.f;

    if (!spikeTex.loadFromFile("Sprites/Obstacles/Sprite.png"))
        std::cout << "Could not load spike sprite sheet\n";
    if (!snakeTex.loadFromFile("Sprites/Obstacles/Snake_2.png"))
    std::cout << "Could not load snake sprite sheet\n";
    if(!confusionTex.loadFromFile("Sprites/Obstacles/Confusion.png"))
    std::cout << "Could not load confusion obstacle\n";

}



void obstacles::spawnSpike(float worldX, float worldZ)
{
    ObstacleData spike ;
    spike.type = ObstacleType::Spike;
    spike.worldX = worldX;
    spike.worldZ = worldZ;

    spike.sprite.setTexture(spikeTex);

    spike.frameCount  = 6;
    spike.frameWidth  = 159;
    spike.frameHeight = 139;
    spike.animSpeed   = 0.08f;

    spike.sprite.setTextureRect(sf::IntRect(0, 0, spike.frameWidth, spike.frameHeight));

    spike.sprite.setOrigin((spike.frameWidth / 2 ) , spike.frameHeight); // base on ground
    sprite.push_back(spike);
}


void obstacles::spawnSnake(float worldX, float worldZ)
{
    ObstacleData snake;

    snake.type = ObstacleType::Snake;
    snake.worldX = worldX;
    snake.worldZ = worldZ;

    snake.sprite.setTexture(snakeTex);

    snake.frameCount  = 7;    // 7 frames
    snake.frameWidth  = 187;    
    snake.frameHeight = 707;   
    snake.animSpeed   = 0.08f;

    snake.sprite.setTextureRect(sf::IntRect(0, 0, snake.frameWidth, snake.frameHeight));
    snake.sprite.setOrigin((snake.frameWidth/2),snake.frameHeight);

    sprite.push_back(snake);
}

void obstacles::spawnConfusion(float worldX, float worldZ)
{
    ObstacleData confusion;
    confusion.type = ObstacleType::Confusing;
    confusion.worldX = worldX;
    confusion.worldZ = worldZ;

    confusion.sprite.setTexture(confusionTex);

    confusion.frameCount  = 8;    // 7 frames
    confusion.frameWidth  = 154;    
    confusion.frameHeight = 335;   
    confusion.animSpeed   = 0.08f;

    confusion.sprite.setTextureRect(sf::IntRect(0, 0, confusion.frameWidth, confusion.frameHeight));
    confusion.sprite.setOrigin((confusion.frameWidth/2),confusion.frameHeight);

    sprite.push_back(confusion);

}

void obstacles::collisonDetection(Player& player , float dt)
{   
        if(player.Invincible())
            return;
        float PlayerZ = player.getWorldZ();
        const float collisionRange = 30.f; // 

        for (auto i = sprite.begin(); i != sprite.end(); )
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
        if (i->sprite.getGlobalBounds().intersects(player.getBounds()))
        {   
            if(i->type == ObstacleType::Spike)
            player.takeDamage(5); //Take damage function in player.cpp

            else if(i-> type == ObstacleType::Snake){
            i->sprite.setScale(0.7f,0.7f);
            player.takeDamage(10);
            player.poison(dt);
            }
            else if(i->type == ObstacleType::Confusing){
                player.loseControl(dt);
            }            
            
            // Remove trap so it doesn't hit again
            i = sprite.erase(i);
            
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

    //Moves the obstacle forward
    for (auto& s : sprite)
    {
    // Move forward in WORLD space
    s.worldZ -= speed * dt;

   // Make the snake change lane continuously
    if (s.type == ObstacleType::Snake){
        s.moveTimer += dt;
        s.worldX += std::sin(s.moveTimer * s.moveSpeed) * s.moveAmplitude * dt;
    }
    else if(s.type == ObstacleType::Confusing){
        s.moveTimer += dt;
        s.worldX += std::tanh(s.moveTimer * s.moveSpeed) * s.moveAmplitude * dt;
    }
    else{
        s.moveTimer += dt;
        s.worldX += std::tan(s.moveTimer * s.moveSpeed) * s.moveAmplitude * dt;
    }
    
    //animation of obstacles
          s.animTimer += dt; 
    if (s.animTimer >= s.animSpeed)
    {
        s.animTimer = 0.f;
        s.currentFrame = (s.currentFrame + 1) % s.frameCount;

            s.sprite.setTextureRect(sf::IntRect(
                s.currentFrame * s.frameWidth ,
                0,
                s.frameWidth,
                s.frameHeight));
        
    }

    // Project every frame
    sf::Vector2f screenPos = Map->transformPerspective(
        Map->perspectiveX + s.worldX,
        s.worldZ);


        //scale based on depth
    s.sprite.setPosition(screenPos);

        if(s.type == ObstacleType::Snake){

        float depth = s.worldZ / 20000.f;
        depth = std::clamp(depth, 0.2f, 0.2f);
        s.sprite.setScale(depth, depth);

        }

        else if(s.type == ObstacleType::Spike){

        float depth = s.worldZ / 3000.f;
        depth = std::clamp(depth, 0.8f, 1.0f);
        s.sprite.setScale(depth, depth);
        }

        else if(s.type == ObstacleType::Confusing){

        float depth = s.worldZ / 5000.f;
        depth = std::clamp(depth, 0.5f, 0.5f);
        s.sprite.setScale(depth, depth);
        }
    
    
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
        int r = rand() % 3;
        if (r == 0)
            spawnSpike(randomX, randomZ);
        else if(r == 1)
            spawnSnake(randomX, randomZ);
        else if(r==2)
            spawnConfusion(randomX,randomZ);

        spawnClock.restart();
    }
}

void obstacles::draw(sf::RenderWindow& window)
{
    for (auto& s : sprite)
        window.draw(s.sprite);
}
