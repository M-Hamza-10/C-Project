#include "player.h"
#include <iostream>
#include <windows.h>

Player::Player(int winWidth, int winHeight, map* Map ,const std::string& spritePath,
                int framewidth, int frameheight ,int frames , float startZ,sf::Keyboard::Key leftKey,
                 sf::Keyboard::Key rightKey, int ID)
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


    if(playerID == 1){
        light.setPosition(-60.f,650.f);
        light.setScale(0.2f,0.2f);
    }
    if(playerID == 2){
        light.setPosition(400.f,650.f);
        light.setScale(0.2f,0.2f);
    }

    playerX = 0.f;
    playerZ = startZ;
    moveSpeed = 400.f;

    health = 100;
    score = 0;

}
void Player::update(float dt)
{   
    //Player Movement
    if (sf::Keyboard::isKeyPressed(moveLeftKey)){
        if(poisoned)
            return;
        playerX -= moveSpeed * dt;
    }

    if (sf::Keyboard::isKeyPressed(moveRightKey)){
        if(poisoned)
            return;
        playerX += moveSpeed * dt;
    }

    // Clamp within road width
    playerX = std::clamp(playerX, -450.f, 450.f);
    
    animatePlayer(dt);


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
    //Controls the poison movement
    if(poisoned){
        poison_timer+=dt;
        if(poison_timer > 1.3f)
            poisoned = false;
    }

    //Controls the player movement when it is confused
     if(confused){
        confuse_timer += dt;
        if(confuse_timer <= 3.f){
        playerX -= moveSpeed * 0.7 * dt;
        playerX = std::clamp(playerX, -450.f, 450.f);
        }

        if(confuse_timer > 3.f)
            confused = false;
    }
    
    
    sf::Vector2f screenPos = projectPlayer(playerZ, playerX);
    player.setPosition(screenPos);

    //       SCALE DEPENDING ON DEPTH 
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

void Player::addHealth(int life)
{
    health += life;

    if(health > 100)
        health = 100;
}

void Player::poison(float dt)
{   
    if(poisoned)
        return;
    
    poisoned = true;
    poison_timer = 0.f;
}

void Player::loseControl(float dt)
{  
    if(confused)
        return;
    
    confused = true;
    confuse_timer = 0.f;
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

void Player::addPowerUp(PowerType type)
{
    inventory.push_back(type);

    if(type == PowerType::lightning){
        if(playerID == 1)
            lightning.loadFromFile("Sprites/Obstacles/Lightning_icon.png");
        else if(playerID == 2)
            lightning.loadFromFile("Sprites/Obstacles/Lightning_icon_2.png");
    }
    if(type == PowerType::Heal){
        if(playerID == 1)
            lightning.loadFromFile("Sprites/Obstacles/Heal_icon.png");
        else if(playerID == 2)
            lightning.loadFromFile("Sprites/Obstacles/Heal_icon_2.png");
    }
    if(type == PowerType::Monster){
        if(playerID == 1)
            lightning.loadFromFile("Sprites/Obstacles/Monster_icon.png");
        else if(playerID == 2)
            lightning.loadFromFile("Sprites/Obstacles/Monster_icon_2.png");
    }
    light.setTexture(lightning);
}

bool Player::hasPowerUp() const
{   
    return !inventory.empty();
}

PowerType Player::usePowerUp()
{   
    if (inventory.empty())
        return PowerType::NONE;

    PowerType type = inventory.front();
    inventory.erase(inventory.begin());
    inventory.clear();
    return type;
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
    if(!inventory.empty())
        window.draw(light);

}
