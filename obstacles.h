#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "map.h"
#include "player.h"

enum class ObstacleType
{
    Spike,
    Snake,
    Confusing
};

class obstacles
{
private:
     struct ObstacleData
    {   
    sf::Sprite sprite;
    float worldX;
    float worldZ;

      ObstacleType type;

    //Animation variables
    int frameCount;
    int currentFrame = 0;
    float animTimer = 0.f;
    float animSpeed;
    int frameWidth;
    int frameHeight;

    // Trignometric functions data
    float moveTimer = 0.f;
    float moveAmplitude = 80.f;
    float moveSpeed = 3.f;
    
    };
   
    sf::Texture spikeTex;
    sf::Texture snakeTex;
    sf::Texture confusionTex;
    
    map* Map;
    std::vector<ObstacleData> sprite;
    

    sf::Clock spawnClock;
    float spawnInterval = 2.0f;
    float speed;
    float maxWorldZ = 500.f; 


public:
    float roadLeft  = -250.f;
    float roadRight = 250.f;
    float spawnZMin = 200.f;
    float spawnZMax = 400.f;
    
    obstacles(int winWidth, int winHeight, map* Map);
    void randomSpawn();

    void animate(float dt);

    void spawnSpike(float worldX, float worldZ);
    void spawnSnake(float worldX, float worldZ);
    void spawnConfusion(float worldX, float worldZ);
    void collisonDetection(Player& player , float dt);
    void update(float dt ,const int window);
    void draw(sf::RenderWindow& window);


};
