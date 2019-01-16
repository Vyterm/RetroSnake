#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include "GameLib.hpp"
#include "GameMap.hpp"

enum class E_Direction
{
	None = 0,
	Left = 1,
	Right = 2,
	Up = 4,
	Down = 8,
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
	Color m_color, m_twinkleColor;
public:
	Snake(Color color);
	Snake(Snake &&snake);
	~Snake();

	void Reset(Map &map, Point position);
	void Clear();

	Point get_headPosition() const { return m_head->m_position; }
	Point get_tailPosition() const { return m_tail->m_position; }
	Color get_color() const { return m_color; }
	void Twinkle(Map &map, const Color &color);
	void Reverse(Map &map);
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

inline E_Direction GetReverseDirection(E_Direction direction)
{
	switch (direction)
	{
	case E_Direction::Left: return E_Direction::Right;
	case E_Direction::Right: return E_Direction::Left;
	case E_Direction::Up: return E_Direction::Down;
	case E_Direction::Down: return E_Direction::Up;
	case E_Direction::None:
	default:
		return E_Direction::None;
	}
}

#endif