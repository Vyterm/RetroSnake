#include "GameCtrl.hpp"

#include <math.h>
#include <iostream>
using namespace std;

#pragma region Construct & Destruct
PlayerCtrl::PlayerCtrl(string name, GameMap &map, bool &isUpdateUI, int kUp, int kLeft, int kDown, int kRight)
	: m_name(name), m_map(map), m_isUpdateUI(isUpdateUI), m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight)
{

}

PlayerCtrl::~PlayerCtrl() {}

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

void PlayerCtrl::Reset(Vector2 position)
{
	m_timer = &(vyt::timer::get_instance().RegisterHandler<ticktock>(*this, 100, true));
}

SnakePlayerCtrl::SnakePlayerCtrl(string name, GameMap &map, bool &isUpdateUI, E_4BitColor color, int kUp, int kLeft, int kDown, int kRight)
	: PlayerCtrl(name, map, isUpdateUI, kUp, kLeft, kDown, kRight), m_snake(color), m_speedLevel(0), m_score(0)
{
}

SnakePlayerCtrl::~SnakePlayerCtrl()
{
}

void SnakePlayerCtrl::Clear()
{
	PlayerCtrl::Clear();
	m_alive = false;
}

void SnakePlayerCtrl::Reset(Vector2 position)
{
	PlayerCtrl::Reset(position);
	m_snake.Reset(m_map, position);
	m_alive = true;
	m_speedLevel = 0;
	m_direction = E_Direction::None;
}

#pragma endregion

#pragma region Properties

void PlayerCtrl::get_keyCtrl(int &kUp, int &kLeft, int &kDown, int &kRight)
{
	kUp = m_kUp;
	kLeft = m_kLeft;
	kDown = m_kDown;
	kRight = m_kRight;
}
void PlayerCtrl::set_keyCtrl(int kUp, int kLeft, int kDown, int kRight)
{
	m_kUp = kUp;
	m_kLeft = kLeft;
	m_kDown = kDown;
	m_kRight = kRight;
}

#pragma endregion

#pragma region Process Methods

E_Direction PlayerCtrl::UpdateDirection()
{
	E_Direction target;
	if (IsKey(m_kLeft))
		target = E_Direction::Left;
	else if (IsKey(m_kUp))
		target = E_Direction::Up;
	else if (IsKey(m_kRight))
		target = E_Direction::Right;
	else if (IsKey(m_kDown))
		target = E_Direction::Down;
	else
		target = E_Direction::None;
	return target;
}

void SnakePlayerCtrl::UpdateDirection()
{
	E_Direction target = PlayerCtrl::UpdateDirection();
	if (target == E_Direction::None)
		return;
	auto targetPosition = GetPositionByDirection(m_snake.get_headPosition(), target);
	if (m_map[targetPosition] == E_CellType::Body && targetPosition != m_snake.get_tailPosition())
		return;
	m_direction = target;
}

bool SnakePlayerCtrl::MoveByPosition()
{
	if (E_Direction::None == m_direction)
		return false;
	return MoveByPosition(GetPositionByDirection(m_snake.get_headPosition(), m_direction));
}

bool SnakePlayerCtrl::MoveByPosition(const Vector2 &position)
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
		else
			m_enemy->set_Alive(false);
	}
	else if (type == E_CellType::Jump)
		return MoveByPosition(GetPositionByDirection(m_map[position].jumpPoint, m_direction));
	else
		set_Alive(false);
	return false;
}

void SnakePlayerCtrl::HandleFood(const Vector2& position)
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
		Vector2 tail = m_enemy->m_snake.get_tailPosition();
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
		vyt::timer::get_instance().RegisterHandler<IncontrollableBuff>(*m_enemy, 3);
		break;
	case E_SubType::SubType8:
		m_snake.ExtendHead(m_map, position);
		break;
	default:
		break;
	}
}

void SnakePlayerCtrl::HandleTerrain(const Vector2& position)
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

void SnakePlayerCtrl::Reverse()
{
	m_direction = GetReverseDirection(m_direction);
	m_snake.Reverse(m_map);
}

void SnakePlayerCtrl::ToNextDeathAnimation()
{
}

#pragma endregion

void SnakePlayerCtrl::Process()
{
	m_timer->Reset(clock_t(SPEED_DELTA * pow(ACCELERATING_FACTOR, m_speedLevel)));
	if (!m_alive) return;
	UpdateDirection();
	if (!MoveByPosition()) return;
	//PlaySound(TEXT("sound_eat.wav"), NULL, SND_FILENAME | SND_ASYNC);
	m_map.GenerateRandomFood();
	m_isUpdateUI = true;
}
