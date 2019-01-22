#include "GameBuff.hpp"
#include "GameCtrl.hpp"

PlayerBuff::PlayerBuff(PlayerCtrl & player, int clockSecond, E_BuffType type)
	: m_player(player), vyt::timer::handler(200, true), m_clockSecond(clockSecond), m_tickCount(0),
	m_type(type), m_isAppend(true), m_playerColor(player.get_Color())
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

PlayerBuff::~PlayerBuff()
{
	m_player.set_Color(m_playerColor);
	if (m_player.m_buffs[int(m_type)] == this)
		m_player.m_buffs[int(m_type)] = nullptr;
}

void PlayerBuff::Invoke()
{
	++m_tickCount;
	if (m_tickCount % 3 == 0)
		m_player.set_Color(ShiningColour());
	if (m_tickCount < 5) return;
	m_tickCount -= 5;
	m_player.set_Color(m_playerColor);

	if (--m_clockSecond < 0)
		RemoveBuff();
	m_player.m_isUpdateUI = true;
}

void PlayerBuff::Copy(const PlayerBuff * buff)
{
	m_clockSecond = buff->m_clockSecond;
	m_player.m_isUpdateUI = true;
}

int PlayerBuff::RemainSecond() const
{
	return m_clockSecond;
}

void PlayerBuff::RemoveBuff()
{
	StopTimer();
}

UnstoppableBuff::UnstoppableBuff(PlayerCtrl & player, int clockSecond) : PlayerBuff(player, clockSecond, E_BuffType::Unstoppable)
{
	if (m_isAppend)
	{
		m_player.set_Unstoppable(true);
		m_player.set_Speed(m_player.get_Speed() + 20);
	}
}

void UnstoppableBuff::RemoveBuff()
{
	PlayerBuff::RemoveBuff();
	m_player.set_Unstoppable(false);
	m_player.set_Speed(m_player.get_Speed() - 20);
}

IncontrollableBuff::IncontrollableBuff(PlayerCtrl & player, int clockSecond)
	: PlayerBuff(player, clockSecond, E_BuffType::Incontrollable)
{
	if (m_isAppend)
	{
		//m_player.m_kUp = m_player.m_kLeft = m_player.m_kDown = m_player.m_kRight = 0;
		m_player.get_keyCtrl(m_kUp, m_kLeft, m_kDown, m_kRight);
		m_player.set_keyCtrl(m_kDown, m_kRight, m_kUp, m_kLeft);
	}
}

void IncontrollableBuff::RemoveBuff()
{
	PlayerBuff::RemoveBuff();
	m_player.set_keyCtrl(m_kUp, m_kLeft, m_kDown, m_kRight);
}

SlippageBuff::SlippageBuff(PlayerCtrl & player, int clockSecond) : PlayerBuff(player, clockSecond, E_BuffType::Slippage)
{
	if (m_isAppend)
		m_player.set_Speed(m_player.get_Speed() + 80);
}

void SlippageBuff::RemoveBuff()
{
	PlayerBuff::RemoveBuff();
	m_player.set_Speed(m_player.get_Speed() - 80);
}