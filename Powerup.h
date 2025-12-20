#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Network.hpp>
#include "PowerType.h"
#include "player.h"
#include "map.h"

class Powers{
    private:

      struct PowerData
    {   
    sf::Sprite powerup;
    float worldX;
    float worldZ;

      PowerType type;

    //Animation variables
    int frameCount;
    int currentFrame = 0;
    float animTimer = 0.f;
    float animSpeed;
    int frameWidth;
    int frameHeight;
    
    };
    float smoketimer = 0.f;
    bool smoked = false;
    
    map* Map;
    std::vector<PowerData> powerup;
    sf::Texture powerTex;

    sf::Texture smokeTex;
    sf::Sprite smoke;
    sf::Clock spawnClock;
    float spawnInterval = 5.0f;
    float speed;
    float maxWorldZ = 500.f;

    //Smoke Animation
    int frameCount = 6; //6
    int currentFrame = 0;
    float animTimer = 0.f;
    float animSpeed = 0.5f;
    float frameWidth = 100.f;//add
    int frameHeight = 189;//add

    public:
    //constructor
    Powers(int winWidth, int winHeight, map* Map);
    //variables
    float roadLeft  = -250.f;
    float roadRight = 250.f;
    float spawnZMin = 200.f;
    float spawnZMax = 400.f;

    //Functions
    void randomSpawn();
    void spawnIcon(float worldX , float worldZ);
    void spawnMonster(float worldX , float worldZ);
    void animate(float dt);
    void animateSmoke(float dt);
    void checkPickup(Player& player , float dt);
    void update(float dt ,const int window);
    void draw(sf::RenderWindow& window);

};