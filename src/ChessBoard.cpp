#include "ChessBoard.h"
#include <iostream>

ChessPiece::ChessPiece() : color(0), x(0), y(0), row(-1), col(-1), is_selected(false) {}

ChessBoard::ChessBoard()
{
    for (int i = 0; i < CHESSBOARD_ROWS; i++)
    {
        std::vector<ChessPiece> temp; // 临时变量 一行棋子
        for (int j = 0; j < CHESSBOARD_COLS; j++)
        {
            ChessPiece chess_piece;
            chess_piece.x = 50 + j * 50;
            chess_piece.y = 50 + i * 50;
            chess_piece.row = i;
            chess_piece.col = j;
            temp.push_back(chess_piece); // 将棋子放入临时变量中
        }
        chess_pieces_arr.push_back(temp);
    }

    for (int i = 0; i < 3; i++)
    {
        int color = rand() % 6 + 1;
        next_three_color.push_back(color);
    }
}

void ChessBoard::PrintChessBoard()
{
    std::cout << "-------------------------" << std::endl;
    for (int i = 0; i < CHESSBOARD_ROWS; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLS; j++)
        {
            std::cout << chess_pieces_arr[i][j].color << " ";
        }
        std::cout << std::endl;
    }
    std::cout << "-------------------------" << std::endl;
}

/**
 * @brief 返回棋盘中剩余棋子的数量
 *
 */
int ChessBoard::GerRemainPieces()
{
    int ball_num = 0;
    for (int i = 0; i < CHESSBOARD_ROWS; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLS; j++)
        {
            if (chess_pieces_arr[i][j].color != 0)
                ball_num++;
        }
    }
    return ball_num;
}

void ChessBoard::CleanChessBoard()
{
    for (int i = 0; i < CHESSBOARD_ROWS; i++)

    {
        for (int j = 0; j < CHESSBOARD_COLS; j++)
            chess_pieces_arr[i][j].color = 0;
    }
}