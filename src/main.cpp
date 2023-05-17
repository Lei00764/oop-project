#include <iostream>
#include <SFML/Graphics.hpp>
#include "Game.h"

int main()
{
    srand(unsigned(time(NULL))); // 设置随机数种子

    Game game;
    game.PlayGame(); // 开始游戏

    return 0;
}