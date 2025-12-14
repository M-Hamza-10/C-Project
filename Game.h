#pragma once
#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Network.hpp>
#include "map.h"
#include "player.h"
#include "obstacles.h"




class Game
{
    private:

    //inlcudes the object of class map
    map *Map;
    Player *player1;
    Player *player2;
    obstacles* Obstacle;

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

    public:
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

    void initText();


    //Variables
    const int windowWidth = 800;
    const int windowHeight = 600;

};