#include "Game.h"

#include <iostream>

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
#define PURPLE sf::Color(128, 0, 128)
#define ORANGE sf::Color(255, 165, 0)

/**
 * @brief 构造函数
 *
 */
Game::Game() : start_row(-1), start_col(-1), end_row(-1), end_col(-1), score(0), is_moving(false), move_times(0), click_twice(false), is_path(false)
{
    for (int i = 0; i < 7; i++)
        delete_ball_num[i] = 0;
}

/**
 * @brief 开始游戏
 */
void Game::PlayGame()
{
    sf::RenderWindow window(sf::VideoMode(1500, 1500), L"五子连珠");
    InitChessBoard();         // 随机生成 7 个棋子
    SetChessPiecesPosition(); // 设置棋子的位置

    while (window.isOpen())
    {

        DealWithEvent(window); // 处理用户事件

        // 更新
        if (click_twice && !is_moving) // 选中起点和终点，且当前棋子没有移动 -> 寻找路径
        {
            SetChessPiecesPosition(); // 设置棋子的位置
            BFS();                    // 寻找路径

            if (is_path)          // 有路径
                is_moving = true; // 开始移动
        }

        if (is_moving)         // 棋子正在移动
            MovePiece(window); // 绘制棋子移动的动画

        window.clear();
        DrawChessBoardBg(window); // 绘制棋盘背景
        DrawChessPieces(window);  // 绘制棋子
        window.display();
    }
}

/**
 * @brief 初始化棋盘，生成随机棋子
 *
 */
void Game::InitChessBoard()
{
    int count = 7; // 记录生成球的数量

    while (count > 0)
    {
        int row_pos = rand() % CHESSBOARD_ROWS;
        int col_pos = rand() % CHESSBOARD_COLS;
        int color = rand() % 6 + 1;
        if (chess_board.chess_pieces_arr[row_pos][col_pos].color == 0)
        {
            chess_board.chess_pieces_arr[row_pos][col_pos].color = color; // 1改成color
            count--;
        }
    }
    chess_board.PrintChessBoard();
}

/**
 * @brief 设置棋子的位置，相对于棋盘左上角的横纵坐标
 *
 */
void Game::SetChessPiecesPosition()
{
    for (int i = 0; i < CHESSBOARD_ROWS; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLS; j++)
        {
            // 位于棋盘格子的正中心
            chess_board.chess_pieces_arr[i][j].x = PIECE_SIZE * j + RADIUS / 4.0;
            chess_board.chess_pieces_arr[i][j].y = PIECE_SIZE * i + RADIUS / 4.0;
        }
    }
}

/**
 * @brief 键盘输入起点和终点坐标
 *
 */
void Game::InputStartEnd()
{
    std::cout << "请输入起点坐标: ";
    std::cin >> start_row >> start_col;
    std::cout << "请输入终点坐标: ";
    std::cin >> end_row >> end_col;
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
            if (chess_board.chess_pieces_arr[i][j].color == 0)
                ball_color_num[0]++;
            else if (chess_board.chess_pieces_arr[i][j].color == 1)
                ball_color_num[1]++;
            else if (chess_board.chess_pieces_arr[i][j].color == 2)
                ball_color_num[2]++;
            else if (chess_board.chess_pieces_arr[i][j].color == 3)
                ball_color_num[3]++;
            else if (chess_board.chess_pieces_arr[i][j].color == 4)
                ball_color_num[4]++;
            else if (chess_board.chess_pieces_arr[i][j].color == 5)
                ball_color_num[5]++;
            else if (chess_board.chess_pieces_arr[i][j].color == 6)
                ball_color_num[6]++;
        }
    }

    std::cout << "棋盘上剩余空位置有：" << ball_color_num[0] << "个" << std::endl;
    std::cout << "棋盘上剩余编号为1的球有：" << ball_color_num[1] << "个" << std::endl;
    std::cout << "消去编号为1的球有：" << delete_ball_num[1] << "个" << std::endl;
}

/**
 * @brief 寻找最短路径
 *
 * @return 返回1表示有路径，返回0表示无路径
 */
void Game::BFS()
{
    is_path = 0;     // is_path = 1 说明有路径
    Point pre[9][9]; // pre用于存储该点的前一点（类似父节点）从这一点走过来
    int visited[9][9] = {0};

    int to_row;
    int to_col;

    // 使用BFS遍历时，为了获得最短路径，每次路径需要保存上一点位置
    int path_index_arr[100][2] = {0};
    // 初始化数组
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
        if (row >= 1 && chess_board.chess_pieces_arr[row - 1][col].color == 0 && visited[row - 1][col] == 0) // 向上
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
        if (col < CHESSBOARD_COLS - 1 && chess_board.chess_pieces_arr[row][col + 1].color == 0 && visited[row][col + 1] == 0) // 向右
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
        if (row < CHESSBOARD_ROWS - 1 && chess_board.chess_pieces_arr[row + 1][col].color == 0 && visited[row + 1][col] == 0) // 向下
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
        if (col >= 1 && chess_board.chess_pieces_arr[row][col - 1].color == 0 && visited[row][col - 1] == 0) // 向左
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

    // 清空path队列
    while (!path.empty())
        path.pop();

    InsertPath(end_row, end_col, pre);
}

/**
 * @brief 寻找最短路径的辅助函数
 */
void Game::InsertPath(int i, int j, Point pre[9][9])
{
    if (pre[i][j].row != -1)
        InsertPath(pre[i][j].row, pre[i][j].col, pre);
    path.push(Point(i, j));
}

/**
 * 将球移到(row, col)后判断是否能够消除
 * 如果可以，消除然后返回得分，如果不行返回0
 * 消除n个球，得 2 * n 分
 */
int Game::IsDelete(int row, int col)
{
    int n = 0;         // 统计每次消球的数量
    int per_score = 0; // 统计分数
    int color = chess_board.chess_pieces_arr[row][col].color;
    int sumy1 = 0; // 统计连续竖球的数量 该位置 + 向下
    int sumy2 = 0; // 统计连续竖球的数量 不包括 + 向上
    int sumx1 = 0; // 统计连续横球的数量 该位置 + 向右
    int sumx2 = 0; // 统计连续横球的数量 不包括 + 向左
                   // 先考虑竖
    // 向下
    for (int i = row; i < CHESSBOARD_ROWS; i++)
    {
        if (chess_board.chess_pieces_arr[i][col].color == 0 || chess_board.chess_pieces_arr[i][col].color != color)
            break;
        sumy1++;
    }
    // 向上
    for (int i = row - 1; i >= 0; i--)
    {
        if (chess_board.chess_pieces_arr[i][col].color == 0 || chess_board.chess_pieces_arr[i][col].color != color)
            break;
        sumy2++;
    }
    // 考虑横
    // 向右
    for (int i = col; i < CHESSBOARD_COLS; i++)
    {
        if (chess_board.chess_pieces_arr[row][i].color == 0 || chess_board.chess_pieces_arr[row][i].color != color)
            break;
        sumx1++;
    }
    // 向左
    for (int i = col - 1; i >= 0; i--)
    {
        if (chess_board.chess_pieces_arr[row][i].color == 0 || chess_board.chess_pieces_arr[row][i].color != color)
            break;
        sumx2++;
    }

    // 既有竖也有横 计算球的分数，并从数组中删除这些点
    if (sumy1 + sumy2 >= 5 && sumx1 + sumx2 >= 5)
    {
        n = sumy1 + sumy2 + sumx1 + sumx2 - 1;
        per_score = 2 * n;
        delete_ball_num[chess_board.chess_pieces_arr[row][col].color] += n;
        for (int i = row - sumy2; i < row + sumy1; i++)
            chess_board.chess_pieces_arr[i][col].color = 0;
        for (int i = col - sumx2; i < col + sumx1; i++)
            chess_board.chess_pieces_arr[row][i].color = 0;
    }
    // 只有竖 计算球的分数，并从数组中删除这些点
    else if (sumy1 + sumy2 >= 5)
    {
        n = sumy1 + sumy2;
        per_score = 2 * n;
        delete_ball_num[chess_board.chess_pieces_arr[row][col].color] += n;
        for (int i = row - sumy2; i < row + sumy1; i++)
            chess_board.chess_pieces_arr[i][col].color = 0;
    }
    // 只有横 计算球的分数，并从数组中删除这些点
    else if (sumx1 + sumx2 >= 5)
    {
        n = sumx1 + sumx2;
        per_score = 2 * n;
        delete_ball_num[chess_board.chess_pieces_arr[row][col].color] += n;
        for (int i = col - sumx2; i < col + sumx1; i++)
            chess_board.chess_pieces_arr[row][i].color = 0;
    }
    return per_score;
}

/**
 * @brief 若移动珠子没能消球，则将随机产生 3 个棋子，作为惩罚
 *
 */
void Game::CreateThreePieces()
{
    int count = 3;
    while (count > 0)
    {
        int row_pos = rand() % CHESSBOARD_ROWS;
        int col_pos = rand() % CHESSBOARD_COLS;
        int color = rand() % 6 + 1;
        if (chess_board.chess_pieces_arr[row_pos][col_pos].color == 0)
        {
            chess_board.chess_pieces_arr[row_pos][col_pos].color = color; // 1改成color
            count--;
        }
        // 系统随机产生的珠子正好能凑成了同色的5颗及以上一起排成横向、纵向或者斜向，则这几颗同向的珠子自行消除，游戏者得分
        int per_score = IsDelete(row_pos, col_pos); // 消球
        if (per_score != 0)
        {
            score += per_score;
            std::cout << "运气真好，本次自动消球得分为：" << per_score << std::endl;
        }
    }
}

/**
 * @brief 更新棋盘，消除棋子，生成新的棋子
 */
void Game::UpdateChessBoard()
{
    chess_board.chess_pieces_arr[end_row][end_col].color = chess_board.chess_pieces_arr[start_row][start_col].color;
    chess_board.chess_pieces_arr[start_row][start_col].color = 0;
    chess_board.chess_pieces_arr[start_row][start_col].is_selected = false;
    chess_board.chess_pieces_arr[end_row][end_col].is_selected = true;
    start_row = end_row;
    start_col = end_col;
}

/**
 * @brief 更新分数
 */
void Game::UpdateScore()
{
    int per_score = IsDelete(end_row, end_col); // 如果有5个及以上的话，会更改arr的值
    if (per_score != 0)                         // 说明本次移动完成了消球
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
        CreateThreePieces(); // 只有当没有消球时，才会生成新的球
    }
}

/**
 * @brief 检测游戏是否成功
 *
 * @return 胜利则返回1，失败则返回-1，如果继续则返回0
 */
int Game::CheckGameOver()
{
    int sum = chess_board.GerRemainPieces(); // 棋盘中剩余棋子的数量
    if (sum == 0)                            // 棋盘中没有棋子了
    {
        std::cout << "胜利！！！" << std::endl;
        return 1; // 游戏结束
    }
    else if (sum + 3 > CHESSBOARD_COLS * CHESSBOARD_ROWS)
    {
        std::cout << "失败！！！" << std::endl;
        return -1; // 游戏结束
    }
    else
        return 0; // 游戏继续
}

/**
 * @brief 处理所有用户事件
 *
 * @param window
 */
void Game::DealWithEvent(sf::RenderWindow &window)
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) // 关闭窗口
            window.close();

        if (event.type == sf::Event::MouseButtonPressed) // 鼠标点击
        {
            if (event.mouseButton.button == sf::Mouse::Right)
            {
                std::cout << "点击鼠标右键，应该关闭游戏" << std::endl;
                window.close();
            }

            if (event.mouseButton.button == sf::Mouse::Left) // 鼠标左键
            {
                int x = event.mouseButton.x;
                int y = event.mouseButton.y;
                std::cout << "x: " << x << " y: " << y << std::endl;
                // 超出棋盘范围
                if (x < OFFSET_X || x > OFFSET_X + CHESSBOARD_COLS * PIECE_SIZE || y < OFFSET_Y || y > OFFSET_Y + CHESSBOARD_ROWS * PIECE_SIZE)
                {
                    std::cout << "超出棋盘范围" << std::endl;
                    continue;
                }
                end_row = (y - OFFSET_Y) / PIECE_SIZE; // 当前点击的行
                end_col = (x - OFFSET_X) / PIECE_SIZE;
                if (chess_board.chess_pieces_arr[end_row][end_col].color != 0) // 该位置有棋子
                {
                    std::cout << "该位置有棋子" << std::endl;
                    if (start_row != -1 && start_col != -1) // 原来选中的球取消标记
                    {
                        chess_board.chess_pieces_arr[start_row][start_col].is_selected = false;

                        std::cout << "取消标记" << start_row << " " << start_col << std::endl;
                    }
                    std::cout << "标记" << end_row << " " << end_col << std::endl;
                    chess_board.chess_pieces_arr[end_row][end_col].is_selected = true; // 选中该球
                    start_row = end_row;
                    start_col = end_col;
                    click_twice = false;
                }
                else // 选中空白地方
                {
                    std::cout << "选中空白地方" << end_row << " " << end_col << std::endl;
                    if (start_row != -1 && start_col != -1) //  说明之前选中过球 此时就要寻找路径了
                        click_twice = true;
                    else
                        click_twice = false;
                }
            }
        }
    }
}

/**
 * @brief 画棋盘背景
 */
void Game::DrawChessBoardBg(sf::RenderWindow &window)
{
    // 绘制 9x9 正方形
    for (int i = 0; i < CHESSBOARD_ROWS; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLS; j++)
        {
            sf::RectangleShape rectangle(sf::Vector2f(PIECE_SIZE, PIECE_SIZE));
            if ((i + j) % 2 == 0)
                rectangle.setFillColor(sf::Color(255, 255, 255));
            else
                rectangle.setFillColor(sf::Color(128, 128, 128));
            rectangle.setPosition(sf::Vector2f(PIECE_SIZE * j + OFFSET_X, PIECE_SIZE * i + OFFSET_Y));
            window.draw(rectangle);
        }
    }
}

/**
 * @brief 画出所有棋子
 */
void Game::DrawChessPieces(sf::RenderWindow &window)
{
    for (int i = 0; i < CHESSBOARD_ROWS; i++)
    {
        for (int j = 0; j < CHESSBOARD_COLS; j++)
        {
            if (chess_board.chess_pieces_arr[i][j].color == 0)
                continue;
            // 棋子用圆形代替
            sf::CircleShape piece(RADIUS); // 半径
            // 如果棋子被标记，画出外轮廓
            if (chess_board.chess_pieces_arr[i][j].is_selected)
            {
                piece.setOutlineThickness(5);
                piece.setOutlineColor(sf::Color::Black);
            }
            // 根据 chess_board[i][j].color 确定颜色
            switch (chess_board.chess_pieces_arr[i][j].color)
            {
            case 1:
                piece.setFillColor(RED);
                break;
            case 2:
                piece.setFillColor(BLUE);
                break;
            case 3:
                piece.setFillColor(GREEN);
                break;
            case 4:
                piece.setFillColor(YELLOW);
                break;
            case 5:
                piece.setFillColor(PURPLE);
                break;
            case 6:
                piece.setFillColor(ORANGE);
                break;
            default:
                break;
            }
            piece.setPosition(chess_board.chess_pieces_arr[i][j].x + OFFSET_X, chess_board.chess_pieces_arr[i][j].y + OFFSET_Y); // 设置棋子的位置
            window.draw(piece);                                                                                                  // 将棋子画在窗口上
        }
    }
}

/**
 * @brief 绘制棋子移动效果
 */
void Game::MovePiece(sf::RenderWindow &window)
{
    if (path.size() == 1) // path中只有一个元素，即终点 -> 移动完成
    {
        is_moving = false;   // 说明一次完整移动完成，设置为静止
        click_twice = false; // 重置鼠标点击次数
        UpdateChessBoard();  // 更新棋盘位置
        UpdateScore();       // 更新分数，如果没有消球的话，生成新的棋子

        int game_over = CheckGameOver(); // 检测游戏是否结束
        if (game_over == 1 || game_over == -1)
            std::cout << "游戏结束" << std::endl;
    }
    else // 当路径不为空，进行移动
    {
        // 我们移动的始终是起点的棋子
        ChessPiece &piece = chess_board.chess_pieces_arr[start_row][start_col];

        std::queue<Point> path_copy = path; // 复制一份path

        int row1 = path_copy.front().row; // 取队头元素
        int col1 = path_copy.front().col;
        path_copy.pop();                  // 队头元素出队
        int row2 = path_copy.front().row; // 取队头元素的下一个元素
        int col2 = path_copy.front().col;

        move_times++;
        if (move_times == 5) // 说明我们移动了一格
        {
            move_times = 0;
            path.pop();
        }

        if (row1 > row2 && col1 == col2) // 上移

            piece.y -= SPEED * PIECE_SIZE / 5.0;

        else if (row1 == row2 && col1 < col2) // 右移

            piece.x += SPEED * PIECE_SIZE / 5.0;

        else if (row1 < row2 && col1 == col2) // 下移

            piece.y += SPEED * PIECE_SIZE / 5.0;

        else if (row1 == row2 && col1 > col2) // 左移

            piece.x -= SPEED * PIECE_SIZE / 5.0;

        sf::sleep(sf::milliseconds(100));
    }
}
