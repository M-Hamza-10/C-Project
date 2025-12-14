#include <SFML\Graphics.hpp>
#include <SFML\Audio.hpp>
#include <SFML\System.hpp>
#include <SFML\Window.hpp>
#include <SFML\Network.hpp>
#include "Game.h"

#include <iostream>
using namespace std;


Game game;
int main()
{
    while(game.running())
{
    game.update();
    game.render();
}



    return 0;
}

