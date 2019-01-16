#include "GameCtrl.hpp"

#include <math.h>
#include <iostream>
using namespace std;

inline bool IsKey(int vKey) { return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001; }

PlayerCtrl::PlayerCtrl(string name, Map &map, bool &isUpdateUI, Color color, int kUp, int kLeft, int kDown, int kRight)
	: m_name(name), m_map(map), m_isUpdateUI(isUpdateUI), m_snake(color), m_speedLevel(0), m_score(0),
	m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight), m_timer(TimerManager::get_instance().RegisterHandler<TickTock, PlayerCtrl>(100, true, *this))
{
}

PlayerCtrl::~PlayerCtrl()
{
	TimerManager::get_instance().UnregiserHandler(m_timer);
}

void PlayerCtrl::Reset(Point position)
{
	m_snake.Reset(m_map, position);
	m_alive = true;
	m_speedLevel = 0;
	m_direction = E_Direction::None;
}

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
	auto type = m_map.GetType(position);
	if (type == E_CellType::None || position == m_snake.get_tailPosition())
	{
		m_snake.TailToHead(m_map, position);
		HandleTerrain(position);
	}
	else if (type == E_CellType::Food)
	{
		HandleFood(position);
		return true;
	}
	else if (type == E_CellType::Body)
	{
		if (m_snake.Contains(position))
			set_Alive(false);
		m_enemy->set_Alive(false);
	}
	else if (type == E_CellType::Jump)
	{
		return MoveByPosition(GetPositionByDirection(m_map[position].jumpPoint, m_direction));
	}
	else
		set_Alive(false);
	return false;
}

void PlayerCtrl::Process()
{
	m_timer.Reset(clock_t(SPEED_DELTA * pow(ACCELERATING_FACTOR, m_speedLevel)));
	UpdateDirection();
	if (!MoveByPosition()) return;
	m_map.GenerateRandomFood();
	m_isUpdateUI = true;
}
