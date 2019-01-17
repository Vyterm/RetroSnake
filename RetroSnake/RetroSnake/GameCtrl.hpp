#ifndef GAME_CTRL_HPP
#define GAME_CTRL_HPP

#include "GameSnake.hpp"
#include "vytTimer.hpp"

#include <string>
#include <vector>
#include <math.h>

using std::string;

constexpr auto SPEED_DELTA = 50;
constexpr auto ACCELERATING_FACTOR = 0.996;

enum class E_BuffType
{
	Unstoppable,
	Incontrollable,
	Slippage,
};
static constexpr auto BuffCount = 3;
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
private:
	#pragma region Fields

	int m_kUp, m_kLeft, m_kDown, m_kRight;
	int m_speedLevel, m_score;
	string m_name;
	GameMap &m_map;
	bool &m_isUpdateUI;
	Snake m_snake;
	bool m_alive = true, m_indeath = true, m_unstoppable = false;
	PlayerCtrl *m_enemy = nullptr;

	#pragma endregion
public:
	#pragma region Properties

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

	#pragma endregion

private:
	#pragma region Process Methods

	#pragma region Process Timer

	vyt::timer::handler *m_timer;

	class TickTock : public vyt::timer::handler
	{
		PlayerCtrl &m_player;
	public:
		TickTock(PlayerCtrl &player, clock_t tickTime, bool isLoop) : vyt::timer::handler(tickTime, isLoop), m_player(player) { }
		void Invoke() { m_player.Process(); }
	};

	#pragma endregion

	void UpdateDirection();
	bool MoveByPosition();
	bool MoveByPosition(const Point &position);

	void HandleFood(const Point& position);
	void HandleTerrain(const Point& position);

	void Reverse();

	void ToNextDeathAnimation();

	#pragma endregion

	#pragma region Player Buffs

	class PlayerBuff : public vyt::timer::handler
	{
		int m_clockSecond;
		int m_tickCount;
		E_Color m_playerColor;
		E_BuffType m_type;
	protected:
		PlayerCtrl &m_player;
		bool m_isAppend;
		PlayerBuff(PlayerCtrl &player, int clockSecond, E_BuffType type);
		virtual ~PlayerBuff();
		virtual E_Color ShiningColour() const { return m_playerColor; }
		virtual void Invoke();
	public:
		virtual void Copy(const PlayerBuff *buff);
		int RemainSecond() const;
		virtual void RemoveBuff();
	};
	PlayerBuff *m_buffs[BuffCount];
	class UnstoppableBuff : public PlayerBuff
	{
	public:
		UnstoppableBuff(PlayerCtrl &player, int clockSecond);
		E_Color ShiningColour() const { return E_Color::LYellow; }
		virtual void RemoveBuff() override;
	};
	class IncontrollableBuff : public PlayerBuff
	{
		int m_kUp, m_kLeft, m_kDown, m_kRight;
	public:
		IncontrollableBuff(PlayerCtrl &player, int clockSecond);
		E_Color ShiningColour() const { return E_Color::Green; }
		virtual void RemoveBuff() override;
	};
	class SlippageBuff : public PlayerBuff
	{
	public:
		SlippageBuff(PlayerCtrl &player, int clockSecond);
		virtual void RemoveBuff() override;
	};

	#pragma endregion

public:
	PlayerCtrl(string name, GameMap &map, bool &isUpdateUI, Color color, int kUp, int kLeft, int kDown, int kRight);
	~PlayerCtrl();

	void Clear();
	void Reset(Point position);

	void Process() override;
};

#endif