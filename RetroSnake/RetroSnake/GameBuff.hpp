#ifndef GAME_BUFF_HPP_INCLUDED
#define GAME_BUFF_HPP_INCLUDED

#include "GameColor.hpp"
#include "vytTimer.hpp"

class PlayerCtrl;

enum class E_BuffType
{
	Unstoppable,
	Incontrollable,
	Slippage,
};
static constexpr auto BuffCount = 3;


class PlayerBuff : public vyt::timer::handler
{
	int m_clockSecond;
	int m_tickCount;
	E_4BitColor m_playerColor;
	E_BuffType m_type;
protected:
	PlayerCtrl &m_player;
	bool m_isAppend;
	PlayerBuff(PlayerCtrl &player, int clockSecond, E_BuffType type);
	virtual ~PlayerBuff();
	virtual E_4BitColor ShiningColour() const { return m_playerColor; }
	virtual void Invoke();
public:
	virtual void Copy(const PlayerBuff *buff);
	int RemainSecond() const;
	virtual void RemoveBuff();
};


class UnstoppableBuff : public PlayerBuff
{
public:
	UnstoppableBuff(PlayerCtrl &player, int clockSecond);
	E_4BitColor ShiningColour() const { return E_4BitColor::LYellow; }
	virtual void RemoveBuff() override;
};
class IncontrollableBuff : public PlayerBuff
{
	int m_kUp, m_kLeft, m_kDown, m_kRight;
public:
	IncontrollableBuff(PlayerCtrl &player, int clockSecond);
	E_4BitColor ShiningColour() const { return E_4BitColor::Green; }
	virtual void RemoveBuff() override;
};
class SlippageBuff : public PlayerBuff
{
public:
	SlippageBuff(PlayerCtrl &player, int clockSecond);
	virtual void RemoveBuff() override;
};
//#pragma region Player Buffs

//class PlayerBuff : public vyt::timer::handler
//{
//	int m_clockSecond;
//	int m_tickCount;
//	E_4BitColor m_playerColor;
//	E_BuffType m_type;
//protected:
//	SnakePlayerCtrl &m_player;
//	bool m_isAppend;
//	PlayerBuff(SnakePlayerCtrl &player, int clockSecond, E_BuffType type);
//	virtual ~PlayerBuff();
//	virtual E_4BitColor ShiningColour() const { return m_playerColor; }
//	virtual void Invoke();
//public:
//	virtual void Copy(const PlayerBuff *buff);
//	int RemainSecond() const;
//	virtual void RemoveBuff();
//};
//PlayerBuff *m_buffs[BuffCount];
//class UnstoppableBuff : public PlayerBuff
//{
//public:
//	UnstoppableBuff(SnakePlayerCtrl &player, int clockSecond);
//	E_4BitColor ShiningColour() const { return E_4BitColor::LYellow; }
//	virtual void RemoveBuff() override;
//};
//class IncontrollableBuff : public PlayerBuff
//{
//	int m_kUp, m_kLeft, m_kDown, m_kRight;
//public:
//	IncontrollableBuff(SnakePlayerCtrl &player, int clockSecond);
//	E_4BitColor ShiningColour() const { return E_4BitColor::Green; }
//	virtual void RemoveBuff() override;
//};
//class SlippageBuff : public PlayerBuff
//{
//public:
//	SlippageBuff(SnakePlayerCtrl &player, int clockSecond);
//	virtual void RemoveBuff() override;
//};

//#pragma endregion
#endif