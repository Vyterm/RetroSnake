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


inline bool IsKeyDown(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001;
}

inline E_Direction GetInputDirection(E_Direction direction)
{
	if (IsKeyDown(VK_LEFT) && direction != E_Direction::Right)
		direction = E_Direction::Left;
	else if (IsKeyDown(VK_UP) && direction != E_Direction::Down)
		direction = E_Direction::Up;
	else if (IsKeyDown(VK_RIGHT) && direction != E_Direction::Left)
		direction = E_Direction::Right;
	else if (IsKeyDown(VK_DOWN) && direction != E_Direction::Up)
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
	map[emptyPoints[rand() % emptyPoints.size()]] = E_MapItem::Food;
	return true;
}
static bool G_IsGameOver = false;

void Game()
{
	Map map;
	InitSurface(map);
	Point point = { 40, 30 };
	Snake snake(point);
	map[point] = E_MapItem::Head;
	GenerateRandomFood(map);
	while (!G_IsGameOver)
	{
		DrawMap(map);
		Sleep(100);
		snake.set_direction(GetInputDirection(snake.get_direction()));
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
			break;
		case E_MoveState::Done:
		default:
			break;
		}
	}
}

int main()
{
	SetTitle("贪吃蛇大作战(Console Version) by 郭弈天");
	SetConsoleWindowSize();
	SetColor(10, 0);

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