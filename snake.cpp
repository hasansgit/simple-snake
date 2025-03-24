#include <iostream>
#include <stdlib.h>
#include <string>
#include <chrono>
#include <thread>

#ifdef _WIN32
	#include <conio.h>
#else
	#include "libs/conio.cpp"
#endif

bool gameOver;

const int WIDTH = 40;
const int HEIGHT = 14;
const int MAX_LEN = (WIDTH - 3) * (HEIGHT - 2);
std::string map;

int snake_len;
int snake_x[MAX_LEN], snake_y[MAX_LEN];

enum Direction
{
	STOP = 0, UP, DOWN, LEFT, RIGHT
};
Direction direction;

int food_x = -1, food_y;

void Setup() {
	srand(static_cast<unsigned int>(time(nullptr)));

	map = std::string(WIDTH - 1, '#') + '\n';
	for (int i = 1; i < HEIGHT - 1; i++) {
		map += '#' + std::string(WIDTH - 3, ' ') + "#\n";
	}
	map += std::string(WIDTH - 1, '#') + '\n';

	snake_len = 1;
	snake_x[0] = WIDTH / 2;
	snake_y[0] = HEIGHT / 2;

	direction = STOP;
}

void clear() {
	std::this_thread::sleep_for(std::chrono::milliseconds(330));
	#ifdef _WIN32
		system("cls");
	#else
		system("clear");
	#endif
}

void Input() {
	if (!_kbhit()) return;
	const char kb = _getch();
	if (kb == 'w' && direction != DOWN) {
		direction = UP;
	} else if (kb == 's' && direction != UP) {
		direction = DOWN;
	} else if (kb == 'a' && direction != RIGHT) {
		direction = LEFT;
	} else if (kb == 'd' && direction != LEFT) {
		direction = RIGHT;
	}
}

void Logic() {
	if (direction == STOP) return;
	for (int i = snake_len - 2; i >= 0; i--) {
		snake_x[i + 1] = snake_x[i];
		snake_y[i + 1] = snake_y[i];
	}

	if (food_x == -1) {
		food_x = rand() % (WIDTH - 3) + 1;
		food_y = rand() % (HEIGHT - 2) + 1;
		map[food_y * WIDTH + food_x] = '$';
	}

	switch (direction) {
	case UP:
		snake_y[0]--;
		break;
	case DOWN:
		snake_y[0]++;
		break;
	case LEFT:
		snake_x[0]--;
		break;
	case RIGHT:
		snake_x[0]++;
		break;
	default:
		break;
	}
	if (map[snake_y[0] * WIDTH + snake_x[0]] == '$') {
		food_x = -1;
		snake_len++;
	} else if (map[snake_y[0] * WIDTH + snake_x[0]] == '#') {
		gameOver = true;
	}
	for (int i = 2; i < snake_len; i++) {
		if (snake_x[0] == snake_x[i] && snake_y[0] == snake_y[i]) {
			gameOver = true;
			break;
		}
	}
}

void Draw() {
	clear();

	map[snake_y[0] * WIDTH + snake_x[0]] = 'O';
	for (int i = 1; i < snake_len - 1; i++) {
		map[snake_y[i] * WIDTH + snake_x[i]] = 'o';
	}
	if (snake_len != 1) map[snake_y[snake_len - 1] * WIDTH + snake_x[snake_len - 1]] = '*';

	std::cout << map << std::endl;
	for (int i = 0; i < snake_len; i++) {
		map[snake_y[i] * WIDTH + snake_x[i]] = ' ';
	}
}

int main() {
	Setup();

	while(!gameOver) {
		Logic();
		Draw();
		Input();
	}

	std::cout << "YOUR SCORE IS " << snake_len << std::endl;

	return 0;
}
