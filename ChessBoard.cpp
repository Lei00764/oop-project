#include "ChessBoard.h"
#include <iostream>

ChessBoard::ChessBoard()
{
    for (int i = 0; i < CHESSBOARD_ROWS; i++)
        for (int j = 0; j < CHESSBOARD_COLS; j++)
            arr[i][j] = 0;
}

void ChessBoard::PrintChessBoard()
{
    for (int i = 0; i < CHESSBOARD_ROWS; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLS; j++)
        {
            std::cout << arr[i][j] << " ";
        }
        std::cout << std::endl;
    }
}