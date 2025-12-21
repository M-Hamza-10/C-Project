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
    
    
    map* Map;
    std::vector<PowerData> powerup;
    sf::Texture powerTex;

    sf::Texture smokeTex;
    sf::Sprite smoke;

    sf::SoundBuffer monstersound;
    sf::Sound monsterS;
    

    sf::Clock spawnClock;
    float spawnInterval = 5.0f;
    float speed;
    float maxWorldZ = 500.f;

    //Smoke Animation
    int frameCount = 8; //8
    int currentFrame = 0;
    float animTimer = 0.f;
    float animSpeed = 0.1f; //0.5
    float frameWidth = 241.f;//add
    int frameHeight = 460;//add
    sf::FloatRect pb;
      //Animation Variables light
    int frameCount2 = 10; //6
    float animTimer2 = 0.f;
    int currentFrame2 = 0;
    float maxtimer = 0.f;
    float animSpeed2 = 0.4f;
   

    public:
    //constructor
    Powers(int winWidth, int winHeight, map* Map);
    //variables
    float roadLeft  = -250.f;
    float roadRight = 250.f;
    float spawnZMin = 200.f;
    float spawnZMax = 400.f;
     //Public for game constructor to change values
    float frameWidth2 = 128.f;//add
    int frameHeight2 = 383;//add
    
    //Basket Monster Trap Smoke
    bool smoked = false;
    bool lighted = false;
    //Public as they are to be declared in game.cpp
    sf::Texture lighttex;
    sf::Sprite light;

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