#include "GameModel.hpp"

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

E_MoveState Snake::MoveByDirection(Map &m_map)
{
	if (E_Direction::None == m_direction)
		return E_MoveState::Done;
	auto tmpPoint = GetPositionByDirection(m_head->m_position, m_direction);
	if (m_map[tmpPoint] == E_MapItem::None)
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

		m_map[tail->m_position] = E_MapItem::None;
		m_map[m_head->m_position] = E_MapItem::Body;
		tail->m_position = tmpPoint;
		m_map[tail->m_position] = E_MapItem::Head;
		m_head = tail;
		return E_MoveState::Done;
	}
	else if (m_map[tmpPoint] == E_MapItem::Food)
	{
		m_head->m_next = new SnakePart(tmpPoint);
		m_head->m_next->m_last = m_head;
		m_map[m_head->m_position] = E_MapItem::Body;
		m_head = m_head->m_next;
		m_map[m_head->m_position] = E_MapItem::Head;
		return E_MoveState::Eat;
	}
	else
		return E_MoveState::Over;
}
