#include "GameCtrl.hpp"

#include <math.h>

inline bool IsKeyDown(int vKey) { return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001; }

void PlayerCtrl::UpdateDirection()
{
	E_Direction target = m_snake.get_direction();
	if (IsKeyDown(m_kLeft))
		target = E_Direction::Left;
	else if (IsKeyDown(m_kUp))
		target = E_Direction::Up;
	else if (IsKeyDown(m_kRight))
		target = E_Direction::Right;
	else if (IsKeyDown(m_kDown))
		target = E_Direction::Down;
	
	//if ((int(target) + int(m_snake.get_direction())) == 0)
	//	return;
	auto targetPosition = GetPositionByDirection(m_snake.get_headPosition(), target);
	if (m_map.Index(targetPosition) == E_CellType::Body && targetPosition != m_snake.get_tailPosition())
		return;
	m_snake.set_direction(target);
}

E_MoveState PlayerCtrl::Process(int timeDelta)
{
	m_moveRemain -= timeDelta;
	if (m_moveRemain <= 0)
	{
		m_moveRemain = int(SPEED_DELTA * pow(ACCELERATING_FACTOR, m_speedLevel));
		UpdateDirection();
		return m_snake.MoveByDirection(m_map);
	}
	return E_MoveState::Done;
}
