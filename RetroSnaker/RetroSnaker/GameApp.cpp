#include "GameApp.hpp"
#include "GameLib.hpp"
#include "GameSurface.hpp"

#include <iostream>
#include <iomanip>

#include <Windows.h>
#include <conio.h>

using std::cout;
using std::cin;
using std::endl;

enum class E_Direction
{
	None,
	Left,
	Right,
	Up,
	Down,
};

struct Point
{
	int x, y;
};

struct SnakePart
{
	Point position;
	SnakePart *next, *last;
};

inline bool IsKeyDown(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001;
}


int main()
{
	SetTitle("贪吃蛇大作战(Console Version) by 郭弈天");
	SetConsoleWindowSize();
	SetColor(10, 0);
	E_MapItem map[GAME_WIDTH][GAME_HEIGHT];
	//Map map;
	InitSurface(&map);
	Point point = { 40, 30 };
	map[point.x][point.y] = E_MapItem::Head;
	E_Direction direction = E_Direction::None;
	while (true)
	{
		if (IsKeyDown(VK_LEFT) && direction != E_Direction::Right)
			direction = E_Direction::Left;
		else if (IsKeyDown(VK_UP) && direction != E_Direction::Down)
			direction = E_Direction::Up;
		else if (IsKeyDown(VK_RIGHT) && direction != E_Direction::Left)
			direction = E_Direction::Right;
		else if (IsKeyDown(VK_DOWN) && direction != E_Direction::Up)
			direction = E_Direction::Down;
		auto tmpPoint = point;
		switch (direction)
		{
		case E_Direction::None:
			break;
		case E_Direction::Left:
			point.x -= 1;
			break;
		case E_Direction::Right:
			point.x += 1;
			break;
		case E_Direction::Up:
			point.y -= 1;
			break;
		case E_Direction::Down:
			point.y += 1;
			break;
		default:
			break;
		}
		if (map[point.x][point.y] == E_MapItem::None)
		{
			map[point.x][point.y] = E_MapItem::Head;
			map[tmpPoint.x][tmpPoint.y] = E_MapItem::None;
		}
		else if (map[point.x][point.y] == E_MapItem::Food)
		{
			map[point.x][point.y] = E_MapItem::Head;
			map[tmpPoint.x][tmpPoint.y] = E_MapItem::Body;
		}
		else
			point = tmpPoint;// ToDo: GameOver

		DrawMap(&map);
		Sleep(100);
	}


	cin.get();

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