#include "Game.h"




Game::Game()
{
    this->initWindow();
    this->initMap();
    this->initPlayer1();
    this->initobstacles();
    this->initText();
    this->initPowerup();
    
    srand(static_cast<unsigned>(time(nullptr)));

}
Game::~Game()
{
    delete this ->Window;
    delete this->Map;
    delete this->player1;
    delete this->player2;
    delete this->Obstacle;
    delete this->power;
}

void Game::initWindow()
{   
    //keyword new to store the window in heap so it never gets destoyed
    this->Window = new sf::RenderWindow(sf::VideoMode(windowHeight,windowWidth), "Race Game",sf::Style::Close | sf::Style::Resize|sf::Style::Titlebar);
    //sets the FPS of a game
    this->Window->setFramerateLimit(60);
}

void Game::initMap()
{
    this->Map = new map(windowWidth,windowHeight);
}

void Game::initPlayer1()
{
    this->player1 = new Player(windowWidth,windowHeight,this->Map,"Sprites/Player/run.png",93,178,11,100.f , sf::Keyboard::A,sf::Keyboard::D , 1);
    this->player2 = new Player(windowWidth,windowHeight,this->Map,"Sprites/Player/player_2.png",128,267,11,160.f , sf::Keyboard::Left, sf::Keyboard::Right , 2);
}

void Game::initobstacles()
{
    this->Obstacle = new obstacles(windowWidth,windowHeight,this->Map);
    //spawn random obstacles
    float pos_x , pos_y;
    
}
void Game::initPowerup()
{
    this->power = new Powers(windowWidth,windowHeight,this->Map);
    //spawn random icons
    float pos_x , pos_y;
    
}
void Game::pollEvents()
{
     

    while (this-> Window-> pollEvent(this->event)) 
    {
        switch(this->event.type)
        {
            case sf::Event::Closed:
                this->Window->close();
                break;
            case sf::Event::KeyPressed:
                if(this->event.key.code==sf::Keyboard::Escape)
                    this->Window->close();
                break;

        }
    }
}

void Game::initText()
{
    font.loadFromFile("Sprites/Fonts/BungeeSpice-Regular.ttf");

    p1HealthText.setFont(font);
    p1HealthText.setCharacterSize(18);
    p1HealthText.setFillColor(sf::Color::Black);
    p1HealthText.setPosition(20.f, 2.f);

    p1ScoreText.setFont(font);
    p1ScoreText.setCharacterSize(18);
    p1ScoreText.setFillColor(sf::Color::White);
    p1ScoreText.setPosition(20.f, 45.f);

    p2HealthText.setFont(font);
    p2HealthText.setCharacterSize(18);
    p2HealthText.setFillColor(sf::Color::Black);
    p2HealthText.setPosition(440.f, 2.f);

    p2ScoreText.setFont(font);
    p2ScoreText.setCharacterSize(18);
    p2ScoreText.setFillColor(sf::Color::White);
    p2ScoreText.setPosition(440.f, 45.f);

        //Player 1 health bars
    p1HealthBack.setSize({200.f, 20.f});
    p1HealthBack.setFillColor(sf::Color(60, 60, 60));
    p1HealthBack.setPosition(20.f, 20.f);

    p1HealthFront.setSize({200.f, 20.f});
    p1HealthFront.setFillColor(sf::Color::Red);
    p1HealthFront.setPosition(20.f, 20.f);

    // -------- Player 2  health bars
    p2HealthBack.setSize({200.f, 20.f});
    p2HealthBack.setFillColor(sf::Color(60, 60, 60));
    p2HealthBack.setPosition(420.f, 20.f);

    p2HealthFront.setSize({200.f, 20.f});
    p2HealthFront.setFillColor(sf::Color::Blue);
    p2HealthFront.setPosition(420.f, 20.f);
}
void Game::applyPowerUp(PowerType type, Player& target){
    switch (type)
    {   
        std::cout << "PowerUpApplied but not technically";
        std::cout << "Applied on player " << player1;
        std::cout << "Applied on player " << player2;
        // case PowerType::Confusion:
        //     target.applyConfusion(1.5f);
        //     break;

        // case PowerType::Net:
        //     target.blockMovement(1.0f);
        //     break;
    }
    }


const bool Game::running() const
{
    return this->Window->isOpen();
}


void Game::update()
{

    this-> pollEvents();
    float dt = deltaClock.restart().asSeconds();
    this-> Map->update(dt);
    
    this->player1->update(dt);
    this->player2->update(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {
        if (player1->hasPowerUp())
        {
        PowerType p = player1->usePowerUp();
        applyPowerUp(p, *player2);
        }
    }

       if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    {
        if (player2->hasPowerUp())
        {
        PowerType p = player2->usePowerUp();
        applyPowerUp(p, *player1);
        }
    }

    
    this->Obstacle->update(dt , windowHeight);
    Obstacle->collisonDetection(*player1 ,dt);
    Obstacle->collisonDetection(*player2,dt);
    this->power->update(dt,windowHeight);
    power->checkPickup(*player1,dt);
    power->checkPickup(*player2,dt);
    
    //First converts the health int to string then appends the string like 80 to '80'
    p1HealthText.setString("P1 Health: " );
    p1ScoreText.setString("P1 Score: " + std::to_string(player1->getScore()));
    p2HealthText.setString("P2 Health: ");
    p2ScoreText.setString("P2 Score: " + std::to_string(player2->getScore()));
    float p1HealthPercent = player1->getHealth() / 100.f;
    float p2HealthPercent = player2->getHealth() / 100.f;
    p1HealthFront.setSize({200.f * p1HealthPercent, 20.f});
    p2HealthFront.setSize({200.f * p2HealthPercent, 20.f});


}
void Game::render()
{
    this->Window->clear(sf::Color(255,155,100));
    this->Window->setView(this->Window->getDefaultView());
    //dereference the window
    this->Map->draw(*Window);
    this->Obstacle->draw(*Window);
    this->power->draw(*Window);
    this->player1->draw(*Window);
    this->player2->draw(*Window);
    
    
    Window->draw(p1HealthText);
    Window->draw(p1ScoreText);
    Window->draw(p2HealthText);
    Window->draw(p2ScoreText);
        // Player 1 health bar
    Window->draw(p1HealthBack);
    Window->draw(p1HealthFront);

    // Player 2 health bar
    Window->draw(p2HealthBack);
    Window->draw(p2HealthFront);
    
    this->Window->display();
    
    

}