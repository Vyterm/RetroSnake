#include "GameCtrl.hpp"

#include <math.h>
#include <iostream>
using namespace std;

#pragma region Construct & Destruct

PlayerCtrl::PlayerCtrl(string name, GameMap &map, bool &isUpdateUI, Color color, int kUp, int kLeft, int kDown, int kRight)
	: m_name(name), m_map(map), m_isUpdateUI(isUpdateUI), m_snake(color), m_speedLevel(0), m_score(0),
	m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight)
{
}

PlayerCtrl::~PlayerCtrl()
{
}

void PlayerCtrl::Clear()
{
	vyt::timer::get_instance().UnregiserHandler(*m_timer);
	for (int i = 0; i < BuffCount; ++i)
		if (nullptr != m_buffs[i])
		{
			m_buffs[i]->RemoveBuff();
			m_buffs[i] = nullptr;
		}
}

void PlayerCtrl::Reset(Point position)
{
	m_timer = &(vyt::timer::get_instance().RegisterHandler<TickTock, PlayerCtrl>(*this, 100, true));
	m_snake.Reset(m_map, position);
	m_alive = true;
	m_speedLevel = 0;
	m_direction = E_Direction::None;
}

#pragma endregion

#pragma region Process Methods

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
		m_snake.TailToHead(m_map, position);
	else if (type == E_CellType::Land)
		HandleTerrain(position);
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
		return MoveByPosition(GetPositionByDirection(m_map[position].jumpPoint, m_direction));
	else
		set_Alive(false);
	return false;
}

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
		m_speedLevel += 15;
		break;
	case E_SubType::SubType4:
		m_snake.ExtendHead(m_map, position);
		m_enemy->m_speedLevel -= 10;
		break;
	case E_SubType::SubType5:
		m_snake.ExtendHead(m_map, position);
		m_enemy->Reverse();
		break;
	case E_SubType::SubType6:
		m_snake.TailToHead(m_map, position);
		vyt::timer::get_instance().RegisterHandler<UnstoppableBuff>(*this, 15);
		break;
	case E_SubType::SubType7:
		m_snake.TailToHead(m_map, position);
		vyt::timer::get_instance().RegisterHandler<IncontrollableBuff>(*m_enemy, 5);
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
	auto item = m_map.GetItem(position);
	switch (item.subType)
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
		//if (!m_snake.RemoveTail(m_map))
		//	m_alive = false;
		//else
		//	m_snake.TailToHead(m_map, position);
		if (!m_snake.RemoveTail(m_map))
			m_alive = false;
		break;
	case E_SubType::SubType4:
		m_snake.TailToHead(m_map, position);
		vyt::timer::get_instance().RegisterHandler<SlippageBuff>(*this, 2);
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

void PlayerCtrl::Reverse()
{
	m_direction = GetReverseDirection(m_direction);
	m_snake.Reverse(m_map);
}

void PlayerCtrl::ToNextDeathAnimation()
{
}

#pragma endregion

#pragma region Player Buffs

PlayerCtrl::PlayerBuff::PlayerBuff(PlayerCtrl & player, int clockSecond, E_BuffType type)
	: m_player(player), vyt::timer::handler(200, true), m_clockSecond(clockSecond), m_tickCount(0),
	m_type(type), m_isAppend(true), m_playerColor(player.get_Color().fore)
{
	if (nullptr != m_player.m_buffs[int(m_type)])
	{
		m_player.m_buffs[int(m_type)]->Copy(this);
		StopTimer();
		m_isAppend = false;
	}
	else
		m_player.m_buffs[int(m_type)] = this;
}

PlayerCtrl::PlayerBuff::~PlayerBuff()
{
	m_player.set_Color({ m_playerColor, DEFAULT_BACK_COLOR });
	if (m_player.m_buffs[int(m_type)] == this)
		m_player.m_buffs[int(m_type)] = nullptr;
}

void PlayerCtrl::PlayerBuff::Invoke()
{
	++m_tickCount;
	if (m_tickCount % 3 == 0)
		m_player.set_Color({ ShiningColour(), DEFAULT_BACK_COLOR });
	if (m_tickCount < 5) return;
	m_tickCount -= 5;
	m_player.set_Color({ m_playerColor, DEFAULT_BACK_COLOR });

	if (--m_clockSecond < 0)
		RemoveBuff();
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

void PlayerCtrl::PlayerBuff::RemoveBuff()
{
	StopTimer();
}

PlayerCtrl::UnstoppableBuff::UnstoppableBuff(PlayerCtrl & player, int clockSecond) : PlayerBuff(player, clockSecond, E_BuffType::Unstoppable)
{
	if (m_isAppend)
	{
		m_player.m_unstoppable = true;
		m_player.m_speedLevel += 20;
	}
}

void PlayerCtrl::UnstoppableBuff::RemoveBuff()
{
	PlayerBuff::RemoveBuff();
	m_player.m_unstoppable = !m_player.m_unstoppable;
	m_player.m_speedLevel -= 20;
}

PlayerCtrl::IncontrollableBuff::IncontrollableBuff(PlayerCtrl & player, int clockSecond)
	: PlayerBuff(player, clockSecond, E_BuffType::Incontrollable),
	m_kUp(m_player.m_kUp), m_kLeft(m_player.m_kLeft), m_kDown(m_player.m_kDown), m_kRight(m_player.m_kRight)
{
	if (m_isAppend)
	{
		//m_player.m_kUp = m_player.m_kLeft = m_player.m_kDown = m_player.m_kRight = 0;
		m_player.m_kUp = m_kDown;
		m_player.m_kLeft = m_kRight;
		m_player.m_kDown = m_kUp;
		m_player.m_kRight = m_kLeft;
	}
}

void PlayerCtrl::IncontrollableBuff::RemoveBuff()
{
	PlayerBuff::RemoveBuff();
	m_player.m_kUp = m_kUp;
	m_player.m_kLeft = m_kLeft;
	m_player.m_kDown = m_kDown;
	m_player.m_kRight = m_kRight;
}

PlayerCtrl::SlippageBuff::SlippageBuff(PlayerCtrl & player, int clockSecond) : PlayerBuff(player, clockSecond, E_BuffType::Slippage)
{
	if (m_isAppend)
		m_player.m_speedLevel += 80;
}

void PlayerCtrl::SlippageBuff::RemoveBuff()
{
	PlayerBuff::RemoveBuff();
	m_player.m_speedLevel -= 80;
}

#pragma endregion

void PlayerCtrl::Process()
{
	m_timer->Reset(clock_t(SPEED_DELTA * pow(ACCELERATING_FACTOR, m_speedLevel)));
	if (!m_alive) return;
	UpdateDirection();
	if (!MoveByPosition()) return;
	m_map.GenerateRandomFood();
	m_isUpdateUI = true;
}
