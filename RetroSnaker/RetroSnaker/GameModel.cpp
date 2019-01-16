#include "GameModel.hpp"

const Color Map::DefaultColor = { 7,0 };

E_MoveState Snake::MoveByDirection(Map &map)
{
	if (E_Direction::None == m_direction)
		return E_MoveState::Done;
	auto tmpPoint = GetPositionByDirection(m_head->m_position, m_direction);
	if (map.Index(tmpPoint) == E_CellType::None || tmpPoint == get_tailPosition())
	{
		SnakePart *newTail;
		if (nullptr != m_tail->m_next)
		{
			newTail = m_tail->m_next;
			m_tail->m_next->m_last = nullptr;
			m_tail->m_next = nullptr;
		}
		else if (m_tail == m_head)
		{
			newTail = m_head;
			m_head->m_next = new SnakePart(tmpPoint);
			m_tail = m_head->m_next;
		}
		else
			throw "Bad tail";
		m_tail->m_last = m_head;
		m_head->m_next = m_tail;

		map.Index(m_tail->m_position) = E_CellType::None;
		map.ColorIndex(m_tail->m_position) = Map::DefaultColor;
		map.Index(m_head->m_position) = E_CellType::Body;
		map.ColorIndex(m_head->m_position) = m_color;
		m_tail->m_position = tmpPoint;
		map.Index(m_tail->m_position) = E_CellType::Head;
		map.ColorIndex(m_tail->m_position) = m_color;
		m_head = m_tail;
		m_tail = newTail;
		return E_MoveState::Done;
	}
	else if (map.Index(tmpPoint) == E_CellType::Food)
	{
		m_head->m_next = new SnakePart(tmpPoint);
		m_head->m_next->m_last = m_head;
		map.Index(m_head->m_position) = E_CellType::Body;
		map.ColorIndex(m_head->m_position) = m_color;
		m_head = m_head->m_next;
		map.Index(m_head->m_position) = E_CellType::Head;
		map.ColorIndex(m_head->m_position) = m_color;
		return E_MoveState::Eat;
	}
	else if (map.Index(tmpPoint) == E_CellType::Body)
	{
		auto tmpBody = m_head;
		while (nullptr != tmpBody)
		{
			if (tmpBody->m_position == tmpPoint)
				return E_MoveState::Over;
			tmpBody = tmpBody->m_last;
		}
		return E_MoveState::Kill;
	}
	else
		return E_MoveState::Over;
}
