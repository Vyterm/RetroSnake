#include "GameCtrl.hpp"

#include <math.h>

inline bool IsKey(int vKey) { return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001; }

//»ÆÂÌ×ÏºìÀ¶
void PlayerCtrl::HandleFood(const Point& position)
{
	++m_speedLevel;
	switch (m_map[position].subType)
	{
	case E_SubType::SubType0:
		m_snake.ExtendHead(m_map, position);
		break;
	case E_SubType::SubType1:
		m_snake.TailToHead(m_map, position);
		Point tail = m_enemy->m_snake.get_tailPosition();
		if (!m_enemy->MoveByPosition())
			m_enemy->m_snake.ExtendTail(m_map, tail);
		break;
	case E_SubType::SubType2:
		m_snake.TailToHead(m_map, position);
		m_snake.RemoveTail(m_map);
		break;
	case E_SubType::SubType3:
		m_snake.ExtendHead(m_map, position);
		m_speedLevel += 25;
		break;
	case E_SubType::SubType4:
		m_snake.ExtendHead(m_map, position);
		m_enemy->m_speedLevel -= 15;
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
	if (IsKey(m_kLeft))
		target = E_Direction::Left;
	else if (IsKey(m_kUp))
		target = E_Direction::Up;
	else if (IsKey(m_kRight))
		target = E_Direction::Right;
	else if (IsKey(m_kDown))
		target = E_Direction::Down;
	
	auto targetPosition = GetPositionByDirection(m_snake.get_headPosition(), target);
	if (m_map[targetPosition] == E_CellType::Body && targetPosition != m_snake.get_tailPosition())
		return;
	m_direction = target;
}

bool PlayerCtrl::MoveByPosition()
{
	if (E_Direction::None == m_direction)
		return false;
	return MoveByPosition(GetPositionByDirection(m_snake.get_headPosition(), m_direction));
}

bool PlayerCtrl::MoveByPosition(const Point &position)
{
	if (m_map[position] == E_CellType::None || position == m_snake.get_tailPosition())
	{
		m_snake.TailToHead(m_map, position);
		HandleTerrain(position);
	}
	else if (m_map[position] == E_CellType::Food)
	{
		HandleFood(position);
		return true;
	}
	else if (m_map[position] == E_CellType::Body)
	{
		if (m_snake.Contains(position))
			m_alive = false;
		m_enemy->m_alive = false;
	}
	else if (m_map[position] == E_CellType::Jump)
	{
		return MoveByPosition(GetPositionByDirection(m_map[position].jumpPoint, m_direction));
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
		return MoveByPosition();
	}
	return false;
}
