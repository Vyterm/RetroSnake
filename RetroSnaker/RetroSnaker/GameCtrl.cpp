#include "GameCtrl.hpp"

#include <math.h>
#include <iostream>
using namespace std;

inline bool IsKey(int vKey) { return (GetAsyncKeyState(vKey) & 0x0001) == 0x0001; }

void PlayerCtrl::Reverse()
{
	m_direction = GetReverseDirection(m_direction);
	m_snake.Reverse(m_map);
}

PlayerCtrl::PlayerCtrl(string name, Map &map, bool &isUpdateUI, Color color, int kUp, int kLeft, int kDown, int kRight)
	: m_name(name), m_map(map), m_isUpdateUI(isUpdateUI), m_snake(color), m_speedLevel(0), m_score(0),
	m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight), m_timer(TimerManager::get_instance().RegisterHandler<TickTock, PlayerCtrl>(*this, 100, true))
{
}

PlayerCtrl::~PlayerCtrl()
{
	TimerManager::get_instance().UnregiserHandler(m_timer);
}

void PlayerCtrl::ClearBuff()
{
	for (int i = 0; i < BuffCount; ++i)
		if (nullptr != m_buffs[i])
		{
			m_buffs[i]->Reset();
			m_buffs[i] = nullptr;
		}
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
	switch (m_map.GetItem(position).subType)
	{
	case E_SubType::SubType0:
		m_snake.ExtendHead(m_map, position);
		++m_speedLevel;
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
		m_snake.ExtendHead(m_map, position);
		m_enemy->Reverse();
		break;
	case E_SubType::SubType6:
		m_snake.TailToHead(m_map, position);
		TimerManager::get_instance().RegisterHandler<UnstoppableBuff>(*this, 15);
		break;
	case E_SubType::SubType7:
		m_snake.TailToHead(m_map, position);
		TimerManager::get_instance().RegisterHandler<IncontrollableBuff>(*m_enemy, 5);
		break;
	case E_SubType::SubType8:
		m_snake.ExtendHead(m_map, position);
		break;
	default:
		break;
	}
}

void PlayerCtrl::HandleTerrain(const Point& position)
{
	switch (m_map.GetItem(position).subType)
	{
	case E_SubType::SubType0:
		set_Alive(false);
		break;
	case E_SubType::SubType1:
		m_snake.TailToHead(m_map, position);
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
	auto type = m_map.GetItem(position).type;
	if (type == E_CellType::None || position == m_snake.get_tailPosition())
	{
		m_snake.TailToHead(m_map, position);
	}
	else if (type == E_CellType::Land)
	{
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

//PlayerCtrl::PlayerBuff::PlayerBuff(PlayerCtrl & player, int clockSecond) : m_player(player), TimerManager::handler(1000, true), m_clockSecond(clockSecond)
//{
//	if (nullptr != m_player.m_buffs[int(this->Type())])
//	{
//		m_player.m_buffs[int(this->Type())]->Copy(this);
//		StopTimer();
//	}
//}
//
//PlayerCtrl::PlayerBuff::~PlayerBuff()
//{
//	if (m_player.m_buffs[int(this->Type())] == this)
//		m_player.m_buffs[int(this->Type())] = nullptr;
//}

PlayerCtrl::PlayerBuff::PlayerBuff(PlayerCtrl & player, int clockSecond)
	: m_player(player), TimerManager::handler(200, true), m_clockSecond(clockSecond), m_tickCount(0), m_playerColor(player.get_Color().fore)
{
}

PlayerCtrl::PlayerBuff::~PlayerBuff()
{
	m_player.set_Color({ m_playerColor, DEFAULT_BACK_COLOR });
}

void PlayerCtrl::PlayerBuff::Invoke()
{
	++m_tickCount;
	if (m_tickCount % 3 == 0)
		m_player.set_Color({ Type() == E_BuffType::Unstoppable ? E_Color::LYellow : E_Color::Green, DEFAULT_BACK_COLOR });
	if (m_tickCount < 5) return;
	m_tickCount -= 5;
	m_player.set_Color({ m_playerColor, DEFAULT_BACK_COLOR });

	if (--m_clockSecond < 0)
		Reset();
	m_player.m_isUpdateUI = true;
}

void PlayerCtrl::PlayerBuff::Copy(const PlayerBuff * buff)
{
	m_clockSecond = buff->m_clockSecond;
	m_player.m_isUpdateUI = true;
}

int PlayerCtrl::PlayerBuff::RemainSecond() const
{
	return m_clockSecond;
}

void PlayerCtrl::PlayerBuff::Reset()
{
	StopTimer();
}

PlayerCtrl::UnstoppableBuff::UnstoppableBuff(PlayerCtrl & player, int clockSecond) : PlayerBuff(player, clockSecond)
{
	if (nullptr != m_player.m_buffs[int(this->Type())])
	{
		m_player.m_buffs[int(this->Type())]->Copy(this);
		StopTimer();
	}
	else
	{
		m_player.m_buffs[int(this->Type())] = this;
		m_player.m_unstoppable = true;
		m_player.m_speedLevel += 50;
	}
}

PlayerCtrl::UnstoppableBuff::~UnstoppableBuff()
{
	if (m_player.m_buffs[int(this->Type())] == this)
		m_player.m_buffs[int(this->Type())] = nullptr;
}

void PlayerCtrl::UnstoppableBuff::Reset()
{
	PlayerBuff::Reset();
	m_player.m_unstoppable = !m_player.m_unstoppable;
	m_player.m_speedLevel -= 50;
}

PlayerCtrl::IncontrollableBuff::IncontrollableBuff(PlayerCtrl & player, int clockSecond)
	: PlayerBuff(player, clockSecond), m_kUp(m_player.m_kUp), m_kLeft(m_player.m_kLeft), m_kDown(m_player.m_kDown), m_kRight(m_player.m_kRight)
{
	if (nullptr != m_player.m_buffs[int(this->Type())])
	{
		m_player.m_buffs[int(this->Type())]->Copy(this);
		StopTimer();
	}
	else
	{
		m_player.m_buffs[int(this->Type())] = this;
		//m_player.m_kUp = m_player.m_kLeft = m_player.m_kDown = m_player.m_kRight = 0;
		m_player.m_kUp = m_kDown;
		m_player.m_kLeft = m_kRight;
		m_player.m_kDown = m_kUp;
		m_player.m_kRight = m_kLeft;
	}
}

PlayerCtrl::IncontrollableBuff::~IncontrollableBuff()
{
	if (m_player.m_buffs[int(this->Type())] == this)
		m_player.m_buffs[int(this->Type())] = nullptr;
}

void PlayerCtrl::IncontrollableBuff::Reset()
{
	PlayerBuff::Reset();
	m_player.m_kUp = m_kUp;
	m_player.m_kLeft = m_kLeft;
	m_player.m_kDown = m_kDown;
	m_player.m_kRight = m_kRight;
}
