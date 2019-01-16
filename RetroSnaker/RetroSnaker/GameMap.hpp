#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "GameLib.hpp"
#include "GameColor.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <vector>

using std::cout;
using std::cin;
using std::endl;
using std::string;

enum class E_CellType
{
	None = 0,
	Wall = 1,
	Food = 2,
	Head = 3,
	Body = 4,
	Jump = 5,
};

enum class E_SubType
{
	SubType0,
	SubType1,
	SubType2,
	SubType3,
	SubType4,
	SubType5,
	SubType6,
	SubType7,
	SubType8,
};

struct Point
{
public:
	int x, y;
	bool operator==(const Point &rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Point &rhs) const { return x != rhs.x || y != rhs.y; }
	void Set(const Point& point) { x = point.x; y = point.y; }
};

static constexpr auto SLEEP_DELTA = 10;
constexpr E_Color FoodColors[] = { E_Color::LYellow, E_Color::LGreen, E_Color::LPurple, E_Color::LRed, E_Color::LBlue };
class PlayerCtrl;

class MapItem
{
public:
	E_CellType type = E_CellType::None;
	E_SubType subType = E_SubType::SubType0;
	Color color = { DEFAULT_FORE_COLOR, DEFAULT_BACK_COLOR };
	Point jumpPoint;
	MapItem() { }
	MapItem(E_CellType type) { this->type = type; }
	MapItem(E_CellType type, Color color) { this->type = type; this->color = color; }
	MapItem& operator=(const MapItem &rhs)
	{
		this->type = rhs.type;
		this->subType = rhs.subType;
		this->color = rhs.color;
		this->jumpPoint = rhs.jumpPoint;
		return *this;
	}

	void SetQuote(const std::shared_ptr<void> &quote)
	{
		m_weakQuote = quote;
	}
	bool TryGetQuote(std::shared_ptr<void> &quote)
	{
		quote = m_weakQuote.lock();
		return m_weakQuote.expired();
	}
	/*
	亮	红	绿	蓝
	1	1	1	1
	0x0~0xF表示暗黑色至亮白色
	*/
	void Set(Color color) { Set(type, subType, color); }
	void Set(E_CellType type, Color color) { Set(type, E_SubType::SubType0, color); }
	void Set(E_CellType type, E_SubType subType, Color color)
	{
		this->type = type;
		this->subType = subType;
		this->color = color;
	}
	MapItem& operator=(E_CellType type)
	{
		this->type = type;
		return *this;
	}
	MapItem& operator=(E_SubType subType)
	{
		this->subType = subType;
		return *this;
	}
	MapItem& operator=(Color color)
	{
		this->color = color;
		return *this;
	}
	bool operator==(const MapItem &rhs) const { return type == rhs.type && subType == rhs.subType && color == rhs.color; }
	bool operator!=(const MapItem &rhs) const { return type != rhs.type || subType != rhs.subType || color != rhs.color; }
	bool operator==(const E_CellType &rhs) const { return rhs == type; }
	bool operator==(const E_SubType &rhs) const { return rhs == subType; }
	bool operator==(const Color &rhs) const { return rhs == color; }
	friend bool operator==(const E_CellType &lhs, const MapItem &rhs) { return lhs == rhs.type; }
	friend bool operator==(const E_SubType &lhs, const MapItem &rhs) { return lhs == rhs.subType; }
	friend bool operator==(const Color &lhs, const MapItem &rhs) { return lhs == rhs.color; }
private:
	std::weak_ptr<void> m_weakQuote;
};

template <int Width, int Height>
class MapModelTemplate
{
	MapItem m_items[Width][Height];
public:
	const MapItem& operator[](Point position) const { return m_items[position.x][position.y]; }
	MapItem& operator[](Point position) { return m_items[position.x][position.y]; }
	const MapItem& Index(int x, int y) const { return m_items[x][y]; }
	MapItem& Index(int x, int y) { return m_items[x][y]; }
};

template <int Width, int Height>
class MapTemplate
{
	Point m_position;
	static const string _images[];
	MapItem m_items[Width][Height];
	MapItem m_staticItems[Width][Height];
	MapItem m_zCacheItems[Width][Height];

	void DrawCell(int x, int y, bool isForce)
	{
		auto item = m_items[x][y] == E_CellType::None ? m_staticItems[x][y] : m_items[x][y];
		if (!isForce && m_zCacheItems[x][y] == item)
			return;
		m_zCacheItems[x][y] = item;
		SetPosition(m_position.x + x, m_position.y + y);
		auto color = m_zCacheItems[x][y].color;
		SetColor(color);
		cout << ToString(m_zCacheItems[x][y]);
	}
	void SetTree(int treeX, int treeY)
	{
		m_staticItems[treeX - 1][treeY] = m_staticItems[treeX][treeY] = m_staticItems[treeX + 1][treeY]
			= m_staticItems[treeX][treeY - 1] = m_staticItems[treeX][treeY + 1] = E_CellType::Wall;
		m_staticItems[treeX - 1][treeY] = m_staticItems[treeX][treeY] = m_staticItems[treeX + 1][treeY]
			= m_staticItems[treeX][treeY - 1] = m_staticItems[treeX][treeY + 1] = DEFAULT_COLOR;
	}

	void SetBorder(int startPosX, int width, int startPosY, int height)
	{
		int endPosX = startPosX + width - 1, endPosY = startPosY + height - 1;
		for (int ri = startPosY; ri <= endPosY; ++ri)
			for (int ci = startPosX; ci <= endPosX; ++ci)
				m_staticItems[ci][ri] = (ci == startPosX || ci == endPosX || ri == startPosY || ri == endPosY) ? E_CellType::Wall : E_CellType::None;
	}

	bool SearchEmptyPosition(Point &emptyPoint)
	{
		std::vector<Point> emptyPoints;
		for (int ri = 0; ri < GAME_HEIGHT; ++ri)
			for (int ci = 0; ci < GAME_WIDTH; ++ci)
				if (E_CellType::None == m_staticItems[ci][ri] && E_CellType::None == m_items[ci][ri])
					emptyPoints.push_back({ ci,ri });
		if (0 == emptyPoints.size())
			return false;
		emptyPoint = emptyPoints[rand() % emptyPoints.size()];
		return true;
	}
public:
	MapTemplate()
	{
		m_position = { 0, 0 };
		SetBorder(0, GAME_WIDTH, 0, GAME_HEIGHT);
		SetBorder(GAME_WIDTH, MAZE_WIDTH, MSG_HEIGHT, MAZE_HEIGHT);
		SetBorder(GAME_WIDTH, MSG_WIDTH, 0, MSG_HEIGHT);
		SetTree(5, 5);
		SetTree(34, 5);
		SetTree(5, 34);
		SetTree(34, 34);
	}
	const MapItem& operator[](Point position) const { return m_items[position.x][position.y]; }
	MapItem& operator[](Point position) { return m_items[position.x][position.y]; }
	const MapItem& Index(int x, int y) const { return m_items[x][y]; }
	MapItem& Index(int x, int y) { return m_items[x][y]; }
	E_CellType GetType(Point position) { return E_CellType::None == m_items[position.x][position.y] ? m_staticItems[position.x][position.y].type : m_items[position.x][position.y].type; }

	static const string& ToString(const MapItem &item) { return _images[int(item.type)]; }

	void Init()
	{
		for (int ri = 0; ri < Height; ++ri)
			for (int ci = 0; ci < Width; ++ci)
				m_zCacheItems[ci][ri] = m_items[ci][ri] = MapItem();
	}

	void Draw(bool isForce = false)
	{
		for (int ri = 0; ri < Height; ++ri)
			for (int ci = 0; ci < Width; ++ci)
				DrawCell(ci, ri, isForce);
	}
	
	bool GenerateRandomFood()
	{
		Point emptyPoint;
		if (!SearchEmptyPosition(emptyPoint))
			return false;
		auto randomType = rand() % 100;
		E_SubType subType = randomType < 0 ? E_SubType::SubType0 :
			randomType < 40 ? E_SubType::SubType1 :
			randomType < 60 ? E_SubType::SubType2 :
			randomType < 80 ? E_SubType::SubType3 : E_SubType::SubType4;

		m_items[emptyPoint.x][emptyPoint.y].Set(E_CellType::Food, subType, { FoodColors[int(subType)] ,DEFAULT_BACK_COLOR });
		return true;
	}
	bool GenerateRandomFood(size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			if (!GenerateRandomFood())
				return false;
		return true;
	}
	bool GenerateEntryPoint(const Color &color)
	{
		Point emptyPoint;
		if (!SearchEmptyPosition(emptyPoint))
			return false;
		m_items[emptyPoint.x][emptyPoint.y].Set(E_CellType::Jump, color);
		// ToDo: Temp solution, wait to refactor.
		Point jumpPoint = { rand() % (MAZE_WIDTH - 2) + (GAME_WIDTH + 1),rand() % (MAZE_HEIGHT - 2) + (MSG_HEIGHT + 1) };
		m_items[emptyPoint.x][emptyPoint.y].jumpPoint = jumpPoint;
		m_items[jumpPoint.x][jumpPoint.y].Set(E_CellType::Jump, color);
		m_items[jumpPoint.x][jumpPoint.y].jumpPoint = emptyPoint;

		return true;
	}
};
typedef MapTemplate<GAME_WIDTH + MAZE_WIDTH, GAME_HEIGHT> Map;
typedef MapTemplate<MAZE_WIDTH, GAME_HEIGHT> Maze;

const string Map::_images[] = { "  ", "■", "食", "◎", "¤", "※" };
//const string Map::_images[] = { "  ", "〓", "❀", "◎", "¤", "※" };
#endif
