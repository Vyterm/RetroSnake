#include "GameApp.hpp"
#include "GameLib.hpp"
#include "GameSurface.hpp"
#include "GameModel.hpp"
#include "GameCtrl.hpp"
#include "PointerVector.hpp"
#include "GameTimer.hpp"

#include <iostream>
#include <iomanip>
#include <vector>

#include <Windows.h>
#include <conio.h>
#include <time.h>
#include <math.h>
#include <stdlib.h>

using std::cout;
using std::cin;
using std::endl;

constexpr Color FoodColors[] = { {14,0}, {10,0}, {13,0}, {12,0}, {9,0} };
constexpr Color FOOD_COLOR = { 12, 0 };
constexpr Color JUMP_COLOR = { 9 , 0 };
constexpr Color EXIT_COLOR = { 13, 0 };
static constexpr auto SLEEP_DELTA = 10;

inline bool IsKey(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x8000) == 0x8000;
}

inline bool IsKeyDown(int vKey)
{
	return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001;
}

inline bool SearchEmptyPosition(Map &map, Point &emptyPoint)
{
	std::vector<Point> emptyPoints;
	for (int ri = 0; ri < GAME_HEIGHT; ++ri)
		for (int ci = 0; ci < GAME_WIDTH; ++ci)
			if (E_CellType::None == map.Index(ci, ri))
				emptyPoints.push_back({ ci,ri });
	if (0 == emptyPoints.size())
		return false;
	emptyPoint = emptyPoints[rand() % emptyPoints.size()];
	return true;
}

inline bool GenerateEntryPoint(Map &map, const Color &color)
{
	Point emptyPoint;
	if (!SearchEmptyPosition(map, emptyPoint))
		return false;
	map[emptyPoint].Set(E_CellType::Jump, color);
	// ToDo: Temp solution, wait to refactor.
	Point jumpPoint = { rand() % (MAZE_WIDTH - 2) + (GAME_WIDTH + 1),rand() % (MAZE_HEIGHT - 2) + (MSG_HEIGHT + 1) };
	map[emptyPoint].jumpPoint = jumpPoint;
	map[jumpPoint].Set(E_CellType::Jump, color);
	map[jumpPoint].jumpPoint = emptyPoint;

	return true;
}

inline bool GenerateRandomFood(Map &map)
{
	Point emptyPoint;
	if (!SearchEmptyPosition(map, emptyPoint))
		return false;
	auto randomType = rand() % 100;
	E_SubType subType = randomType < 0 ? E_SubType::SubType0 : 
		randomType < 40 ? E_SubType::SubType1 : 
		randomType < 60 ? E_SubType::SubType2 : 
		randomType < 80 ? E_SubType::SubType3 : E_SubType::SubType4;

	map[emptyPoint].Set(E_CellType::Food, subType, FoodColors[int(subType)]);
	return true;
}

static bool G_IsGameOver = false;
static int G_Player1Score = 0, G_Player2Score = 0;

inline bool ProcessSnake(Map &map, PlayerCtrl &player)
{
	auto isEatFood = player.Process(SLEEP_DELTA);
	if (isEatFood)
		return GenerateRandomFood(map);
	return isEatFood;
}

inline void DrawOverPanel(PlayerCtrl &player1, PlayerCtrl &player2)
{
	if (!player1.IsAlive())
	{
		++G_Player2Score;
		OverSurface(player2.get_Name(), player2.get_Color(), true);
		G_IsGameOver = true;
	}
	else if (!player2.IsAlive())
	{
		++G_Player1Score;
		OverSurface(player1.get_Name(), player1.get_Color(), true);
		G_IsGameOver = true;
	}
}

class SecondHandler : public TimerManager::handler
{
	int m_id;
	int m_invokeCount;
public:
	SecondHandler(clock_t invokeTime, bool isLoop, int id) : TimerManager::handler(invokeTime, isLoop), m_invokeCount(0), m_id(id) { }
	void Invoke() override
	{
		SetPosition(m_id*10, 41);
		cout << "Timer" << m_id << "Invoke" << ++m_invokeCount << "  ";
	}
};

void Game()
{
	srand((unsigned)time(nullptr));
	bool isGamePause = false;
	int timer1InvokeCount = 0, timer2InvokeCount = 0;
	auto &timer1 = TimerManager::get_instance().RegisterHandler<SecondHandler>(1000, true, 0);
	auto &timer2 = TimerManager::get_instance().RegisterHandler<SecondHandler>(2000, true, 1);
	Map map;
	InitSurface(map);
	PlayerCtrl player1("玩家一", map, { GAME_WIDTH / 2 + 5,GAME_HEIGHT / 2 }, { 15,0 }, VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT);
	PlayerCtrl player2("玩家二", map, { GAME_WIDTH / 2 - 5,GAME_HEIGHT / 2 }, { 11,0 }, 'W', 'A', 'S', 'D');
	player1.SetEnemy(player2);
	player2.SetEnemy(player1);
	int eatFoodCount = 0;
	GenerateRandomFood(map);
	GenerateRandomFood(map);
	GenerateRandomFood(map);
	GenerateEntryPoint(map, JUMP_COLOR);
	GenerateEntryPoint(map, EXIT_COLOR);
	ShowMsg(G_Player1Score, G_Player2Score, player1, player2);
	while (!G_IsGameOver)
	{
		if (IsKeyDown(VK_SPACE))
			isGamePause = !isGamePause;

		if (isGamePause)
			continue;
		bool updateFlag = false;
		updateFlag |= ProcessSnake(map, player1);
		updateFlag |= ProcessSnake(map, player2);
		if (updateFlag)
			ShowMsg(G_Player1Score, G_Player2Score, player1, player2);
		DrawMap(map);
		TimerManager::get_instance().HandleClock();
		Sleep(SLEEP_DELTA);
		DrawOverPanel(player1, player2);
	}
	TimerManager::get_instance().UnregiserHandler(timer1);
	TimerManager::get_instance().UnregiserHandler(timer2);
}

int main()
{
	SetTitle("贪吃蛇大作战(Console Version) by 郭弈天");
	SetConsoleWindowSize();
	//ResetCursor();

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