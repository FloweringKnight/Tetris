#include <iostream>
#include <SFML/Graphics.hpp>
#include "C_RandomNum/RandomNum.h"

// using namespace sf;

constexpr int WIDTH = 10;
constexpr int HEIGHT = 20;

int field[HEIGHT][WIDTH] = {0};

struct Point {
    int x;
    int y;
};

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
    //  |  8  |  9  |
    //  -------------
};

bool check(const Point p) {
    if (p.y >= HEIGHT) {
        return false;
    }
    return true;
}

int main_game(sf::RenderWindow &window, sf::Texture &texture, sf::Sprite &sprite) {
    Point a[4] = {0, 0};
    int LR_move = WIDTH / 2;
    int DOWN_move = 0;
    int Rotate = 0;
    int ColorNum = RandomNum::range(1,8);
    int BlockNum = RandomNum::range(0,6);
    float timer = 0.f;
    float delay = 0.3f;
    sf::Clock clock;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        const int dx = LR_move;

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

        s_Figures block_temp = figures_new[BlockNum][Rotate];
        int figures_temp[4] = {block_temp.a, block_temp.b, block_temp.c, block_temp.d};

        window.clear(sf::Color::White);


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
            sprite.setTextureRect(sf::IntRect({ColorNum * 18, 0}, {18, 18}));
            sprite.setPosition(sf::Vector2f(static_cast<float>(a[i].x * 19), static_cast<float>(a[i].y * 19)));
            window.draw(sprite);
        }
        if (timer > delay) {
            DOWN_move += 1;
            timer = 0.f;
        }
        delay = 0.3f;
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
        int bool_tmp = 0;
        for (const auto &[x, y] : a) {
            if (field[y][x] != 0 || y >= HEIGHT) {
                bool_tmp += 1;
            }else {
                bool_tmp += 0;
            }
        }
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
        for (int i = 0; i < HEIGHT; i++) {
            for (int j = 0; j < WIDTH; j++) {
                if (field[i][j] != 0) {
                    sprite.setTextureRect(sf::IntRect({field[i][j] * 18, 0}, {18, 18}));
                    sprite.setPosition(sf::Vector2f(static_cast<float>(j * 19), static_cast<float>(i * 19)));
                    window.draw(sprite);
                }
            }
        }
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
            cancel_tmp = 0;
        }

        int fail_check = 0;
        for (int j = 0; j < WIDTH; ++j) {
            fail_check += field[0][j];
        }
        if (fail_check > 0 ){return 0;}
        window.display();
    }
    return 0;
}


int main() {
    sf::RenderWindow window(sf::VideoMode(sf::Vector2u(320, 480), 24), "Tetris");
    window.setFramerateLimit(60);
    sf::Texture texture("../title.png");
    sf::Sprite sprite(texture);

    main_game(window, texture, sprite);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (event->is<sf::Event::KeyPressed>()) {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) window.close();
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space)) {
                    for (auto & i : field) {
                        for (int & j : i) {
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
