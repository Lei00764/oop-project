#ifndef Game_H
#define Game_H

#include "ChessBoard.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <queue>
#include <vector>

// 棋盘位置偏移量
const int OFFSET_X = 50; // 棋盘左上角横坐标
const int OFFSET_Y = 80; // 棋盘左上角纵坐标  上下
// 结果信息显示偏移量
const int MESSAGE_X = 670;
const int MESSAGE_Y = 170;
// 信息提示区偏移量
const int PROMPT_X = 10;
const int PROMPT_Y = 530;
const int PROMPT_TIME = 2; // 信息提示显示时间
// 下3棋子显示去偏移量
const int PIECES_X = 180;
const int PIECES_Y = 10;

const int PIECE_SIZE = 50; // 棋盘格子大小

const int RADIUS = 20; // 棋子半径
const int SPEED = 10;  // 棋子移动速度

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
    void PlayGame();      // 开始游戏
    void DealWithEvent(); // 处理所有event事件

    void InitGame();               // 初始化游戏
    void SetChessPiecesPosition(); // 设置棋子位置，每次移动前均要重新设置
    void InputStartEnd();          // 键盘输入起点和终点坐标

    void BFS();                                     // 广度优先搜索，判断是否有路径从起点到终点
    void InsertPath(int i, int j, Point pre[9][9]); // 将路径插入到path数组中，配合BFS使用
    void IsDelete(int row, int col);                // 判断是否消球，如果有5个及以上的话，会更改arr的值
    void CreateThreePieces();                       // 随机生成 3 个棋子
    void UpdateChessBoard();                        // 更新棋盘，消除棋子，生成新的棋子
    void UpdateScore();                             // 更新得分
    void CheckGameOver();                           // 检查游戏是否结束

    void DrawChessBoardBg();    // 绘制棋盘背景
    void DrawChessPieces();     // 绘制棋子
    void DrawMovePiece();       // 绘制棋子移动效果
    void DrawNextThreePieces(); // 显示接下来应该出现的 3 个棋子
    void DrawMessage();         // 显示游戏信息
    void DrawScore();           // 显示游戏得分
    void DrawPrompt();          // 显示游戏提示信息
    void DrawGameOver();        // 显示游戏结束画面

private:
    ChessBoard chess_board;
    int start_row;              // 起点行坐标
    int start_col;              // 起点列坐标
    int end_row;                // 终点行坐标
    int end_col;                // 终点列坐标
    int score;                  // 得分
    int per_score;              // 单次得分
    int move_times;             // 移动次数
    bool is_moving;             // is_moving = 1 正在移动，is_moving = 0 当前处于静止状态
    bool click_twice;           // click_twice = true 选中起点和终点
    bool is_path;               // 从起点到终点是否有路径
    bool auto_score;            // 自动得分
    int game_over;              // game_over = 1 胜利 game_over = -1 失败 game_over = 0 继续
    int delete_ball_num[7];     // 记录各种颜色球消除总数量
    bool game_ove_music_played; // 记录游戏结束音乐是否已经播放过
    std::queue<Point> path;     // 记录从起点到终点的路径
    std::vector<int> t_score;   // 保留 3 次随机棋子可能产生的分数

private:
    sf::RenderWindow window;       // 窗口
    sf::Font font;                 // 字体
    sf::Clock clock1;              // 记录自动得分显示信息显示
    sf::Time elapsed1;             // 配合clock使用
    sf::SoundBuffer sound_buffer1; // 音频缓冲区
    sf::Sound sound;               // 音频源对象
};

#endif