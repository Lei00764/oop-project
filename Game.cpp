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

int BFS(int arr[9][9], int rows, int cols, int start_row, int start_col, int end_row, int end_col, int visited[9][9], int path[100][2])
{
    int is_path = 0; // is_path = 1 说明有路径
    Point pre[9][9]; // pre用于存储该点的前一点（类似父节点）从这一点走过来

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
        if (arr[row - 1][col] == 0 && row >= 1 && visited[row - 1][col] == 0) // 向上
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
        if (arr[row][col + 1] == 0 && col < cols - 1 && visited[row][col + 1] == 0) // 向右
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
        if (arr[row + 1][col] == 0 && row < rows - 1 && visited[row + 1][col] == 0) // 向下
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
        if (arr[row][col - 1] == 0 && col >= 1 && visited[row][col - 1] == 0) // 向左
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

void Game::InitChessBoard()
{
    int count = 7; // 记录生成球的数量

    while (count > 0)
    {
        int row_pos = rand() % CHESSBOARD_ROWS;
        int col_pos = rand() % CHESSBOARD_COLS;
        int color = rand() % 7 + 1;
        if (chess_board.arr[row_pos][col_pos] == 0)
        {
            chess_board.arr[row_pos][col_pos] = color; // 1改成color
            count--;
        }
    }
    chess_board.PrintChessBoard();
}

void Game::InputStartEnd()
{
    std::cout << "请输入起点坐标: ";
    std::cin >> start_row >> start_col;
    std::cout << "请输入终点坐标: ";
    std::cin >> end_row >> end_col;
    int visited[9][9] = {0};
    int path[100][2];
    for (int i = 0; i < 100; i++)
    {
        path[i][0] = -1;
        path[i][1] = -1;
    }

    int is_path = BFS(chess_board.arr, 9, 9, start_row, start_col, end_row, end_col, visited, path);

    std::cout << "无法从"
              << "(" << start_row << ", " << start_col << ")"
              << "到达"
              << "(" << end_row << ", " << end_col << ")" << std::endl;

    if (is_path)
    {
        // 打印路径
        for (int i = 0; i < 100; i++)
        {
            if (path[i][0] == -1 && path[i][1] == -1)
                break;
            std::cout << "(" << path[i][0] << ", " << path[i][1] << ")" << std::endl;
        }

        std::cout << "画出移动过程" << std::endl;
    }
}
