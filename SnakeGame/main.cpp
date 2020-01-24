#include <SFML/Graphics.hpp>
#include <iostream>                             
#include <vector>                                     
#include <time.h>                                      

using namespace sf;

const int N = 30;
const int M = 20;
const int Size = 16;
const int dx[] = {-1, 0, 1, 0};
const int dy[] = {0, -1, 0, 1};

struct Point {
    int x, y;
    Point(int _x = 0, int _y = 0):x(_x), y(_y){};
    bool operator == (Point var) {
        if (this->x == var.x && this->y == var.y) {
            return true;
        }
        return false;
    }
};      

class Game {
    private:
        Point food;
        int direction;
        Clock clock;
        std::vector<Point> snake;

        void CreateNewFood() {
            while (true) {
                bool ok = true;
                food = Point(rand() % N, rand() % M);
                for (auto it:snake) {
                    if (food.x == it.x && food.y == it.y) {
                        ok = false;
                        break;
                    }
                }
                if (ok) break;
            }
        }

    public:
        Game() {
            for (int i = 0; i < 5; i++) {
                snake.push_back(Point(5 - i, 0));
            }
            direction = 2;
            CreateNewFood();
        }

        void setDriection(int x) {
            if (abs(x - direction) != 2) 
                direction = x;
        }

        void Move() {
            if (clock.getElapsedTime().asSeconds() > 0.15) {
                std::vector <Point> temp(snake);
                for (int i = 1; i < (int)snake.size(); i++) {
                    temp[i] = snake[i - 1];
                }
                swap(snake, temp);
                snake[0] = Point(snake[0].x + dx[direction], snake[0].y + dy[direction]);
                snake[0].x = ((snake[0].x < 0) * N + snake[0].x) % N;
                snake[0].y = ((snake[0].y < 0) * M + snake[0].y) % M;
                if (snake[0] == food) {
                    snake.push_back(temp.back());
                    CreateNewFood();
                }
                clock.restart();
            }
        }

        bool Alive() {
            for (int i = 1; i < (int)snake.size(); i++) {
                if (snake[i].x == snake[0].x && snake[i].y == snake[0].y) {
                    return false;
                }
            }
            return true;
        }

        void Draw(Sprite red, RenderWindow &window) {
            for (auto it:snake) {
                red.setPosition(it.x * Size, it.y * Size);
                window.draw(red);
            }
            red.setPosition(food.x * Size, food.y * Size);
            window.draw(red);
        }
} a;

int main() {
    //Init and load data
    RenderWindow window(VideoMode((N + 0) * 16, (M + 0) * 16), "Snake Game!");
    Texture text1, text2;
    text1.loadFromFile("images/white.png");
    text2.loadFromFile("images/red.png");
    Sprite white(text1), red(text2);

    while (window.isOpen()) {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            }
            if (event.type == Event::KeyPressed) {
                int temp;
                switch (event.key.code) {
                    case Keyboard::Left: 
                        temp = 0;
                        break;
                    case Keyboard::Up:
                        temp = 1;
                        break;
                    case Keyboard::Right:
                        temp = 2;
                        break;
                    case Keyboard::Down:
                        temp = 3;
                        break;
                }
                a.setDriection(temp);
            }
        }
        if (!a.Alive()) {
            window.close();
        }
        //Draw
        window.clear(Color::White);
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < M; j++) {
                white.setPosition(i * Size, j * Size);
                window.draw(white);
            }
        }
        a.Draw(red, window);
        a.Move();

        window.display();
    }

    return 0;
}