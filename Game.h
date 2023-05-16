#ifndef Game_H
#define Game_H

#include "ChessBoard.h"

struct Point
{
    int row;
    int col;
    Point() : row(-1), col(-1){};
};

class Game
{
public:
    Game();
    void InitChessBoard();                                  // 初始化棋盘 随机生成 7 个球
    void PlayGame();                                        // 开始游戏
    void InputStartEnd();                                   // 输入起点和终点坐标
    int IsDelete(int row, int col);                         // 判断是否消球，如果有5个及以上的话，会更改arr的值
    void ShowMessage();                                     // 显示游戏信息
    void CreateThreeBall();                                 // 随机生成 3 个球
    int BFS();                                              // 广度优先搜索，判断是否有路径从起点到终点，返回1表示有路径，返回0表示无路径
    void InsertPath(int i, int j, Point pre[9][9], int &p); // 将路径插入到path数组中
    int GetBallNum();                                       // 棋盘中剩余棋子的数量

private:
    ChessBoard chess_board;
    int start_row;          // 起点行坐标
    int start_col;          // 起点列坐标
    int end_row;            // 终点行坐标
    int end_col;            // 终点列坐标
    int score;              // 得分
    int delete_ball_num[7]; // 记录各种颜色球消除总数量
    int path[100][2];       // 记录从起点到终点的路径
};

#endif