#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Network.hpp>
#include "map.h"


class Player
{
    private:
      // Player Addition
    sf::Texture playerTex;
    sf::Sprite player;
    // sf::Texture player2Tex;
    // sf::Sprite player2;
    map *Map;

    sf::Keyboard::Key moveLeftKey;
    sf::Keyboard::Key moveRightKey;

    int health;
    int score;
    float playerX ;   // horizontal offset relative to center
    float playerZ ; // distance forward (depth)
    float playerSpeed;

    //Player blinker
    bool isInvincible = false;
    float invincibleTimer = 0.f;
    float invincibleDuration = 3.0f; // seconds

    float blinkTimer = 0.f;
    float blinkInterval = 0.1f; // blink speed
    bool visible = true;

 

    // float player2X = playerX - 3.0f;   // horizontal offset relative to center
    // float player2Z = 160.f;

    // Animation
 

    int winWidth;
    int winHeight;
    float frameWidth ; //93
    int frameHeight; //178
    int frameCount ;
    int currentFrame = 0;
    float animTimer = 0.f;
    float animSpeed = 0.08f; // smaller = faster
    int moveSpeed;

    // int frame2Height = 267;
    // float frame2Width = 128;
    // int frameCount2 = 11;
    // int currentFrame2 = 0;
    // float animTimer2 = 0.f;
    // float animSpeed2 = 0.08f;

    void animatePlayer(float dt);
    // void animatePlayer2(float dt);

    sf::Vector2f projectPlayer(float worldZ, float offsetX);

    public:
      int playerID;
      //Each player constructed w.r.t its frame width and height
       Player(int winWidth,
           int winHeight,
           map* Map,
           const std::string& spritePath,
           int frameWidth,
           int frameHeight,
           int frames,
           float startZ,
           sf::Keyboard::Key LeftKey,
          sf::Keyboard::Key RightKey,int ID);

    
    void update(float dt);
    void draw(sf::RenderWindow &window);

    int getHealth() const;
    int getScore() const;
    void takeDamage(int dmg);
    float getWorldZ() const;
    void addScore(int value);
    bool Invincible() const;
    sf::FloatRect getBounds() const;

};