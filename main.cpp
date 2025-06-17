#include <iostream>
#include <SFML/Graphics.hpp>
#include "C_RandomNum/RandomNum.h"

// using namespace sf;

//设置游戏区域宽、高
constexpr int WIDTH = 10;
constexpr int HEIGHT = 20;

//  存储落地后方块的变量
int field[HEIGHT][WIDTH] = {0};

//  每个方块的坐标，每个俄罗斯方块由四个小方块组成，故结构体通常实例化为4个成员的数组，可表示一个完整的俄罗斯方块的坐标
struct Point {
    int x;
    int y;
};

//  构建俄罗斯方块，4个小方块共有7种组合，每种组合有0° 90° 180° 270°共四种旋转
//  由于计算得出的旋转坐标可能会产生奇怪的空行或空列，故将每种方块的旋转也定义好，根据旋转计算坐标即可
struct s_Figures{int a,b,c,d;};

constexpr s_Figures figures_new[7][4] = {
    s_Figures(0,2,4,6),s_Figures(1,3,5,7),s_Figures(1,3,5,7),s_Figures(0,2,4,6),
    s_Figures(0,2,3,5),s_Figures(0,2,3,5),s_Figures(2,4,5,7),s_Figures(2,4,5,7),
    s_Figures(1,3,2,4),s_Figures(1,3,2,4),s_Figures(3,5,4,6),s_Figures(3,5,4,6),
    s_Figures(1,2,3,5),s_Figures(1,2,3,5),s_Figures(3,4,5,7),s_Figures(3,4,5,7),
    s_Figures(0,1,3,5),s_Figures(0,1,3,5),s_Figures(2,3,5,7),s_Figures(2,3,5,7),
    s_Figures(1,3,4,5),s_Figures(1,3,4,5),s_Figures(6,3,5,7),s_Figures(6,3,5,7),
    s_Figures(1,3,0,2),s_Figures(1,3,0,2),s_Figures(4,6,5,7),s_Figures(4,6,5,7),
    //  _____________
    //  |  0  |  1  |
    //  |  2  |  3  |
    //  |  4  |  5  |
    //  |  6  |  7  |
    //  -------------
    //  简单对应上图，可从数字识别出是哪种组合方式
};

// 游戏主进程函数
void main_game(sf::RenderWindow &window, sf::Texture &texture, sf::Sprite &sprite) {
    Point a[4] = {0, 0};                                  //  方块坐标实例化
    int LR_move = WIDTH / 2;                              //  方块左右便宜量
    int DOWN_move = 0;                                    //  方块上下偏移量
    int Rotate = 0;                                       //  方块旋转标识，初始为0
    int ColorNum = RandomNum::range(1,8);             //  共8种颜色，随机选择一种颜色作为初始，注：颜色从1开始
    int BlockNum = RandomNum::range(0,6);             //  共7种组合，随机选择一种俄罗斯方块作为开始
    float timer = 0.f;                                    //  计时器
    float delay = 0.3f;                                   //  下落速度
    sf::Clock clock;                                      //  钟表实例

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        const int dx = LR_move;     //  记录计算前的左右偏移量，如边际检测不合法，需要将左右偏移量再赋值回去

        //  设置按键    ↑：变形；  ↓：加速  ←→：移动  Esc：关闭游戏窗口
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) Rotate = (Rotate + 1) % 4;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) delay = 0.05f;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) LR_move -= 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) LR_move += 1;
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();
            }
        }

        //  构建临时变量，根据是哪种俄罗斯方块与旋转偏移量获取方块的坐标特征值
        s_Figures block_temp = figures_new[BlockNum][Rotate];
        int figures_temp[4] = {block_temp.a, block_temp.b, block_temp.c, block_temp.d};

        //  由于下方计算坐标后存在显示动作，故在此清理画布
        window.clear(sf::Color::White);

        //  根据每个俄罗斯方块的四个小方块坐标特征值，循环计算坐标，由于旋转偏移量不同，计算方式有所不同
        for (int i = 0; i < 4; i++) {
            switch (Rotate) {
                case 0:
                    a[i].x = figures_temp[i] % 2 + LR_move;
                    a[i].y = figures_temp[i] / 2 + DOWN_move;
                    break;
                case 1:
                    a[i].x = figures_temp[i] / 2 + LR_move;
                    a[i].y = (figures_temp[i] + 1) % 2 + DOWN_move;
                    break;
                case 2:
                    a[i].x = (figures_temp[i] + 1) % 2 + LR_move;
                    a[i].y = (3 - figures_temp[i] / 2) + DOWN_move;
                    break;
                case 3:
                    a[i].x = 3 - figures_temp[i] / 2 + LR_move;
                    a[i].y = figures_temp[i] % 2 + DOWN_move;
                    break;
                default: continue;
            }

            //  图片资源为8个不同颜色的 18 * 18小方格组成，此处根据ColorNum选择对应的颜色区域，用来生成小方块
            sprite.setTextureRect(sf::IntRect({ColorNum * 18, 0}, {18, 18}));
            //  根据计算出的坐标，画出小方块
            //  注：计算坐标时根据方块数来计算，以每个方块的长宽均为1，但实际上每个方块都是18px * 18px的小格子，故需将坐标值乘以18
            //  注2：经测试，每个方块中间夹1像素的空白效果较好，故小方格按照19px来生成
            sprite.setPosition(sf::Vector2f(static_cast<float>(a[i].x * 19), static_cast<float>(a[i].y * 19)));
            //  循环draw出小格子
            window.draw(sprite);
        }

        // 计时器一旦超过了delay，俄罗斯方块的上下偏移量增加1，计时器重新开始计时
        if (timer > delay) {
            DOWN_move += 1;
            timer = 0.f;
        }

        //  这里重置了下落速度，因 ↓ 键会修改速度，放在这里重置
        delay = 0.3f;

        //  左右偏移检测，如偏移量使俄罗斯方块中的某一个或多个方块的位置变得不合法，则将左右偏移量重置为上一个偏移量
        //  这里可能会出现一个残影，因为左右偏移量在某一瞬间是非法的，检测到非法之后重置为合法，故至少有一瞬间可以看到方块位置非法，存在优化空间
        //  这里同时检测了方块在空中的左右碰撞，但存在BUG，需要优化，需特别注意
        int move_tmp = 0;
        for (const auto &[x, y] : a) {
            if (x < 0 || x > WIDTH-1 || field[y][x] != 0) {
                move_tmp += 1;
            }else{
                move_tmp += 0;
            }
        }
        if (move_tmp > 0) {
            LR_move = dx;
        }

        //  下落检测，下落到底后，由field变量接管画图工作
        //  检测到纵坐标非法后，将此时的方块的坐标传给field，需要注意field二维数组第一个数字表示行，代表纵坐标，第二个数字表示列，代表横坐标
        int bool_tmp = 0;
        for (const auto &[x, y] : a) {
            if (field[y][x] != 0 || y >= HEIGHT) {
                bool_tmp += 1;
            }else {
                bool_tmp += 0;
            }
        }
        //  下落检测完成后，重置方块的左右偏移量、上下偏移量、颜色、俄罗斯方块类型，从而可以在初始位置生成下一个方块
        //  下落后的方块显示由field接管
        if (bool_tmp > 0) {
            DOWN_move = 0;
            LR_move = WIDTH / 2;
            for (auto &[x, y]: a) {
                field[y - 1][x] = ColorNum;
                x = 0;
                y = 0;
            }
            ColorNum = RandomNum::range(1, 8);
            BlockNum = RandomNum::range(0, 6);
        }
        //  以对应的颜色draw出field记录的方块
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (field[i][j] != 0) {
                    sprite.setTextureRect(sf::IntRect({field[i][j] * 18, 0}, {18, 18}));
                    sprite.setPosition(sf::Vector2f(static_cast<float>(j * 19), static_cast<float>(i * 19)));
                    window.draw(sprite);
                }
            }
        }

        //  整行消去检测
        //  循环查找field，从下向上检测
        //  若某一行所有的小方块均有颜色，则从此行开始，循环将上面一行赋值给下面一行
        int cancel_tmp = 0;
        for (int i = HEIGHT-1; i >= 0; i--) {
            for (int j = 0; j < WIDTH; j++) {
                if (field[i][j] != 0) {
                    cancel_tmp += 1;
                }
            }
            if (cancel_tmp == WIDTH) {
                for (int tmp = i ; tmp > 0; tmp--) {
                    for (int j = 0; j < WIDTH; j++) {
                        field[tmp][j] = field[tmp-1][j];
                    }
                }
            }
            //  循环检测过程变量重置
            cancel_tmp = 0;
        }

        //  游戏失败检测
        //  检测field的第0行，如果存在颜色方块，游戏即宣告失败，返回。
        int fail_check = 0;
        for (int j = 0; j < WIDTH; ++j) {
            fail_check += field[0][j];
        }
        if (fail_check > 0 ){break;}
        window.display();
    }
}

//  程序主函数
int main() {
    //  设置游戏窗口 320*480，名称为Tetris
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(320, 480), 24), "Tetris");
    window.setFramerateLimit(60);
    //  加载资源
    sf::Texture texture("../title.png");
    //  设置精灵
    sf::Sprite sprite(texture);

    //  调用游戏主进程，开启一局，失败后进程会自动结束
    main_game(window, texture, sprite);

    //  游戏结束后，根据游戏最终时的field绘制画面，并停在此处
    //  设置按键    Esc：关闭窗口    空格：重新开始游戏（即重置field后重新运行游戏主进程）
    //  若长时间停留在此画面，会造成CPU使用率不断升高，需要继续学习SFML的垃圾回收机制，存在很大的优化空间。
    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                    for (auto &i: field) {
                        for (int &j: i) {
                            j = 0;
                        }
                    }
                    main_game(window, texture, sprite);
                }
            }
        }
    }
    return 0;
}
