#ifndef Game_H
#define Game_H

#include "ChessBoard.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <iostream>
#include <queue>

// 偏移量
const int OFFSET_X = 100; // 棋盘左上角横坐标
const int OFFSET_Y = 200; // 棋盘左上角纵坐标  上下

const int PIECE_SIZE = 100; // 棋盘格子大小

const int RADIUS = 40; // 棋子半径
const int SPEED = 1;   // 棋子移动速度

#define RED sf::Color(255, 0, 0)
#define YELLOW sf::Color(255, 255, 0)
#define BLUE sf::Color(0, 0, 255)
#define GREEN sf::Color(0, 255, 0)
#define ORANGE sf::Color(255, 165, 0)
#define PURPLE sf::Color(128, 0, 128)

struct Point
{
    int row;
    int col;
    Point() : row(-1), col(-1){};
    Point(int r, int c) : row(r), col(c){};
};

class Game
{
public:
    Game();
    void PlayGame();               // 开始游戏
    void InitChessBoard();         // 初始化棋盘 随机生成 7 个棋子
    void SetChessPiecesPosition(); // 设置棋子位置，每次移动前均要重新设置
    void InputStartEnd();          // 键盘输入起点和终点坐标

    void BFS();                                     // 广度优先搜索，判断是否有路径从起点到终点
    void InsertPath(int i, int j, Point pre[9][9]); // 将路径插入到path数组中，配合BFS使用
    int IsDelete(int row, int col);                 // 判断是否消球，如果有5个及以上的话，会更改arr的值
    void CreateThreePieces();                       // 随机生成 3 个棋子
    void UpdateChessBoard();                        // 更新棋盘，消除棋子，生成新的棋子
    void UpdateScore();                             // 更新得分
    int CheckGameOver();                            // 检查游戏是否结束 胜利则返回1，失败则返回-1，如果继续则返回0

    void DealWithEvent(sf::RenderWindow &window);              // 处理所有event事件
    void DrawChessBoardBg(sf::RenderWindow &window);           // 绘制棋盘背景
    void DrawChessPieces(sf::RenderWindow &window);            // 绘制棋子
    void MovePiece(sf::RenderWindow &window);                  // 绘制棋子移动效果
    void DrawMessage(sf::RenderWindow &window, sf::Font font); // 显示游戏信息

public:
    ChessBoard chess_board;
    int start_row;          // 起点行坐标
    int start_col;          // 起点列坐标
    int end_row;            // 终点行坐标
    int end_col;            // 终点列坐标
    int score;              // 得分
    int delete_ball_num[7]; // 记录各种颜色球消除总数量
    std::queue<Point> path; // 记录从起点到终点的路径
    int move_times;         // 移动次数
    bool is_moving;         // is_moving = 1 正在移动，is_moving = 0 当前处于静止状态
    bool click_twice;       // click_twice = true 选中起点和终点
    bool is_path;           // 从起点到终点是否有路径
};

#endif