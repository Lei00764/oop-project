#include <iostream>
#include <cstring>
#include <string>
#include "Game.h"
#include <vector>

/**
 * @brief 初始化文本，中文 宽字符
 *
 */
sf::Text init_text(const std::wstring &s, const sf::Font &font, int font_size = 36)
{
    sf::Text text;
    text.setString(s);                   // 设置字符串
    text.setFont(font);                  // 设置字体
    text.setCharacterSize(font_size);    // 文字大小
    text.setFillColor(sf::Color::Black); // 颜色
    text.setStyle(sf::Text::Bold);       // 样式
    return text;
}

/**
 * @brief 英文或数字
 *
 */
sf::Text init_text(const std::string &s, const sf::Font &font, int font_size = 24)
{
    sf::Text text;
    text.setString(s);                   // 设置字符串
    text.setFont(font);                  // 设置字体
    text.setCharacterSize(font_size);    // 文字大小
    text.setFillColor(sf::Color::Black); // 颜色
    text.setStyle(sf::Text::Bold);       // 样式
    return text;
}

/**
 * @brief 初始化字体
 *
 */
sf::Font init_font(const std::string &s)
{
    sf::Font font;
    if (font.loadFromFile(s))
        std::cout << "font success" << std::endl;
    return font;
}

/**
 * @brief 构造函数
 *
 */
Game::Game()
{
    InitGame(); // 初始化游戏
}

/**
 * @brief 开始游戏
 */
void Game::PlayGame()
{
    window.create(sf::VideoMode(1000, 600), L"五子连珠");

    SetChessPiecesPosition(); // 设置棋子的位置

    font = init_font("../assets/font/LXGWWenKai-Regular.ttf"); // 加载字体
    while (window.isOpen())
    {
        DealWithEvent(); // 处理用户事件

        // 更新
        if (click_twice && !is_moving) // 选中起点和终点，且当前棋子没有移动 -> 寻找路径
        {
            BFS(); // 寻找路径

            if (is_path)          // 有路径
                is_moving = true; // 开始移动
        }

        if (is_moving)       // 棋子正在移动
            DrawMovePiece(); // 绘制棋子移动的动画，移动完成后会更新状态

        window.clear();
        DrawChessBoardBg();    // 绘制棋盘背景
        DrawChessPieces();     // 绘制棋子
        DrawNextThreePieces(); // 绘制接下来应该出现的 3 个棋子
        DrawMessage();         // 绘制结果信息
        DrawScore();           // 绘制游戏总得分
        DrawPrompt();          // 绘制提示信息
        DrawGameOver();        // 绘制游戏结束画面
        window.display();
    }
}

/**
 * @brief 初始化游戏
 *
 */
void Game::InitGame()
{
    start_row = -1;
    start_col = -1;
    end_row = -1;
    end_col = -1;
    score = 0;
    per_score = 0;
    is_moving = false;
    move_times = 0;
    click_twice = false;
    is_path = false;
    auto_score = false;
    game_ove_music_played = false;
    for (int i = 0; i < 7; i++)
        delete_ball_num[i] = 0;

    chess_board.CleanChessBoard(); // 清空棋盘

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

    sound_buffer1.loadFromFile("../assets/sound/move.wav"); // 加载声音缓冲区
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
 * 消除n个球，得 2 * n 分
 *
 */
void Game::IsDelete(int row, int col)
{
    int n = 0; // 统计每次消球的数量

    int color = chess_board.chess_pieces_arr[row][col].color;
    int sumy1 = 0; // 统计连续竖球的数量 该位置 + 向下
    int sumy2 = 0; // 统计连续竖球的数量 不包括 + 向上
    int sumx1 = 0; // 统计连续横球的数量 该位置 + 向右
    int sumx2 = 0; // 统计连续横球的数量 不包括 + 向左
    // 先考虑竖

    per_score = 0; // 统计分数
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
    if (per_score != 0)
        chess_board.chess_pieces_arr[row][col].is_selected = false; // 如果消球，需要取消选中
}

/**
 * @brief 若移动珠子没能消去棋子，则将随机产生 3 个棋子，作为惩罚
 *
 */
void Game::CreateThreePieces()
{
    t_score.clear();                  // 清空 t_score
    sf::sleep(sf::milliseconds(300)); // 休息 0.3s 再生成棋子
    int count = 0;
    while (count < 3)
    {
        int row_pos = rand() % CHESSBOARD_ROWS;
        int col_pos = rand() % CHESSBOARD_COLS;

        if (chess_board.chess_pieces_arr[row_pos][col_pos].color == 0)
        {
            chess_board.chess_pieces_arr[row_pos][col_pos].color = chess_board.next_three_color[count];
            count++;
            // 系统随机产生的珠子正好能凑成了同色的5颗及以上一起排成横向、纵向或者斜向，则这几颗同向的珠子自行消除，游戏者得分
            per_score = 0;
            IsDelete(row_pos, col_pos); // 能否消去棋子
            t_score.push_back(per_score);
        }
    }
    int sum = 0;
    for (int i = 0; i < t_score.size(); i++)
        sum += t_score[i];

    if (sum != 0) // 说明本次运气好，随机产生的棋子有自动消除
    {
        score += sum;      // 把加分加到总得分上
        auto_score = true; // 本次是自动得分
        clock1.restart();  // 重置时钟计数器
    }

    // 更新下次应该显示的棋盘
    for (int i = 0; i < chess_board.next_three_color.size(); i++)
    {
        int color = rand() % 6 + 1;
        chess_board.next_three_color[i] = color;
    }
}

/**
 * @brief 更新棋盘，交换起点和终点棋子
 *
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
 *
 */
void Game::UpdateScore()
{
    IsDelete(end_row, end_col); // 如果有5个及以上的话，会更改arr的值
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
        CreateThreePieces(); // 只有当没有消球时，才会生成新的球
    }
}

/**
 * @brief 检测游戏是否成功
 *
 * @return 胜利则返回1，失败则返回-1，如果继续则返回0
 */
void Game::CheckGameOver()
{
    int sum = chess_board.GerRemainPieces(); // 棋盘中剩余棋子的数量
    if (sum == 0)                            // 棋盘中没有棋子了
    {
        std::cout << "胜利！！！" << std::endl;
        game_over = 1; // 游戏结束
    }
    else if (sum + 3 > CHESSBOARD_COLS * CHESSBOARD_ROWS)
    {
        std::cout << "失败！！！" << std::endl;
        game_over = -1; // 游戏结束
    }
    else
        game_over = 0; // 游戏继续
}

/**
 * @brief 处理所有用户事件
 *
 * @param window
 */
void Game::DealWithEvent()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed) // 关闭窗口
            window.close();

        if (event.type == sf::Event::MouseButtonPressed) // 鼠标点击
        {
            if (is_moving) // 在移动时，不在接受鼠标点击事件
                continue;

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
                sound.setBuffer(sound_buffer1);
                sound.play(); // 播放点击音乐

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
        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::Q)
            {
                std::cout << "按键盘q键，关闭游戏" << std::endl;
                window.close();
            }
            else if (event.key.code == sf::Keyboard::R)
            {
                std::cout << "按键盘r键，重新开始游戏" << std::endl;
                game_over = 0; // 游戏继续
                InitGame();
            }
        }
    }
}

/**
 * @brief 画棋盘背景
 *
 */
void Game::DrawChessBoardBg()
{
    // 加载背景图片
    sf::Texture background_texture;
    if (!background_texture.loadFromFile("../assets/chessboard_bg.png"))
        return;
    sf::Sprite background_sprite;
    background_sprite.setTexture(background_texture);
    window.draw(background_sprite);

    // 加载系统信息背景图片
    sf::Texture message_bg_texture;
    if (!message_bg_texture.loadFromFile("../assets/message_bg.png"))
        return;
    sf::Sprite message_bg_sprite;
    message_bg_sprite.setTexture(message_bg_texture);
    message_bg_sprite.setPosition(550, 60);
    window.draw(message_bg_sprite);

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
 *
 */
void Game::DrawChessPieces()
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
                piece.setFillColor(YELLOW);
                break;
            case 3:
                piece.setFillColor(BLUE);
                break;
            case 4:
                piece.setFillColor(GREEN);
                break;
            case 5:
                piece.setFillColor(ORANGE);
                break;
            case 6:
                piece.setFillColor(PURPLE);
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
 *
 */
void Game::DrawMovePiece()
{
    if (path.size() == 1) // path中只有一个元素，即终点 -> 移动完成
    {
        is_moving = false;        // 说明一次完整移动完成，设置为静止
        click_twice = false;      // 重置鼠标点击次数
        UpdateChessBoard();       // 更新棋盘位置，更改数组中的棋子位置
        UpdateScore();            // 更新分数，如果没有消球的话，生成新的棋子
        SetChessPiecesPosition(); // 设置棋子的位置
        CheckGameOver();          // 检测游戏是否结束

        // 打印棋盘
        chess_board.PrintChessBoard();
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
            piece.y -= PIECE_SIZE / 5.0;
        else if (row1 == row2 && col1 < col2) // 右移
            piece.x += PIECE_SIZE / 5.0;
        else if (row1 < row2 && col1 == col2) // 下移
            piece.y += PIECE_SIZE / 5.0;
        else if (row1 == row2 && col1 > col2) // 左移
            piece.x -= PIECE_SIZE / 5.0;

        sf::sleep(sf::milliseconds(SPEED));
    }
}

/**
 * @brief 绘制游戏结束画面
 *
 */
void Game::DrawGameOver()
{

    if (game_over == 1 || game_over == -1)
    {
        sf::Music music;            // 创建音乐对象
        if (!game_ove_music_played) // 如果音乐还没有播放过
        {
            if (game_over == 1)
                music.openFromFile("../assets/sound/success.wav"); // 加载胜利音乐
            else
                music.openFromFile("../assets/sound/fail.wav"); // 加载失败音乐
            music.play();                                       // 播放音乐

            game_ove_music_played = true; // 将音乐播放标志设置为true
        }

        sf::Texture over_texture;
        if (game_over == 1)
            over_texture.loadFromFile("../assets/success.png"); // 加载图片
        else
            over_texture.loadFromFile("../assets/fail.png");
        sf::Sprite over_sprite;
        over_sprite.setTexture(over_texture);
        over_sprite.setPosition(100, 150);

        // 创建一个空白矩阵
        sf::RectangleShape rectangle(sf::Vector2f(480.f, 70.f));
        rectangle.setFillColor(sf::Color::White);
        rectangle.setPosition(50.f, 400.f);

        std::wstring str = L"本轮游戏总得分：" + std::to_wstring(score);
        sf::Text text = init_text(str, font, 50);
        text.setPosition(50, 400);

        window.draw(over_sprite);
        window.draw(rectangle);
        window.draw(text);

        // 等待音乐播放完毕
        while (music.getStatus() == sf::Music::Playing)
        {
            // 处理窗口事件
            DealWithEvent();
            window.clear();
            DrawChessBoardBg();    // 绘制棋盘背景
            DrawChessPieces();     // 绘制棋子
            DrawNextThreePieces(); // 绘制接下来应该出现的 3 个棋子
            DrawMessage();         // 绘制结果信息
            DrawScore();           // 绘制游戏总得分
            DrawPrompt();          // 绘制提示信息
            window.display();
        }
        game_ove_music_played = true;
    }
}

/**
 * @brief 绘制下3个棋子
 *
 */
void Game::DrawNextThreePieces()
{
    // 画白色背景
    sf::RectangleShape rectangle(sf::Vector2f(300.f, 60.f));
    rectangle.setFillColor(sf::Color::White);
    rectangle.setPosition(PIECES_X - 50, PIECES_Y - 10);
    window.draw(rectangle);

    // 画出下3个棋子
    for (int i = 0; i < 3; i++)
    {
        sf::CircleShape piece(RADIUS); // 半径
        int color = chess_board.next_three_color[i];
        switch (color)
        {
        case 1:
            piece.setFillColor(RED);
            break;
        case 2:
            piece.setFillColor(YELLOW);
            break;
        case 3:
            piece.setFillColor(BLUE);
            break;
        case 4:
            piece.setFillColor(GREEN);
            break;
        case 5:
            piece.setFillColor(ORANGE);
            break;
        case 6:
            piece.setFillColor(PURPLE);
            break;
        default:
            break;
        }
        piece.setPosition(PIECES_X + i * 80, PIECES_Y);
        window.draw(piece);
    }
}

/**
 * @brief 画游戏信息
 *
 */
void Game::DrawMessage()
{
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

    // 当前剩余球的数量 + 百分比
    for (int i = 0; i < 7; i++)
    {
        sf::Text text1 = init_text(std::to_string(ball_color_num[i]), font);
        if (ball_color_num[i] < 10)
            text1.setPosition(MESSAGE_X + 16, MESSAGE_Y + i * 60);
        else
            text1.setPosition(MESSAGE_X, MESSAGE_Y + i * 60);
        window.draw(text1);

        int num1 = ball_color_num[i] * 100 / CHESSBOARD_ROWS / CHESSBOARD_COLS;         // 整数部分
        int num2 = ball_color_num[i] * 10000 / CHESSBOARD_ROWS / CHESSBOARD_COLS % 100; // 小数部分

        sf::Text text2;
        if (num2 < 10)
            text2 = init_text("(" + std::to_string(num1) + "." + std::to_string(num2) + "0%)", font); // 末尾补零
        else
            text2 = init_text("(" + std::to_string(num1) + "." + std::to_string(num2) + "%)", font);

        if (num1 < 10) // 整数部分只有个位数
            text2.setPosition(MESSAGE_X + 40 + 16, MESSAGE_Y + i * 60);
        else
            text2.setPosition(MESSAGE_X + 40, MESSAGE_Y + i * 60);
        window.draw(text2);
    }

    // 消去的球的数量
    for (int i = 0; i < 7; i++)
    {
        sf::Text text = init_text(std::to_string(delete_ball_num[i]), font);
        text.setPosition(MESSAGE_X + 220, MESSAGE_Y + i * 60);
        window.draw(text);
    }
}

/**
 * @brief 绘制游戏总得分
 *
 */
void Game::DrawScore()
{
    std::wstring str = L"得分：" + std::to_wstring(score);
    sf::Text text1 = init_text(str, font, 36);
    text1.setPosition(650, 10);
    window.draw(text1);
}

/**
 * @brief 绘制游戏提示信息
 *
 */
void Game::DrawPrompt()
{
    // 若自动消除，则提示信息 显示 PROMPT_TIME，单位 s
    sf::Text text3;
    elapsed1 = clock1.getElapsedTime();
    int sum = 0; // 随机消球得分
    for (int i = 0; i < t_score.size(); i++)
        sum += t_score[i];
    if (auto_score && sum != 0)
    {
        std::wstring str = L"运气真好，本次自动消球得分" + std::to_wstring(sum);
        text3 = init_text(str, font);

        if (elapsed1.asSeconds() >= PROMPT_TIME)
        {
            text3.setString("");
            auto_score = false;
        }
    }
    text3.setPosition(PROMPT_X, PROMPT_Y);
    window.draw(text3);
}