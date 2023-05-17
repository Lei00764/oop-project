#ifndef Graphic_H
#define Graphic_H

#include "ChessBoard.h"
#include <SFML/Graphics.hpp>

// 定义六种颜色：红、黄、蓝、绿、紫、橙
#define RED sf::Color(255, 0, 0)
#define YELLOW sf::Color(255, 255, 0)
#define BLUE sf::Color(0, 0, 255)
#define GREEN sf::Color(0, 255, 0)
#define PURPLE sf::Color(128, 0, 128)
#define ORANGE sf::Color(255, 165, 0)

class Graphic
{
public:
    void DrawChessBoardBg(); // 绘制棋盘棋盘
    void DrawChessPieces();  // 绘制棋子
};

#endif