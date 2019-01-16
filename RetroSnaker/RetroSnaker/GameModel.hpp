#pragma once
#include "GameLib.hpp"

enum class E_CellType
{
	None = 0,
	Wall = 1,
	Food = 2,
	Head = 3,
	Body = 4,
};
struct Point
{
public:
	int x, y;
	bool operator==(const Point &rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Point &rhs) const { return x != rhs.x || y != rhs.y; }
};
struct Color
{
	/* 0-黑色, 1-蓝色,   2-绿色,      3-浅绿色,     4-红色,   5-紫色,   6-黄色,   7-白色,
	 * 8-灰色, 9-淡蓝色, 10-淡绿色,   11-淡浅绿色   12-淡红色 13-淡紫色 14-淡黄色 15-亮白色*/
	int fore, back;
};

template <int Width, int Height>
class MapTemplate
{
	struct MapItem
	{
		E_CellType type;
		Color color = Map::DefaultColor;
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
typedef MapTemplate<GAME_WIDTH, GAME_HEIGHT> Map;

enum class E_Direction
{
	None  =  0,
	Left  =  1,
	Right = -1,
	Up    =  2,
	Down  = -2,
};

enum class E_MoveState
{
	Done,
	Eat,
	Kill,
	Over,
};

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
	E_Direction m_direction;
	Color m_color;
public:
	Snake(Map &map, Point position, Color color) : m_head(new SnakePart(position)), m_tail(m_head), m_color(color)
	{
		map.Index(position) = E_CellType::Head;
		map.ColorIndex(position) = color;
	}
	Snake(Snake &&snake)
	{
		if (this == &snake) return;
		this->m_head = snake.m_head;
		snake.m_head = nullptr;
		this->m_tail = snake.m_tail;
		snake.m_tail = nullptr;
		this->m_direction = snake.m_direction;
		this->m_color = snake.m_color;
	}
	~Snake()
	{
		while (nullptr != m_head)
		{
			auto tmp = m_head;
			m_head = m_head->m_last;
			delete tmp;
		}
		m_tail = nullptr;
	}
	Point get_headPosition() const { return m_head->m_position; }
	Point get_tailPosition() const { return m_tail->m_position; }
	E_Direction get_direction() const { return m_direction; }
	void set_direction(E_Direction direction) { m_direction = direction; }
	Color get_color() const { return m_color; }
	E_MoveState MoveByDirection(Map &m_map);
};

inline Point GetPositionByDirection(Point startPos, E_Direction direction)
{
	switch (direction)
	{
	case E_Direction::Left:
		startPos.x -= 1;
		break;
	case E_Direction::Right:
		startPos.x += 1;
		break;
	case E_Direction::Up:
		startPos.y -= 1;
		break;
	case E_Direction::Down:
		startPos.y += 1;
		break;
	case E_Direction::None:
	default:
		break;
	}
	return startPos;
}