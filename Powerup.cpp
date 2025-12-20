#include "Powerup.h"
#include "player.h"
#include <iostream>
#include <Windows.h>

Powers::Powers(int winWidth, int winHeight, map *Map)
{
    this->Map = Map;
    speed = 120.f;

    if (!powerTex.loadFromFile("Sprites/Obstacles/Power_icon.png"))
        std::cout << "Could not load spike sprite sheet\n";
    if(!smokeTex.loadFromFile("Sprites/Obstacles/SampleSmoke_2.png"))
        std::cout << "Could not load smoke\n";
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

        spawnClock.restart();
    }
}

void Powers::spawnIcon(float worldX, float worldZ)
{
    PowerData Icon ;
    Icon.worldX = worldX;
    Icon.worldZ = worldZ;

    Icon.powerup.setTexture(powerTex);

    Icon.frameCount  = 6;
    Icon.frameWidth  = 194;
    Icon.frameHeight = 261;
    Icon.animSpeed   = 0.08f;

    Icon.powerup.setTextureRect(sf::IntRect(0, 0, Icon.frameWidth, Icon.frameHeight));
    int random = rand() % 3;
    if(random == 0)
        Icon.type = PowerType::Monster;
    else if(random == 1)
        Icon.type = PowerType::lightning;
    else if(random == 2)
        Icon.type = PowerType::Heal;

    Icon.powerup.setOrigin((Icon.frameWidth / 2) , Icon.frameHeight); // base on ground
    powerup.push_back(Icon);
}

void Powers::spawnMonster(float worldX, float worldZ)
{
    PowerData Monster ;
    Monster.worldX = worldX;
    Monster.worldZ = worldZ;
    Monster.type = PowerType::MonsterApply;
    Monster.powerup.setTexture(powerTex);

    Monster.frameCount  = 6;
    Monster.frameWidth  = 194;
    Monster.frameHeight = 261;
    Monster.animSpeed   = 0.08f;

    Monster.powerup.setTextureRect(sf::IntRect(0, 0, Monster.frameWidth, Monster.frameHeight));

    Monster.powerup.setOrigin((Monster.frameWidth / 2) , Monster.frameHeight); // base on ground
    powerup.push_back(Monster);
}


void Powers::checkPickup(Player &player, float dt)
{

        float PlayerZ = player.getWorldZ();
        const float collisionRange = 30.f; // 

    for (auto i = powerup.begin(); i != powerup.end();)
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
        if (i->powerup.getGlobalBounds().intersects(player.getBounds()))
        {   
            if(i->type == PowerType::MonsterApply)
            {   
                player.takeDamage(20);
                smoked = true;
                smoketimer = 0.f;

                sf::FloatRect pb =player.getBounds();
                smoke.setTexture(smokeTex);
                smoke.setPosition((pb.left + pb.width)/2 + 20.f ,(pb.top + pb.height)/2);
                smoke.setScale(2.5f, 2.5f);
                
                i = powerup.erase(i);
                continue;
                
            }
                    // Player can only pick if inventory empty
            if (!player.hasPowerUp() && i->type != PowerType::MonsterApply)
            {
                player.addPowerUp(i->type);
                i = powerup.erase(i); 
                continue;
            }
   
        }
        ++i;
    }
}


void Powers::update(float dt, const int window)
{
    //Randomly spawns the obstacle
    randomSpawn();
    
    if (smoked)
    {   
        animTimer += dt;
    if (animTimer >= animSpeed)
    {
        animTimer = 0.f;
        currentFrame = (currentFrame + 1) % frameCount;

        smoke.setTextureRect(sf::IntRect(
            currentFrame * frameWidth,
            0,
            frameWidth,
            frameHeight)); 
    }
        smoketimer+=dt;
        if (smoketimer >= 2.6f)
        {
            smoked = false;
            smoketimer = 0.f;
            currentFrame = 0;
        }
    }

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


            s.powerup.setTextureRect(sf::IntRect(
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
    s.powerup.setPosition(screenPos);

    float depth = s.worldZ / 20000.f;
    depth = std::clamp(depth, 0.5f, 0.5f);
    s.powerup.setScale(depth, depth);
    }
}

void Powers::draw(sf::RenderWindow &window)
{   
    if(smoked)
        window.draw(smoke);
    for (auto& s : powerup)
        window.draw(s.powerup);
}
