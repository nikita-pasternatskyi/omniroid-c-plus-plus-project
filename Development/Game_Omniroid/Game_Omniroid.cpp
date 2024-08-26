#include <iostream>
#include "src/Game.h"

int main()
{
    Game game;
    while (game.ShouldRun())
    {
        game.Update();
    }
    return 0;
}