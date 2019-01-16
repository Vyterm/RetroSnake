#include "GameCtrl.hpp"

#include <math.h>

inline bool IsKeyDown(int vKey) { return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001; }

//»ÆÂÌ×ÏºìÀ¶
void PlayerCtrl::HandleFood(const Point& position)
{
	switch (m_map[position].subType)
	{
	case E_SubType::SubType0:
		m_snake.ExtendHead(m_map, position);
		++m_speedLevel;
		break;
	case E_SubType::SubType1:
		m_snake.TailToHead(m_map, position);
		Point tail = m_enemy->m_snake.get_tailPosition();
		if (!m_enemy->MoveByDirection())
			m_enemy->m_snake.ExtendTail(m_map, tail);
		break;
	case E_SubType::SubType2:
		m_snake.TailToHead(m_map, position);
		m_snake.RemoveTail(m_map);
		break;
	case E_SubType::SubType3:
		m_snake.ExtendHead(m_map, position);
		m_speedLevel += 20;
		break;
	case E_SubType::SubType4:
		m_snake.ExtendHead(m_map, position);
		m_enemy->m_speedLevel -= 10;
		break;
	case E_SubType::SubType5:
		break;
	case E_SubType::SubType6:
		break;
	case E_SubType::SubType7:
		break;
	case E_SubType::SubType8:
		break;
	default:
		break;
	}
}

void PlayerCtrl::HandleTerrain(const Point& position)
{
	switch (m_map[position].subType)
	{
	case E_SubType::SubType0:
		break;
	case E_SubType::SubType1:
		break;
	case E_SubType::SubType2:
		break;
	case E_SubType::SubType3:
		break;
	case E_SubType::SubType4:
		break;
	case E_SubType::SubType5:
		break;
	case E_SubType::SubType6:
		break;
	case E_SubType::SubType7:
		break;
	case E_SubType::SubType8:
		break;
	default:
		break;
	}
}

void PlayerCtrl::UpdateDirection()
{
	E_Direction target = m_direction;
	if (IsKeyDown(m_kLeft))
		target = E_Direction::Left;
	else if (IsKeyDown(m_kUp))
		target = E_Direction::Up;
	else if (IsKeyDown(m_kRight))
		target = E_Direction::Right;
	else if (IsKeyDown(m_kDown))
		target = E_Direction::Down;
	
	auto targetPosition = GetPositionByDirection(m_snake.get_headPosition(), target);
	if (m_map.Index(targetPosition) == E_CellType::Body && targetPosition != m_snake.get_tailPosition())
		return;
	m_direction = target;
}

bool PlayerCtrl::MoveByDirection()
{
	if (E_Direction::None == m_direction)
		return false;
	auto tmpPoint = GetPositionByDirection(m_snake.get_headPosition(), m_direction);
	if (m_map.Index(tmpPoint) == E_CellType::None || tmpPoint == m_snake.get_tailPosition())
	{
		m_snake.TailToHead(m_map, tmpPoint);
		HandleTerrain(tmpPoint);
	}
	else if (m_map.Index(tmpPoint) == E_CellType::Food)
	{
		HandleFood(tmpPoint);
		return true;
	}
	else if (m_map.Index(tmpPoint) == E_CellType::Body)
	{
		if (m_snake.Contains(tmpPoint))
			m_alive = false;
		m_enemy->m_alive = false;
	}
	else if (m_map.Index(tmpPoint) == E_CellType::Jump)
	{
		m_snake.TailToHead(m_map, m_map[tmpPoint].jumpPoint);
	}
	else
		m_alive = false;
	return false;
}

bool PlayerCtrl::Process(int timeDelta)
{
	m_moveRemain -= timeDelta;
	if (m_moveRemain <= 0)
	{
		m_moveRemain += int(SPEED_DELTA * pow(ACCELERATING_FACTOR, m_speedLevel));
		UpdateDirection();
		return MoveByDirection();
	}
	return false;
}
