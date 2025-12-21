#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Network.hpp>
#include "map.h"
#include "PowerType.h"
#include <vector>


class Player
{
    private:
      // Player Addition
    sf::Texture playerTex;
    sf::Sprite player;
    sf::Texture lightning;
    sf::Sprite light;

    map *Map;

    sf::Keyboard::Key moveLeftKey;
    sf::Keyboard::Key moveRightKey;
    std::vector<PowerType> inventory;
    int health;
    int score;
    float playerX ;   // horizontal offset relative to center
    float playerZ ; // distance forward (depth)
    float playerSpeed;

    //Player blinker
    bool isInvincible = false;
    float invincibleTimer = 0.f;
    float invincibleDuration = 3.0f; // seconds
    bool poisoned = false;
    bool confused = false;
    float confuse_timer = 0.f;
    float poison_timer = 0.f;
    float blinkTimer = 0.f;
    float blinkInterval = 0.1f; // blink speed
    bool visible = true;

 

    // float player2X = playerX - 3.0f;   // horizontal offset relative to center
    // float player2Z = 160.f;
 
    
    int winWidth;
    int winHeight;
    float frameWidth ; //93
    int frameHeight; //178
    int frameCount ;
    int currentFrame = 0;
    float animTimer = 0.f;
    float animSpeed = 0.08f; // smaller = faster
    int moveSpeed;

    void animatePlayer(float dt);

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
          sf::Keyboard::Key RightKey,int ID
        );

    
    void update(float dt);
    void draw(sf::RenderWindow &window);
        //Health/score/collision
    int getHealth() const;
    int getScore() const;

    void takeDamage(int dmg);
    void addHealth(int life);
    void poison(float dt);
    void loseControl(float dt);

    float getWorldZ() const;
    void addScore(int value);
    bool Invincible() const;
    void addPowerUp(PowerType type);
    bool hasPowerUp() const;
    PowerType usePowerUp();
    sf::FloatRect getBounds() const;

};