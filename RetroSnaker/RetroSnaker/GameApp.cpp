#include "GameApp.hpp"
#include "GameLib.hpp"
#include "GameSurface.hpp"
#include "GameModel.hpp"

#include <iostream>
#include <iomanip>
#include <vector>

#include <Windows.h>
#include <conio.h>
#include <time.h>

using std::cout;
using std::cin;
using std::endl;

constexpr Color FOOD_COLOR = { 12, 0 };

inline bool IsKeyDown(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001;
}

inline E_Direction GetInputDirection(E_Direction direction, int kLeft = VK_LEFT, int kRight = VK_RIGHT, int kUp = VK_UP, int kDown = VK_DOWN)
{
	if (IsKeyDown(kLeft) && direction != E_Direction::Right)
		direction = E_Direction::Left;
	else if (IsKeyDown(kUp) && direction != E_Direction::Down)
		direction = E_Direction::Up;
	else if (IsKeyDown(kRight) && direction != E_Direction::Left)
		direction = E_Direction::Right;
	else if (IsKeyDown(kDown) && direction != E_Direction::Up)
		direction = E_Direction::Down;
	return direction;
}

inline bool GenerateRandomFood(Map &map)
{
	srand((unsigned)time(nullptr));
	std::vector<Point> emptyPoints;
	for (int ri = 0; ri < GAME_HEIGHT; ++ri)
		for (int ci = 0; ci < GAME_WIDTH; ++ci)
			if (E_MapItem::None == map.Index(ci, ri))
				emptyPoints.push_back({ ci,ri });
	if (0 == emptyPoints.size())
		return false;
	auto point = emptyPoints[rand() % emptyPoints.size()];
	map[point] = E_MapItem::Food;
	map.ColorIndex(point) = FOOD_COLOR;
	return true;
}
static bool G_IsGameOver = false;

inline Snake CreateSnake(Map &map, Point point, Color color)
{
	Snake snake(point, color);
	map[point] = E_MapItem::Head;
	map.ColorIndex(point) = color;
	return snake;
}

inline void ProcessSnake(Map &map, Snake &snake, int &eatFoodCount)
{
	switch (snake.MoveByDirection(map))
	{
	case E_MoveState::Over:
		OverSurface(false);
		G_IsGameOver = true;
		break;
	case E_MoveState::Eat:
		if (!GenerateRandomFood(map))
		{
			OverSurface(true);
			G_IsGameOver = true;
		}
		++eatFoodCount;
		break;
	case E_MoveState::Done:
	default:
		break;
	}
}

void Game()
{
	Map map;
	InitSurface(map);
	Snake snake1 = CreateSnake(map, { 40,30 }, { 10,0 });
	Snake snake2 = CreateSnake(map, { 30,30 }, { 9,0 });
	int eatFoodCount = 0;
	GenerateRandomFood(map);
	while (!G_IsGameOver)
	{
		DrawMap(map);
		Sleep(100 - eatFoodCount/5);
		snake1.set_direction(GetInputDirection(snake1.get_direction()));
		ProcessSnake(map, snake1, eatFoodCount);
		snake2.set_direction(GetInputDirection(snake2.get_direction(), 'A', 'D', 'W', 'S'));
		ProcessSnake(map, snake2, eatFoodCount);
	}
}

int main()
{
	SetTitle("贪吃蛇大作战(Console Version) by 郭弈天");
	SetConsoleWindowSize();

	char c = '\0';
	while ('q' != c)
	{
		G_IsGameOver = false;
		Game();
		fflush(stdin);
		c = '\0';
		while ('q' != c && 'r' != c)
			c = _getch();
	}

	return 0;
}

void TestGetAsyncKeyState()
{
	int invokeCount = 0;
	while (true)
	{
		Sleep(80);
		SetPosition(0, 0);
		SHORT key;
		key = GetAsyncKeyState(VK_LEFT);
		cout << std::hex << std::setw(4) << std::setfill('0') << key << endl;
		key = GetAsyncKeyState(VK_RIGHT);
		cout << std::hex << std::setw(4) << std::setfill('0') << key << endl;
		key = GetAsyncKeyState(VK_UP);
		cout << std::hex << std::setw(4) << std::setfill('0') << key << endl;
		key = GetAsyncKeyState(VK_DOWN);
		cout << std::hex << std::setw(4) << std::setfill('0') << key << endl;
		cout << "InvokeCount : " << ++invokeCount << endl;
	}
}