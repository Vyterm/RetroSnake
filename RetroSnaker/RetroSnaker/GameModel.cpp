#include "GameModel.hpp"

const Color Map::DefaultColor = { 7,0 };

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

E_MoveState Snake::MoveByDirection(Map &map)
{
	if (E_Direction::None == m_direction)
		return E_MoveState::Done;
	auto tmpPoint = GetPositionByDirection(m_head->m_position, m_direction);
	if (map[tmpPoint] == E_MapItem::None)
	{
		auto tail = m_head;
		while (nullptr != tail->m_last)
			tail = tail->m_last;
		if (nullptr != tail->m_next)
		{
			tail->m_next->m_last = nullptr;
			tail->m_next = nullptr;
		}
		if (tail == m_head)
		{
			m_head->m_next = new SnakePart(tmpPoint);
			tail = m_head->m_next;
		}
		tail->m_last = m_head;
		m_head->m_next = tail;

		map[tail->m_position] = E_MapItem::None;
		map.ColorIndex(tail->m_position) = Map::DefaultColor;
		map[m_head->m_position] = E_MapItem::Body;
		map.ColorIndex(m_head->m_position) = m_color;
		tail->m_position = tmpPoint;
		map[tail->m_position] = E_MapItem::Head;
		map.ColorIndex(tail->m_position) = m_color;
		m_head = tail;
		return E_MoveState::Done;
	}
	else if (map[tmpPoint] == E_MapItem::Food)
	{
		m_head->m_next = new SnakePart(tmpPoint);
		m_head->m_next->m_last = m_head;
		map[m_head->m_position] = E_MapItem::Body;
		map.ColorIndex(m_head->m_position) = m_color;
		m_head = m_head->m_next;
		map[m_head->m_position] = E_MapItem::Head;
		map.ColorIndex(m_head->m_position) = m_color;
		return E_MoveState::Eat;
	}
	else
		return E_MoveState::Over;
}
