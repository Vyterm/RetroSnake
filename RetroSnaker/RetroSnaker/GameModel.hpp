#pragma once
#include "GameLib.hpp"
#include <memory>

enum class E_CellType : char
{
	None = ' ',
	Wall = '#',
	Food = 'F',
	Head = '@',
	Body = '0',
	Jump = 'J',
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

enum class E_Direction
{
	None = 0,
	Left = 1,
	Right = 2,
	Up = 4,
	Down = 8,
};

enum class E_MoveState
{
	Done,
	Eat,
	Kill,
	Over,
};

struct Point
{
public:
	int x, y;
	bool operator==(const Point &rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Point &rhs) const { return x != rhs.x || y != rhs.y; }
	void Set(const Point& point) { x = point.x; y = point.y; }
};

/* 0-黑色, 1-蓝色,   2-绿色,      3-浅绿色,     4-红色,   5-紫色,   6-黄色,   7-白色,
 * 8-灰色, 9-淡蓝色, 10-淡绿色,   11-淡浅绿色   12-淡红色 13-淡紫色 14-淡黄色 15-亮白色*/
struct Color
{
	int fore, back;
};

template <int Width, int Height>
class MapTemplate
{
	struct MapItem
	{
		E_CellType type = E_CellType::None;
		E_SubType subType = E_SubType::SubType0;
		Color color = Map::DefaultColor;
		Point jumpPoint;
		MapItem() { }
		void SetQuote(const std::shared_ptr<void> &quote)
		{
			m_weakQuote = quote;
		}
		bool TryGetQuote(std::shared_ptr<void> &quote)
		{
			quote = m_weakQuote.lock();
			return m_weakQuote.expired();
		}
		void Set(E_CellType type, Color color)
		{
			Set(type, E_SubType::SubType0, color);
		}
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
	private:
		std::weak_ptr<void> m_weakQuote;
	};
	MapItem m_items[Width][Height];
public:
	static const Color DefaultColor;
	MapTemplate() { }
	const MapItem& operator[](Point position) const { return m_items[position.x][position.y]; }
	MapItem& operator[](Point position) { return m_items[position.x][position.y]; }

	const E_CellType& Index(Point position) const { return m_items[position.x][position.y].type; }
	E_CellType& Index(Point position) { return m_items[position.x][position.y].type; }
	const E_CellType& Index(int x, int y) const { return m_items[x][y].type; }
	E_CellType& Index(int x, int y) { return m_items[x][y].type; }

	const Color& ColorIndex(Point position) const { return m_items[position.x][position.y].color; }
	Color& ColorIndex(Point position) { return m_items[position.x][position.y].color; }
	const Color &ColorIndex(int x, int y) const { return m_items[x][y].color; }
	/* 0-黑色, 1-蓝色,   2-绿色,      3-浅绿色,     4-红色,   5-紫色,   6-黄色,   7-白色,
	 * 8-灰色, 9-淡蓝色, 10-淡绿色,   11-淡浅绿色   12-淡红色 13-淡紫色 14-淡黄色 15-亮白色*/
	Color &ColorIndex(int x, int y) { return m_items[x][y].color; }
};
typedef MapTemplate<GAME_WIDTH + MAZE_WIDTH, GAME_HEIGHT + MAZE_HEIGHT> Map;

class Snake
{
	class SnakePart
	{
		Point m_position;
		SnakePart *m_last, *m_next;
	public:
		SnakePart(Point position) : m_position(position), m_last(nullptr), m_next(nullptr) { }
		friend class Snake;
	};
	SnakePart *m_head, *m_tail;
	Color m_color;
public:
	Snake(Map &map, Point position, Color color);
	Snake(Snake &&snake);
	~Snake();
	Point get_headPosition() const { return m_head->m_position; }
	Point get_tailPosition() const { return m_tail->m_position; }
	Color get_color() const { return m_color; }
	void TailToHead(Map &map, Point position);
	void ExtendHead(Map &map, Point position);
	void ExtendTail(Map &map, Point position);
	void RemoveTail(Map &map);
	bool Contains(Point position);
};

inline Point GetPositionByDirection(Point startPos, E_Direction direction)
{
	int shift = int(direction);
	startPos.x -= (shift >> 0) & 1;
	startPos.x += (shift >> 1) & 1;
	startPos.y -= (shift >> 2) & 1;
	startPos.y += (shift >> 3) & 1;
	return startPos;
}