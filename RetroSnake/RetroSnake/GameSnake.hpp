#ifndef GAME_MODEL_HPP
#define GAME_MODEL_HPP

#include "winapi.hpp"
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
		Vector2 m_position;
		SnakePart *m_last, *m_next;
	public:
		SnakePart(Vector2 position) : m_position(position), m_last(nullptr), m_next(nullptr) { }
		friend class Snake;
	};
	SnakePart *m_head, *m_tail;
	E_4BitColor m_color, m_twinkleColor;
public:
	Snake(E_4BitColor color);
	Snake(Snake &&snake);
	~Snake();

	void Reset(GameMap &map, Vector2 position);
	void Clear();

	Vector2 get_headPosition() const { return m_head->m_position; }
	Vector2 get_tailPosition() const { return m_tail->m_position; }
	E_4BitColor get_color() const { return m_color; }
	void Twinkle(GameMap &map, const E_4BitColor &color);
	void Reverse(GameMap &map);
	void TailToHead(GameMap &map, Vector2 position);
	void ExtendHead(GameMap &map, Vector2 position);
	void ExtendTail(GameMap &map, Vector2 position);
	bool RemoveHead(GameMap &map);
	bool RemoveTail(GameMap &map);
	bool Contains(Vector2 position);
};

inline Vector2 GetPositionByDirection(Vector2 startPos, E_Direction direction)
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