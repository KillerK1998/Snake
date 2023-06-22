// Sneke.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <stdlib.h>
#include <Windows.h>
using namespace std;
using std::getline;

class Letter {
public:
    string value;
    void setValue(string value) {
        this->value = value;
    }
    string getValue() {
        return value;
    }
};

using LetterPtr = shared_ptr<Letter>;

class Row {
public:
    int width;
    vector<LetterPtr> letters;

    Row(int width) {
        this->width = width;
        for (int count = 0; count < width; count++) {
            LetterPtr letter = make_shared<Letter>();
            letter->setValue(" ");
            letters.emplace_back(letter);
        }
    }

    void print() {
        for (auto letter : letters) {
            cout << letter->getValue();
        }
    }

    void clear() {
        for (auto letter : letters) {
            letter->setValue(" ");
        }
    }

    LetterPtr getLetter(int index) {
        return letters[index];
    }
};

using RowPtr = shared_ptr<Row>;

class Board {
public:
    int width;
    int height;
    vector <RowPtr> rows;

    Board(int width, int height) {
        this->width = width;
        this->height = height;
        rows = vector<RowPtr>();
        for (int count = 0; count < height; count++) {
            RowPtr row = make_shared<Row>(width);
            rows.emplace_back(row);
        }
    }

    void print() {
        for (auto row : rows) {
            row->print();
            cout << "\n";
        }
    }
    void cleanPrint() {
        string output = "";
        for (auto row : rows) {
            for (auto letter : row->letters) {
                output += letter->getValue();
            }
            output += "\n";
        }
        cout << output; 
    }

    RowPtr getRow(int index) {
        return rows[index];
    }

    void setLetter(int letterIndex, int rowIndex, string letterValue) {
        RowPtr secondRow = getRow(rowIndex);
        LetterPtr letterToChange = secondRow->getLetter(letterIndex);
        letterToChange->setValue(letterValue);
    }

    void clear() {
        for (auto row : rows) {
            row->clear();
        }
    }
};

using BoardPtr = shared_ptr<Board>;

class Snake {
public:
    BoardPtr board;
    string text;
    vector<pair<int, int>> segments;
    int vx; 
    int vy; 
    bool ateFood; 

    Snake() {

    }
    Snake(BoardPtr board) {
        this->board = board;
        this->text = "o";
        this->vx = 1;
        this->vy = 0;
        this->ateFood = false;
        // Starting position of the snake
        segments.emplace_back(make_pair(20, 10));
    }

    void draw() {
        for (auto segment : segments) {
            board->setLetter(segment.first, segment.second, text);
        }
    }

    void setDirection(int x_direction, int y_direction) {
        this->vx = x_direction;
        this->vy = y_direction;
    }

    void update() {
        pair<int, int> head = segments[0];
        int new_x = head.first + vx;
        int new_y = head.second + vy;

        // Check if the snake hits itself or the boundaries of the board
        if (hitSelf(new_x, new_y) || hitBoundary(new_x, new_y)) {
            // Game over
            cout << "Game Over!" << endl;
            exit(0);
        }

        segments.insert(segments.begin(), make_pair(new_x, new_y));

        // If snake ate food, don't remove the tail segment
        if (!ateFood) {
            segments.pop_back();
        }
        else {
            ateFood = false;
        }
    }

    bool hitSelf(int x, int y) {
        for (int i = 1; i < segments.size(); i++) {
            if (segments[i].first == x && segments[i].second == y) {
                return true;
            }
        }
        return false;
    }

    bool hitBoundary(int x, int y) {
        if (x < 0 || x >= board->width || y < 0 || y >= board->height) {
            return true;
        }
        return false;
    }

    void eatFood() {
        ateFood = true;
    }
};

class Food {
public:
    BoardPtr board;
    string text;
    int x;
    int y;
    Food() {

    }
    Food(BoardPtr board) {
        this->board = board;
        this->text = "*";
        generatePosition();
    }

    void draw() {
        board->setLetter(x, y, text);
    }

    void generatePosition() {
        // Generate a random position within the board boundaries
        x = getRandomPosition(board->width);
        y = getRandomPosition(board->height);
    }

    int getRandomPosition(int max) {
        // Generate a random position between 0 and max-1
        return rand() % max;
    }
};

class Game {
public:
    Snake snake;
    Food food;
    BoardPtr board;
    Game() {
        this->board = make_shared<Board>(40, 20);
        snake = Snake(board);
        food = Food(board);
    }

    void start() {
        bool gameOver = false;
        while (!gameOver) {
            doInputs();
            doUpdates();
            drawScreen();
            Sleep(200);
        }
    }

    void doUpdates() {
        snake.update();
        checkFoodCollision();
    }

    void doInputs() {
        int x_direction = 0;
        int y_direction = 0;
        if (keyPressed(65))
            x_direction = -1;
        if (keyPressed(68))
            x_direction = 1;
        if (keyPressed(87))
            y_direction = -1;
        if (keyPressed(83))
            y_direction = 1;
        if (x_direction != 0 || y_direction != 0)
            snake.setDirection(x_direction, y_direction);
    }

    bool keyPressed(int key) {
        if (GetKeyState(key) & 0x8000)
            return true;
        return false;
    }

    void checkFoodCollision() {
        pair<int, int> head = snake.segments[0];
        if (head.first == food.x && head.second == food.y) {
            snake.eatFood();
            food.generatePosition();
        }
    }

    void clearScreen() {
        system("cls");
        board->clear();
    }

    void drawScreen() {
        clearScreen();
        food.draw();
        snake.draw();
        board->cleanPrint(); 
    }
};

int main() {
    Game myGame;
    myGame.start();
    return 0;
}
 
