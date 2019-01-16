#pragma once
#include "GameLib.hpp"

enum class E_MapItem
{
	None = 0,
	Wall = 1,
	Food = 2,
	Head = 3,
	Body = 4,
};
struct Point
{
	int x, y;
};

template <int Width, int Height>
class MapTemplate
{
	E_MapItem m_items[Width][Height];
public:
	//const E_MapItem * operator[](int x) const { return m_items[x]; }
	//E_MapItem * operator[](int x) { return m_items[x]; }
	const E_MapItem& operator[](Point position) const { return m_items[position.x][position.y]; }
	E_MapItem& operator[](Point position) { return m_items[position.x][position.y]; }
	const E_MapItem& Index(int x, int y) const { return m_items[x][y]; }
	E_MapItem& Index(int x, int y) { return m_items[x][y]; }
};
typedef MapTemplate<GAME_WIDTH, GAME_HEIGHT> Map;

enum class E_Direction
{
	None,
	Left,
	Right,
	Up,
	Down,
};

enum class E_MoveState
{
	Done,
	Eat,
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
	SnakePart *m_head;
	E_Direction m_direction;
public:
	Snake(Point position) : m_head(new SnakePart(position)) { }
	~Snake()
	{
		while (nullptr != m_head)
		{
			auto tmp = m_head;
			m_head = m_head->m_last;
			delete tmp;
		}
	}
	E_Direction get_direction() const { return m_direction; }
	void set_direction(E_Direction direction) { m_direction = direction; }
	E_MoveState MoveByDirection(Map &m_map);
};