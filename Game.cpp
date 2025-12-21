#include "Game.h"

Game::Game()
{
    this->initWindow();
    this->initMap();
    this->initPlayer1();
    this->initobstacles();
    this->initText();
    this->initPowerup();
    this->initSound();
    Main.loadFromFile("Sprites/Map/Mainmenu.png");
    menu.setTexture(Main);
    menu.setPosition(-150,0);
    
    
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
    this->player1 = new Player(windowWidth,windowHeight,this->Map,"Sprites/Player/run.png",93,178,11,100.f , sf::Keyboard::A,sf::Keyboard::D , 1 );
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
    p2HealthBack.setPosition(400.f, 20.f);

    p2HealthFront.setSize({200.f, 20.f});
    p2HealthFront.setFillColor(sf::Color::Blue);
    p2HealthFront.setPosition(400.f, 20.f);

    startText.setFont(font);
    startText.setString("PRESS SPACE TO START");
    startText.setCharacterSize(40);
    startText.setFillColor(sf::Color::White);
    startText.setPosition(80.f, 350.f);


    gameOverText.setFont(font);
    gameOverText.setString("                    GAME OVER\nPRESS SPACE TO RESTART");//5 tab
    gameOverText.setCharacterSize(40);
    gameOverText.setFillColor(sf::Color::Magenta);
    gameOverText.setPosition(50.f, 230.f);
    


}

void Game::initSound()
{   
    lightBuffer.loadFromFile("Sprites/Lightning.ogg");
    lightS.setBuffer(lightBuffer);
    HealBuffer.loadFromFile("Sprites/Healing.wav");
    HealS.setBuffer(HealBuffer);
}

void Game::applyPowerUp(PowerType type, Player& target , int id , float dt){
    
        float randomX = roadLeft + static_cast<float>(rand()) / RAND_MAX *(roadRight - roadLeft);
        float randomZ = spawnZMin + static_cast<float>(rand()) / RAND_MAX *(spawnZMax - spawnZMin);
    
    switch (type)
    {   
        case PowerType::Monster:
        power->spawnMonster(randomX,randomZ);
        break;

        case PowerType::Heal:

            if(id == 1){
                player1->addHealth(10);
                HealS.play();
            }
            else if(id == 2){
                player2->addHealth(10);
                HealS.play();
            }
            break;

        case::PowerType::lightning:
            target.poison(0.f);
            target.takeDamage(20);

            if(id == 1){
            sf::FloatRect pb =player2->getBounds();
                float centerX = pb.left + (pb.width / 2.f); //Fall on centre of player
                float bottomY = pb.top ; //Fall on player's head
                power->light.setTexture(power->lighttex);
                power->light.setOrigin(power->frameWidth2 / 2.f, static_cast<float>(power->frameHeight2));
                power->light.setPosition(centerX , bottomY);
                power->light.setScale(0.8f, 0.8f);
                power->lighted = true;
                lightS.play();
                lightS.setVolume(7);
            }
            else if(id == 2){
                sf::FloatRect pb =player1->getBounds();
                float centerX = pb.left + (pb.width / 2.f);
                float bottomY = pb.top ;
                power->light.setTexture(power->lighttex);
                power->light.setOrigin(power->frameWidth2 / 2.f, static_cast<float>(power->frameHeight2));
                power->light.setPosition(centerX , bottomY);
                power->light.setScale(0.8f, 0.8f);
                power->lighted = true;
                lightS.play();
                lightS.setVolume(7);
            }
            if(id == 1)
                player1->addScore(20);
            else
                player2->addScore(20);

            break;

        default:
            break;
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
    if (state == GameState::Start)
    {
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
    {
        state = GameState::Playing;
    }
    return;
    }
    if ((player1->getHealth() <= 0 || player2->getHealth() <= 0) || (player1->getScore() >= 99 || player2->getScore() >= 99))
    {
    
    state = GameState::GameOver;
    }

    if (state == GameState::GameOver)
    {   
        if(player1->getHealth() <= 0 || player2->getScore() >= 100){
        winnertext.setFont(font);
        winnertext.setString("                  Player 2 Wins!");//5 tab
        winnertext.setCharacterSize(40);
        winnertext.setFillColor(sf::Color::Black);
        winnertext.setPosition(50.f, 330.f);
        }
        else if(player2->getHealth() <= 0 || player1->getScore() >= 100){
        winnertext.setFont(font);
        winnertext.setString("                  Player 1 Wins!");//5 tab
        winnertext.setCharacterSize(40);
        winnertext.setFillColor(sf::Color::Black);
        winnertext.setPosition(50.f, 330.f);
        }
        // bgMusic.stop();
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {   
            delete this->Obstacle;
            delete this->power;
            initPlayer1();
            initMap();
            initobstacles();
            initText();
            initPowerup();
            initSound();
            state = GameState::Playing;
            // bgMusic.play();
        }
    return;
    }

    this-> Map->update(dt);
    
    this->player1->update(dt);
    this->player2->update(dt);

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
    {   
        if (player1->hasPowerUp()){
            PowerType p = player1->usePowerUp();
            if (p != PowerType::NONE)
                applyPowerUp(p, *player2 , 1 , dt);
        }
    }

       if (sf::Keyboard::isKeyPressed(sf::Keyboard::RControl))
    {   
        
        if (player2->hasPowerUp()){
            PowerType p = player2->usePowerUp();
            if (p != PowerType::NONE)
            applyPowerUp(p, *player1 , 2 , dt);
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

    if (state == GameState::Start){  
        Window->draw(menu); 
        Window->draw(startText);
    }

    else if (state == GameState::Playing)
    {
        this->Obstacle->draw(*Window);
        this->power->draw(*Window);
        int r = rand() % 10;
        if(r>=0 && r <= 4){
            this->player1->draw(*Window);
            this->player2->draw(*Window);
        }

        else if(r >= 5 && r < 10){
            this->player2->draw(*Window);
            this->player1->draw(*Window);
        }
    
        
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
    }
    else if (state == GameState::GameOver){
        Window->draw(gameOverText);
        Window->draw(winnertext);
    }
    this->Window->display();
    
    

}