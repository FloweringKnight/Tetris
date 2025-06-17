#include <SFML/Graphics.hpp>
// using namespace sf;

const int M = 20;
const int N = 10;

int field[M][N] = {0};

struct Point {
    int x, y;
} a[4], b[4];

int figures[7][4] = {
    1, 3, 5, 7, //  _____________
    2, 4, 5, 7, //  |  0  |  1  |
    3, 5, 4, 6, //  |  2  |  3  |
    3, 5, 4, 7, //  |  4  |  5  |
    2, 3, 5, 7, //  |  6  |  7  |
    3, 5, 7, 6, //  |  8  |  9  |
    2, 3, 4, 5, //  -------------
};

bool check() {
    for (const auto &i: a) {
        if (i.x < 0 || i.x > N || i.y >= M) {
            return false;
        } else if (field[i.y][i.x] == 0) {
            return false;
        }
    }
    return true;
}

int main() {
    // srand(time(nullptr));
    sf::RenderWindow window(sf::VideoMode({480, 720}), "Tetris");

    sf::Texture texture("..\\title.png", false, sf::IntRect({0, 0}, {144, 18}));
    sf::Sprite sprite(texture);
    sprite.setTextureRect(sf::IntRect({0, 0}, {18, 18}));

    int dx = 0;
    bool rotate = false;
    int colorNum = 1;

    float timer = 0.0f;
    float delay = 0.3f;
    sf::Clock clock;

    while (window.isOpen()) {
        float time = clock.getElapsedTime().asSeconds();
        clock.restart();
        timer += time;

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)) {
                rotate = true;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) {
                dx = -1;
            } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) {
                dx = 1;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)) {
            delay = 0.05f;
        }

        //MOVE
        for (int i = 0; i < 4; i++) {
            b[i] = a[i];
            a[i].x += dx;
        }
        if (!check()) {
            for (int i = 0; i < 4; i++) {
                a[i] = b[i];
            }
        }
        //rotate
        if (rotate) {
            Point p = a[1];
            for (auto & i : a) {
                int x = i.y - p.y;
                int y = i.x - p.x;
                i.x = p.x - x;
                i.y = p.y + y;
            }
            if (!check()) {
                for (int i = 0; i < 4; i++) {
                    a[i] = b[i];
                }
            }
        }

        //tick
        if (timer > delay) {
            for (int i = 0; i < 4; i++) {
                b[i] = a[i];
                a[i].y += 1;
            }
            if (!check()) {
                for (int i = 0; i < 4; i++) {
                    field[b[i].y][b[i].x] = colorNum;
                }
                colorNum =  1 + rand() % 7;
                int n = rand() % 7;
                for (int i = 0; i < 4; i++) {
                    a[i].x = figures[n][i] % 2;
                    a[i].y = figures[n][i] / 2;
                }
            }
            timer = 0.0f;
        }

        // int n = 3;
        // if (a[0].x == 0) {
        //     for (int i = 0; i < 4; i++) {
        //         a[i].x = figures[n][i] % 2;
        //         a[i].y = figures[n][i] / 2;
        //     }
        // }
        dx = 0;
        rotate = false;
        delay = 0.3f;

        // check line

        int k = M -1;
        for (int i = M-1; i > 0 ; i--) {
            int count = 0;
            for (int j = 0; j < N; j++) {
                if (field[i][j]) {
                    count++;
                }
                field[k][j] = field[i][j];
            }
            if (count < N) {
                k--;
            }
        }

        // draw
        window.clear(sf::Color::White);

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (field[i][j] == 0) {
                    continue;
                }
                sprite.setTextureRect(sf::IntRect({field[i][j]*18, 0}, {18, 18}));
                sprite.setPosition(sf::Vector2f(static_cast<float>(j * 18), static_cast<float>(i * 18)));
                window.draw(sprite);
            }
        }

        for (const auto &i: a) {
            sprite.setTextureRect(sf::IntRect({colorNum*18, 0}, {18, 18}));
            sprite.setPosition({static_cast<float>(i.x * 18), static_cast<float>(i.y * 18)});
            window.draw(sprite);
        }

        window.display();
    }
    return 0;
}
