#include "Game.h"

#include <iostream>

struct Point
{
    int row;
    int col;
    Point() : row(-1), col(-1){};
};

void InsertPath(int i, int j, Point pre[9][9], int path[100][2], int &p)
{
    if (pre[i][j].row != -1)
        InsertPath(pre[i][j].row, pre[i][j].col, pre, path, p);
    path[p][0] = i;
    path[p][1] = j;
    p++;
}

int Game::BFS()
{
    int is_path = 0; // is_path = 1 说明有路径
    Point pre[9][9]; // pre用于存储该点的前一点（类似父节点）从这一点走过来
    int visited[9][9] = {0};

    int to_row;
    int to_col;

    // 使用BFS遍历时，为了获得最短路径，每次路径需要保存上一点位置
    int path_index_arr[100][2] = {0};
    for (int i = 0; i < 100; i++)
    {
        path_index_arr[i][0] = -1;
        path_index_arr[i][1] = -1;
    }

    int front = -1, tail = -1; // front指向队首，tail指向队尾 front指向第一个元素的前一项，tail指向最后一个元素
    tail++;
    path_index_arr[tail][0] = start_row;
    path_index_arr[tail][1] = start_col;

    pre[start_row][start_col].row = -1;
    pre[start_row][start_col].col = -1;
    while (front != tail)
    {
        int row = path_index_arr[front + 1][0]; // (row, col)为队头点
        int col = path_index_arr[front + 1][1];
        front++;                              // 队首元素出队，移动队首指针就行
        if (row == end_row && col == end_col) // 找到终点
        {
            is_path = 1; // 存在路径
            break;
        }
        visited[row][col] = 1;
        if (chess_board.arr[row - 1][col] == 0 && row >= 1 && visited[row - 1][col] == 0) // 向上
        {
            to_row = row - 1;
            to_col = col;
            visited[to_row][to_col] = 1;
            tail++;
            path_index_arr[tail][0] = to_row;
            path_index_arr[tail][1] = to_col;
            pre[to_row][to_col].row = row;
            pre[to_row][to_col].col = col;
        }
        if (chess_board.arr[row][col + 1] == 0 && col < CHESSBOARD_COLS - 1 && visited[row][col + 1] == 0) // 向右
        {
            to_row = row;
            to_col = col + 1;
            visited[to_row][to_col] = 1;
            tail++;
            path_index_arr[tail][0] = to_row;
            path_index_arr[tail][1] = to_col;
            pre[to_row][to_col].row = row;
            pre[to_row][to_col].col = col;
        }
        if (chess_board.arr[row + 1][col] == 0 && row < CHESSBOARD_ROWS - 1 && visited[row + 1][col] == 0) // 向下
        {
            to_row = row + 1;
            to_col = col;
            visited[to_row][to_col] = 1;
            tail++;
            path_index_arr[tail][0] = to_row;
            path_index_arr[tail][1] = to_col;
            pre[to_row][to_col].row = row;
            pre[to_row][to_col].col = col;
        }
        if (chess_board.arr[row][col - 1] == 0 && col >= 1 && visited[row][col - 1] == 0) // 向左
        {
            to_row = row;
            to_col = col - 1;
            visited[to_row][to_col] = 1;
            tail++;
            path_index_arr[tail][0] = to_row;
            path_index_arr[tail][1] = to_col;
            pre[to_row][to_col].row = row;
            pre[to_row][to_col].col = col;
        }
    }

    int p = 0;
    InsertPath(end_row, end_col, pre, path, p);
    return is_path;
}

Game::Game() : start_row(-1), start_col(-1), end_row(-1), end_col(-1), score(0)
{
    for (int i = 0; i < 7; i++)
        delete_ball_num[i] = 0;
}

void Game::InitChessBoard()
{
    int count = 40; // 记录生成球的数量

    while (count > 0)
    {
        int row_pos = rand() % CHESSBOARD_ROWS;
        int col_pos = rand() % CHESSBOARD_COLS;
        int color = rand() % 7 + 1;
        if (chess_board.arr[row_pos][col_pos] == 0)
        {
            chess_board.arr[row_pos][col_pos] = 1; // 1改成color
            count--;
        }
    }
    chess_board.PrintChessBoard();
}

void Game::PlayGame()
{
    InitChessBoard(); // 随机生成 7 个棋子
    while (1)
    {
        ShowMessage();
        InputStartEnd(); // 输入起点和终点
        int visited[9][9] = {0};
        for (int i = 0; i < 100; i++) // 初始化路径数组
        {
            path[i][0] = -1;
            path[i][1] = -1;
        }
        int is_path = BFS();
        if (is_path)
        {
            // 打印路径
            for (int i = 0; i < 100; i++)
            {
                if (path[i][0] == -1 && path[i][1] == -1)
                    break;
                std::cout << "(" << path[i][0] << ", " << path[i][1] << ")" << std::endl;
            }
            chess_board.arr[end_row][end_col] = chess_board.arr[start_row][start_col];
            chess_board.arr[start_row][start_col] = 0;
            // 下一次不需要选择start_row, start_col
            start_row = end_row;
            start_col = end_col;
        }
        else
        {
            std::cout << "无法从"
                      << "(" << start_row << ", " << start_col << ")"
                      << "到达"
                      << "(" << end_row << ", " << end_col << ")" << std::endl;
        }

        int per_score = IsDelete(); // 如果有5个及以上的话，会更改arr的值
        if (per_score != 0)         // 说明本次移动完成了消球
        {
            start_row = -1;
            start_col = -1;
            end_row = -1;
            end_col = -1;
            score += per_score;
            std::cout << "本次移动消球" << per_score / 2 << "个" << std::endl;
        }
        else
        {
            std::cout << "无法消除，将随机生成 3 个球" << std::endl;
            CreateThreeBall(); // 只有当没有消球时，才会生成新的球
        }
        // 打印棋盘
        chess_board.PrintChessBoard();
    }
}

void Game::ShowMessage()
{
    // 总得分
    std::cout << "总得分：" << score << std::endl;
    int ball_color_num[7] = {0}; // 棋盘上每种颜色的球的数量
    for (int i = 0; i < CHESSBOARD_ROWS; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLS; j++)
        {
            if (chess_board.arr[i][j] == 0)
                ball_color_num[0]++;
            else if (chess_board.arr[i][j] == 1)
                ball_color_num[1]++;
            else if (chess_board.arr[i][j] == 2)
                ball_color_num[2]++;
            else if (chess_board.arr[i][j] == 3)
                ball_color_num[3]++;
            else if (chess_board.arr[i][j] == 4)
                ball_color_num[4]++;
            else if (chess_board.arr[i][j] == 5)
                ball_color_num[5]++;
            else if (chess_board.arr[i][j] == 6)
                ball_color_num[6]++;
        }
    }

    std::cout << "棋盘上剩余空位置有：" << ball_color_num[0] << "个" << std::endl;
    std::cout << "棋盘上剩余编号为1的球有：" << ball_color_num[1] << "个" << std::endl;
    std::cout << "消去编号为1的球有：" << delete_ball_num[1] << "个" << std::endl;
}

/**
 * 将球移到(end_row, end_col)后判断是否能够消除
 * 如果可以，消除然后返回得分，如果不行返回0
 * 消除n个球，得 2 * n 分
 */
int Game::IsDelete()
{
    int n = 0;         // 统计每次消球的数量
    int per_score = 0; // 统计分数
    int color = chess_board.arr[end_row][end_col];
    int sumy1 = 0; // 统计连续竖球的数量 该位置 + 向下
    int sumy2 = 0; // 统计连续竖球的数量 不包括 + 向上
    int sumx1 = 0; // 统计连续横球的数量 该位置 + 向右
    int sumx2 = 0; // 统计连续横球的数量 不包括 + 向左
                   // 先考虑竖
    // 向下
    for (int i = end_row; i < CHESSBOARD_ROWS; i++)
    {
        if (chess_board.arr[i][end_col] == 0 || chess_board.arr[i][end_col] != color)
            break;
        sumy1++;
    }
    // 向上
    for (int i = end_row - 1; i >= 0; i--)
    {
        if (chess_board.arr[i][end_col] == 0 || chess_board.arr[i][end_col] != color)
            break;
        sumy2++;
    }
    // 考虑横
    // 向右
    for (int i = end_col; i < CHESSBOARD_COLS; i++)
    {
        if (chess_board.arr[end_row][i] == 0 || chess_board.arr[end_row][i] != color)
            break;
        sumx1++;
    }
    // 向左
    for (int i = end_col - 1; i >= 0; i--)
    {
        if (chess_board.arr[end_row][i] == 0 || chess_board.arr[end_row][i] != color)
            break;
        sumx2++;
    }

    // 既有竖也有横 计算球的分数，并从数组中删除这些点
    if (sumy1 + sumy2 >= 5 && sumx1 + sumx2 >= 5)
    {
        n = sumy1 + sumy2 + sumx1 + sumx2 - 1;
        per_score = 2 * n;
        delete_ball_num[chess_board.arr[end_row][end_col]] += n;
        for (int i = end_row - sumy2; i < end_row + sumy1; i++)
            chess_board.arr[i][end_col] = 0;
        for (int i = end_col - sumx2; i < end_col + sumx1; i++)
            chess_board.arr[end_row][i] = 0;
    }
    // 只有竖 计算球的分数，并从数组中删除这些点
    else if (sumy1 + sumy2 >= 5)
    {
        n = sumy1 + sumy2;
        per_score = 2 * n;
        delete_ball_num[chess_board.arr[end_row][end_col]] += n;
        for (int i = end_row - sumy2; i < end_row + sumy1; i++)
            chess_board.arr[i][end_col] = 0;
    }
    // 只有横 计算球的分数，并从数组中删除这些点
    else if (sumx1 + sumx2 >= 5)
    {
        n = sumx1 + sumx2;
        per_score = 2 * n;
        delete_ball_num[chess_board.arr[end_row][end_col]] += n;
        for (int i = end_col - sumx2; i < end_col + sumx1; i++)
            chess_board.arr[end_row][i] = 0;
    }
    return per_score;
}

/**
 * @brief 若移动珠子没能消球，则将随机产生 3 个球，作为惩罚
 *
 */
void Game::CreateThreeBall()
{
    int count = 3;
    while (count > 0)
    {
        int row_pos = rand() % CHESSBOARD_ROWS;
        int col_pos = rand() % CHESSBOARD_COLS;
        int color = rand() % 7 + 1;
        if (chess_board.arr[row_pos][col_pos] == 0)
        {
            chess_board.arr[row_pos][col_pos] = 1; // 1改成color
            count--;
        }
    }
}

void Game::InputStartEnd()
{
    std::cout << "请输入起点坐标: ";
    std::cin >> start_row >> start_col;
    std::cout << "请输入终点坐标: ";
    std::cin >> end_row >> end_col;
}
