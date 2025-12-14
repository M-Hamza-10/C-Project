#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "map.h"
#include "player.h"


class obstacles
{
private:
     struct ObstacleData
    {
    sf::Sprite spikes;
    float worldX;
    float worldZ;
    
    };
   
    sf::Texture spikeTex;
    
    map* Map;
    std::vector<ObstacleData> spikes;
    

    sf::Clock spawnClock;
    float spawnInterval = 10.0f;
    float speed;
    float maxWorldZ = 500.f; 
    // Animation
    int frameCount = 6 , framecount2 = 6;
    int currentFrame = 0;
    float animTimer = 0.f;
    float animSpeed = 0.08f;
    int frameWidth = 159;   
    int frameHeight = 139;

public:
    float roadLeft  = -250.f;
    float roadRight = 250.f;
    float spawnZMin = 200.f;
    float spawnZMax = 400.f;
    obstacles(int winWidth, int winHeight, map* Map);
    void randomSpawn();
    void animate(float dt);
    void spawnSpike(float worldX, float worldZ);
    void collisonDetection(Player& player);
    void update(float dt ,const int window);
    void draw(sf::RenderWindow& window);


};
