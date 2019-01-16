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
	Land = 1,
	Food = 2,
	Head = 3,
	Body = 4,
	Jump = 5,
};

enum class E_SubType
{
	// 作为食物是普通食物，作为地形是山峰
	SubType0,
	// 作为食物使对方变长，作为地形是草地
	SubType1,
	// 作为食物使自己变短，作为地形是
	SubType2,
	// 作为食物使自己增速，作为地形是岩浆
	SubType3,
	// 作为食物使对方减速，作为地形是冰块
	SubType4,
	// 作为食物使对方反转，作为地形是
	SubType5,
	// 作为食物自己变刚体，作为地形是
	SubType6,
	// 作为食物对方变失控，作为地形是
	SubType7,
	// 作为食物自己变幽灵，作为地形是
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
constexpr E_Color SubTypeColors[] = { E_Color::White, E_Color::LGreen, E_Color::LPurple, E_Color::LRed, E_Color::LBlue, E_Color::Black, E_Color::LYellow, E_Color::Green, E_Color::Cyan };
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
	bool operator!=(const E_CellType &rhs) const { return rhs != type; }
	bool operator==(const E_SubType &rhs) const { return rhs == subType; }
	bool operator!=(const E_SubType &rhs) const { return rhs != subType; }
	bool operator==(const Color &rhs) const { return rhs == color; }
	bool operator!=(const Color &rhs) const { return rhs != color; }
	friend bool operator==(const E_CellType &lhs, const MapItem &rhs) { return lhs == rhs.type; }
	friend bool operator==(const E_SubType &lhs, const MapItem &rhs) { return lhs == rhs.subType; }
	friend bool operator==(const Color &lhs, const MapItem &rhs) { return lhs == rhs.color; }
private:
	std::weak_ptr<void> m_weakQuote;
};

template <int Width, int Height>
class MapModelTemplate
{
protected:
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
		auto item = MixCell(x, y);
		if (!isForce && m_zCacheItems[x][y] == item)
			return;
		m_zCacheItems[x][y] = item;
		SetPosition(m_position.x + x, m_position.y + y);
		auto color = m_zCacheItems[x][y].color;
		SetColor(color);
		cout << ToString(m_zCacheItems[x][y]);
	}
	void SetTree(int treeX, int treeY, const Color &color = DEFAULT_COLOR)
	{
		m_staticItems[treeX - 1][treeY] = m_staticItems[treeX][treeY] = m_staticItems[treeX + 1][treeY]
			= m_staticItems[treeX][treeY - 1] = m_staticItems[treeX][treeY + 1] = E_CellType::Land;
		m_staticItems[treeX - 1][treeY] = m_staticItems[treeX][treeY] = m_staticItems[treeX + 1][treeY]
			= m_staticItems[treeX][treeY - 1] = m_staticItems[treeX][treeY + 1] = color;
	}

	void SetBorder(int startPosX, int width, int startPosY, int height)
	{
		int endPosX = startPosX + width - 1, endPosY = startPosY + height - 1;
		for (int ri = startPosY; ri <= endPosY; ++ri)
			for (int ci = startPosX; ci <= endPosX; ++ci)
				m_staticItems[ci][ri] = (ci == startPosX || ci == endPosX || ri == startPosY || ri == endPosY) ? E_CellType::Land : E_CellType::None;
	}

	void SetGrassland(int startPosX, int width, int startPosY, int height)
	{
		int endPosX = startPosX + width - 1, endPosY = startPosY + height - 1;
		for (int ri = startPosY; ri <= endPosY; ++ri)
			for (int ci = startPosX; ci <= endPosX; ++ci)
				m_staticItems[ci][ri].Set(E_CellType::Land, E_SubType::SubType1, {DEFAULT_BACK_COLOR, SubTypeColors[int(E_SubType::SubType1)]});
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
	MapItem MixCell(int x, int y)
	{
		MapItem upperLayer = m_items[x][y] == E_CellType::None ? m_staticItems[x][y] : m_items[x][y];
		if (upperLayer.type != E_CellType::Body && upperLayer.type != E_CellType::Head) return upperLayer;
		if (m_staticItems[x][y] != E_CellType::Land || m_staticItems[x][y] != E_SubType::SubType1) return upperLayer;
		auto type = upperLayer.type == E_CellType::Head ? E_CellType::Head : E_CellType::Land;
		auto subType = upperLayer.type == E_CellType::Head ? E_SubType::SubType0 : E_SubType::SubType1;
		Color color = { upperLayer.color.fore, m_staticItems[x][y].color.back };
		upperLayer.Set(type, subType, color);
		return upperLayer;
	}
public:
	MapTemplate()
	{
		m_position = { 0, 0 };
		SetBorder(0, GAME_WIDTH, 0, GAME_HEIGHT);
		SetBorder(GAME_WIDTH, MAZE_WIDTH, MSG_HEIGHT, MAZE_HEIGHT);
		SetBorder(GAME_WIDTH, MSG_WIDTH, 0, MSG_HEIGHT);
		SetGrassland(10, 20, 1, 10);
		SetTree(5, 5);
		SetTree(34, 5);
		SetTree(5, 34);
		SetTree(34, 34);
	}
	const MapItem& operator[](Point position) const { return m_items[position.x][position.y]; }
	MapItem& operator[](Point position) { return m_items[position.x][position.y]; }
	const MapItem& Index(int x, int y) const { return m_items[x][y]; }
	MapItem& Index(int x, int y) { return m_items[x][y]; }
	const MapItem& GetItem(Point position) { return E_CellType::None == m_items[position.x][position.y] ? m_staticItems[position.x][position.y] : m_items[position.x][position.y]; }

	static string ToString(const MapItem &item)
	{
		if (item.type == E_CellType::Land && item.subType == E_SubType::SubType1)
			return "≡";
		return _images[int(item.type)];
	}

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
			randomType < 15 ? E_SubType::SubType1 :
			randomType < 30 ? E_SubType::SubType2 :
			randomType < 50 ? E_SubType::SubType3 :
			randomType < 70 ? E_SubType::SubType4 :
			randomType < 75 ? E_SubType::SubType5 : 
			randomType < 80 ? E_SubType::SubType6 : E_SubType::SubType7;

		m_items[emptyPoint.x][emptyPoint.y].Set(E_CellType::Food, subType, { SubTypeColors[int(subType)] ,DEFAULT_BACK_COLOR });
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
	bool IsBlocked(const Point &position)
	{
		bool isBlocked = true;
		isBlocked &= E_CellType::None != GetItem({ position.x + 1, position.y }).type;
		isBlocked &= E_CellType::None != GetItem({ position.x - 1, position.y }).type;
		isBlocked &= E_CellType::None != GetItem({ position.x, position.y + 1 }).type;
		isBlocked &= E_CellType::None != GetItem({ position.x, position.y - 1 }).type;
		return isBlocked;
	}
};
typedef MapTemplate<GAME_WIDTH + MAZE_WIDTH, GAME_HEIGHT> Map;
typedef MapTemplate<MAZE_WIDTH, GAME_HEIGHT> Maze;

const string Map::_images[] = { "  ", "■", "☆", "◎", "¤", "※" };
//const string Map::_images[] = { "  ", "〓", "❀", "◎", "¤", "※" };
#endif
