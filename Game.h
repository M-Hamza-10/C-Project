#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Network.hpp>
#include <iostream>
#include "map.h"
#include "player.h"
#include "obstacles.h"
#include "Powerup.h"

enum class GameState
{
    Start,
    Playing,
    GameOver
};
class Game
{
    private:

    //inlcudes the object of class map
    map *Map;
    Player *player1;
    Player *player2;
    obstacles* Obstacle;
    Powers* power;
    PowerType  icon;
    
    sf::RenderWindow* Window;
    sf::Event event;
    sf::Clock deltaClock;
    
    // Health bars
    sf::RectangleShape p1HealthBack;
    sf::RectangleShape p1HealthFront;
    //Front rectangle represents current health while back one represents total health
    sf::RectangleShape p2HealthBack;
    sf::RectangleShape p2HealthFront;


    // sf::VideoMode videomode;
    
    void initVariables();
    void initWindow();
    void initMap();
    void initPlayer1();
    void initobstacles();
    void initPowerup();
    void applyPowerUp(PowerType type, Player& target , int id , float dt);

   

    public:

    GameState state = GameState::Start;
    //Monster Deploy Variables
    float roadLeft  = -250.f;
    float roadRight = 250.f;
    float spawnZMin = 200.f;
    float spawnZMax = 400.f;
    
    //constructors
    Game();
    //Destructor
    virtual ~Game();

    void update();
    void render();
    const bool running() const;
    void pollEvents();
    
    sf::Font font;

    sf::Text p1HealthText;
    sf::Text p2HealthText;
    sf::Text p1ScoreText;
    sf::Text p2ScoreText;
    sf::Text startText;
    sf::Text gameOverText;
    sf::Text winnertext;

    //sf::Music bgMusic; //No background music as of now
    sf::SoundBuffer lightBuffer;
    sf::SoundBuffer HealBuffer;
    sf::Sound lightS;
    sf::Sound HealS;

    sf::Texture Main;
    sf::Sprite menu;

    void initText();
    void initSound();


    //Variables
    const int windowWidth = 800;
    const int windowHeight = 600;

};