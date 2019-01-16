#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "GameLib.hpp"

#include <iostream>
#include <string>
#include <memory>

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

/*
亮	红	绿	蓝
1	1	1	1
0x0~0xF表示暗黑色至亮白色
*/
struct Color
{
	int fore, back;
	bool operator==(const Color &rhs) const { return fore == rhs.fore && back == rhs.back; }
	bool operator!=(const Color &rhs) const { return fore != rhs.fore || back != rhs.back; }
	void Set(const Color& color) { fore = color.fore; back = color.back; }
};

constexpr Color DEFAULT_COLOR = { 0B0111, 0B0000 };

class MapItem
{
public:
	E_CellType type = E_CellType::None;
	E_SubType subType = E_SubType::SubType0;
	Color color = DEFAULT_COLOR;
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
class MapTemplate
{
	Point m_position;
	static const string _images[];
	MapItem m_items[Width][Height];
	MapItem m_zCacheItems[Width][Height];

	void DrawCell(int x, int y, bool isForce)
	{
		if (!isForce && m_zCacheItems[x][y] == m_items[x][y])
			return;
		m_zCacheItems[x][y] = m_items[x][y];
		SetPosition(m_position.x + x, m_position.y + y);
		auto color = m_zCacheItems[x][y].color;
		SetColor(color.fore, color.back);
		cout << ToString(m_zCacheItems[x][y]);
	}
public:
	MapTemplate()
	{
		m_position = { 0, 0 };
	}
	const MapItem& operator[](Point position) const { return m_items[position.x][position.y]; }
	MapItem& operator[](Point position) { return m_items[position.x][position.y]; }
	const MapItem& Index(int x, int y) const { return m_items[x][y]; }
	MapItem& Index(int x, int y) { return m_items[x][y]; }

	static const string& ToString(const MapItem &item) { return _images[int(item.type)]; }

	void Init(int startPosX, int width, int startPosY, int height)
	{
		int endPosX = startPosX + width - 1, endPosY = startPosY + height - 1;
		for (int ri = startPosY; ri <= endPosY; ++ri)
			for (int ci = startPosX; ci <= endPosX; ++ci)
				m_items[ci][ri] = (ci == startPosX || ci == endPosX || ri == startPosY || ri == endPosY) ? E_CellType::Wall : E_CellType::None;
	}

	void Draw(bool isForce = false)
	{
		for (int ri = 0; ri < Height; ++ri)
			for (int ci = 0; ci < Width; ++ci)
				DrawCell(ci, ri, isForce);
	}
};
typedef MapTemplate<GAME_WIDTH + MAZE_WIDTH, GAME_HEIGHT> Map;
typedef MapTemplate<MAZE_WIDTH, GAME_HEIGHT> Maze;

const string Map::_images[] = { "  ", "〓", "食", "◎", "¤", "※" };
//const string Map::_images[] = { "  ", "〓", "❀", "◎", "¤", "※" };
#endif
