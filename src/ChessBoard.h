#ifndef ChessBoard_H
#define ChessBoard_H
#include <vector>

class Game;

const int CHESSBOARD_ROWS = 9; // 棋盘行数
const int CHESSBOARD_COLS = 9; // 棋盘列数

class ChessPiece
{
public:
    ChessPiece();

public:
    int color; // 颜色
    int x;     // 棋子与棋盘的相对位置（相对于棋盘左上角）横坐标
    int y;
    int row; // 棋子位于棋盘的行
    int col;
    bool is_selected; // 是否被选中
};

class ChessBoard
{
public:
    ChessBoard();           // 构造函数
    void PrintChessBoard(); // 打印棋盘
    int GerRemainPieces();  // 获取棋盘剩余棋子数量
    void CleanChessBoard(); // 清空棋盘

    friend class Game; // 声明 Game 为友元类

private:
    std::vector<std::vector<ChessPiece>> chess_pieces_arr; // 二维数组
    std::vector<int> next_three_color;                     // 下一次显示的3个棋子的颜色
};

#endif