#include <iostream>
#include "Game.h"

int main()
{
    srand(unsigned(time(NULL))); // 设置随机数种子
    Game game;

    game.InitChessBoard();

    game.InputStartEnd();

    return 0;
}