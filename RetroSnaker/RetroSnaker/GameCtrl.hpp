#include "GameModel.hpp"
#include <string>
#include <math.h>

using std::string;

constexpr auto SPEED_DELTA = 100;
constexpr auto ACCELERATING_FACTOR = 0.995;

class DirectionCtrl
{
protected:
	E_Direction m_direction;
public:
	virtual bool Process(int timeDelta) = NULL;
	virtual bool IsAlive() const = NULL;
	virtual ~DirectionCtrl() {}
};

class PlayerCtrl : public virtual DirectionCtrl
{
	int m_kUp, m_kLeft, m_kDown, m_kRight;
	int m_speedLevel, m_moveRemain;
	string m_name;
	Map &m_map;
	Snake m_snake;
	bool m_alive = true;
	PlayerCtrl *m_enemy = nullptr;
	void HandleFood(const Point& position);
	void HandleTerrain(const Point& position);
public:
	PlayerCtrl(string name, Map &map, Point position, Color color, int kUp, int kLeft, int kDown, int kRight)
		: m_name(name), m_map(map), m_snake(map, position, color), m_speedLevel(0), 
		m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight) { }
	void UpdateDirection();
	bool MoveByDirection();
	bool Process(int timeDelta) override;

	int get_Speed() const { return int(SPEED_DELTA / pow(ACCELERATING_FACTOR, m_speedLevel)); }

	Color get_Color() const { return m_snake.get_color(); }
	string get_Name() const { return m_name; }

	bool get_Alive() const { return m_alive; }
	void set_Alive(bool alive) { m_alive = alive; }
	bool IsAlive() const { return m_alive; }

	void SetEnemy(PlayerCtrl &enemy) { m_enemy = &enemy; }
};