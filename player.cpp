#include "player.h"
#include <iostream>
#include <windows.h>

Player::Player(int winWidth, int winHeight, map* Map ,const std::string& spritePath,
                int framewidth, int frameheight ,int frames , float startZ,sf::Keyboard::Key leftKey,
                 sf::Keyboard::Key rightKey, int ID )
{  
    this-> Map = Map;
    
    moveLeftKey = leftKey;
    moveRightKey = rightKey;
    playerID = ID;
   
    playerTex.loadFromFile(spritePath);
    player.setTexture(playerTex);
    frameWidth  = framewidth;
    frameHeight = frameheight;
    frameCount  = frames;
    player.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    player.setOrigin(frameWidth / 2.f, frameHeight); // feet on ground
 
    // player2Tex.loadFromFile("Sprites/Player/player_2.png");
    // player2.setTexture(player2Tex);
    // player2.setTextureRect(sf::IntRect(0, 0, frameWidth, frameHeight));
    // player2.setOrigin((frameWidth / 2) - 100, frameHeight);
    playerX = 0.f;
    playerZ = startZ;
    moveSpeed = 400.f;

    health = 100;
    score = 0;

}
void Player::update(float dt)
{
    
    // ========================================
    //      PLAYER MOVEMENT (Left / Right)
    // ========================================
    if (sf::Keyboard::isKeyPressed(moveLeftKey))
        playerX -= moveSpeed * dt;

    if (sf::Keyboard::isKeyPressed(moveRightKey))
        playerX += moveSpeed * dt;

    // Clamp within road width
    playerX = std::clamp(playerX, -450.f, 450.f);

    //Player 2
    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    //     player2X -= 300 * dt;

    // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    //     player2X += 300 * dt;

    // // Clamp within road width
    // player2X = std::max(-1000.f, std::min(350.f, player2X ));

    // ========================================
    //         PLAYER ANIMATION
    // ========================================
    animatePlayer(dt);
    // animatePlayer2(dt);

    // ========================================
    //    PROJECT PLAYER INTO PERSPECTIVE
    // ========================================
      if(playerID == 2) {
        playerZ = 110.f;
      }

        if (isInvincible)
    {
    invincibleTimer += dt;
    blinkTimer += dt;

    if (blinkTimer >= blinkInterval)
    {
        blinkTimer = 0.f;
        visible = !visible;

        sf::Color colour = player.getColor();
        colour.a = visible ? 255 : 80; // alpha
        player.setColor(colour);
    }

    if (invincibleTimer >= invincibleDuration)
    {
        isInvincible = false;
        player.setColor(sf::Color::White); // fully visible
    }
    }
    
    sf::Vector2f screenPos = projectPlayer(playerZ, playerX);
    player.setPosition(screenPos);

    // sf::Vector2f screenPos2 = projectPlayer(player2Z, player2X);
    // player2.setPosition(screenPos2);

    // ========================================
    //       SCALE DEPENDING ON DEPTH
    // ========================================
    if(playerID == 1){
    float depthScale = 1.f - (playerZ / winHeight);
    if (depthScale < 0.8f)
        depthScale = 0.8f;

        player.setScale(depthScale , depthScale);
    }
    else if(playerID == 2){
    float depthScale2 = 0.55;
        player.setScale(depthScale2 , depthScale2);
    }

    // float depthScale2 = 1.f - (player2Z / winHeight);
    // if (depthScale2 < 0.6f)
    //     depthScale2 = 0.6f;

    
    // player2.setScale(depthScale2, depthScale2);
}

void Player::animatePlayer(float dt)
{
    animTimer += dt;
    if (animTimer >= animSpeed)
    {
        animTimer = 0.f;
        currentFrame = (currentFrame + 1) % frameCount;

        player.setTextureRect(sf::IntRect(
            currentFrame * frameWidth,
            0,
            frameWidth,
            frameHeight)); 
    }
}
// void Player::animatePlayer2(float dt)
// {
//     animTimer2 += dt;
//     if (animTimer2 >= animSpeed2)
//     {
//         animTimer2 = 0.f;
//         currentFrame2 = (currentFrame2 + 1) % frameCount2;

//         player2.setTextureRect(sf::IntRect(
//             currentFrame2 * frame2Width,
//             0,
//             frame2Width,
//             frame2Height)); 
//     }
// }

sf::Vector2f Player::projectPlayer(float worldZ, float offsetX)
{
        // 1. Convert fake-depth Z into Y for transformPerspective()
    float y = worldZ;

    // 2. Fake 3D X: center + offset
    float x = Map->perspectiveX + offsetX;
    
    // 3. Use your existing perspective system!
    return Map->transformPerspective(x, y);
}

int Player::getHealth() const
{
    return health;
}

int Player::getScore() const
{
    return score;
}

void Player::takeDamage(int dmg)
{  
    if (isInvincible)
        return;

    health -= dmg;
 

    if (health < 0)
        health = 0;

    isInvincible = true;
    invincibleTimer = 0.f;
    blinkTimer = 0.f;
    visible = true;
}

float Player::getWorldZ() const
{
    return playerZ;
}

void Player::addScore(int value)
{
    score += value;
}

bool Player::Invincible() const
{
    return isInvincible;
}

sf::FloatRect Player::getBounds() const
{
    sf::FloatRect bounds = player.getGlobalBounds();
    bounds.left += bounds.width * 0.2f;
    bounds.width *= 0.6f;

    bounds.top += bounds.height * 0.15f;
    bounds.height *= 0.7f;
    return bounds;
}




void Player::draw(sf::RenderWindow &window)
{   
    window.draw(player);
    // window.draw(player2);
}
