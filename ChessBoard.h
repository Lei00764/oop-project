#ifndef ChessBoard_H
#define ChessBoard_H

#include <vector>

class Game;

const int CHESSBOARD_ROWS = 9; // 棋盘行数
const int CHESSBOARD_COLS = 9; // 棋盘列数

class ChessBoard
{
public:
    ChessBoard();           // 构造函数
    void PrintChessBoard(); // 打印棋盘

    friend class Game; // 声明 Game 为友元类

private:
    int arr[CHESSBOARD_ROWS][CHESSBOARD_COLS]; //  二维数组
};

#endif