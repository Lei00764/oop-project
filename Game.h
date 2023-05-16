#ifndef Game_H
#define Game_H

#include "ChessBoard.h"

class Game
{
public:
    // Game();
    void InitChessBoard(); // 初始化棋盘 随机生成 7 个球
    void InputStartEnd();  // 输入起点和终点坐标

private:
    ChessBoard chess_board;
    int start_row; // 起点行坐标
    int start_col; // 起点列坐标
    int end_row;   // 终点行坐标
    int end_col;   // 终点列坐标
};

#endif