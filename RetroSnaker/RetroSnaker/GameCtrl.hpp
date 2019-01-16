#ifndef GAME_CTRL_HPP
#define GAME_CTRL_HPP

#include "GameModel.hpp"
#include "GameTimer.hpp"

#include <string>
#include <vector>
#include <math.h>

using std::string;

constexpr auto SPEED_DELTA = 50;
constexpr auto ACCELERATING_FACTOR = 0.995;

enum class E_BuffType
{
	Unstoppable,
	Incontrollable,
};
static constexpr auto BuffCount = 2;
class DirectionCtrl
{
protected:
	E_Direction m_direction;
public:
	virtual void Process() = NULL;
	virtual bool IsAlive() const = NULL;
	virtual ~DirectionCtrl() {}
};

class PlayerCtrl : public virtual DirectionCtrl
{
	int m_kUp, m_kLeft, m_kDown, m_kRight;
	int m_speedLevel, m_score;
	string m_name;
	Map &m_map;
	bool &m_isUpdateUI;
	Snake m_snake;
	bool m_alive = true, m_unstoppable = false;
	PlayerCtrl *m_enemy = nullptr;
	void HandleFood(const Point& position);
	void HandleTerrain(const Point& position);
	TimerManager::handler &m_timer;

	class TickTock : public TimerManager::handler
	{
		PlayerCtrl &m_player;
	public:
		TickTock(PlayerCtrl &player, clock_t tickTime, bool isLoop) : TimerManager::handler(tickTime, isLoop), m_player(player) { }
		void Invoke() { m_player.Process(); }
	};

	class PlayerBuff : public TimerManager::handler
	{
		int m_clockSecond;
		int m_tickCount;
		E_Color m_playerColor;
	protected:
		PlayerCtrl &m_player;
		PlayerBuff(PlayerCtrl &player, int clockSecond);
		virtual ~PlayerBuff();
		virtual void Invoke();
		virtual E_BuffType Type() const = NULL;
		//virtual Color Color() const = NULL;
		//virtual E_BuffType Type() const { return E_BuffType(-1); }
	public:
		virtual void Copy(const PlayerBuff *buff);
		int RemainSecond() const;
		virtual void Reset();
	};
	PlayerBuff *m_buffs[BuffCount];
	class UnstoppableBuff : public PlayerBuff
	{
	public:
		UnstoppableBuff(PlayerCtrl &player, int clockSecond);
		~UnstoppableBuff();
		E_BuffType Type() const { return E_BuffType::Unstoppable; }
		virtual void Reset() override;
	};
	class IncontrollableBuff : public PlayerBuff
	{
		int m_kUp, m_kLeft, m_kDown, m_kRight;
	public:
		IncontrollableBuff(PlayerCtrl &player, int clockSecond);
		~IncontrollableBuff();
		E_BuffType Type() const { return E_BuffType::Incontrollable; }
		virtual void Reset() override;
	};
	void Reverse();
public:
	PlayerCtrl(string name, Map &map, bool &isUpdateUI, Color color, int kUp, int kLeft, int kDown, int kRight);
	~PlayerCtrl();

	void ClearBuff();
	void Reset(Point position);

	void UpdateDirection();
	bool MoveByPosition();
	bool MoveByPosition(const Point &position);
	void Process() override;

	int get_Speed() const { return int(SPEED_DELTA / pow(ACCELERATING_FACTOR, m_speedLevel)); }

	void IncreaseScore() { ++m_score; }
	int get_Score() const { return m_score; }

	int get_BuffRemainSecond(E_BuffType type) const { return m_buffs[int(type)] == nullptr ? 0 : m_buffs[int(type)]->RemainSecond(); }

	void set_Color(const Color &color) { m_snake.Twinkle(m_map, color); }
	Color get_Color() const { return m_snake.get_color(); }
	string get_Name() const { return m_name; }

	bool get_Alive() const { return m_alive; }
	void set_Alive(bool alive)
	{
		if (!m_unstoppable || alive)
			m_alive = alive;
		else
			m_alive = !m_map.IsBlocked(m_snake.get_headPosition());
	}
	bool IsAlive() const { return m_alive; }

	void SetEnemy(PlayerCtrl &enemy) { m_enemy = &enemy; }
};

#endif