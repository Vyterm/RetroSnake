#include "GameModel.hpp"
#include <string>
#include <math.h>

using std::string;

constexpr auto SPEED_DELTA = 80;
constexpr auto ACCELERATING_FACTOR = 0.99;

class DirectionCtrl
{
protected:
	E_Direction m_direction;
public:
	virtual E_MoveState Process(int timeDelta) = NULL;
	virtual ~DirectionCtrl() {}
};

class PlayerCtrl : public virtual DirectionCtrl
{
	int m_kUp, m_kLeft, m_kDown, m_kRight;
	int m_speedLevel, m_moveRemain;
	string m_name;
	Map &m_map;
	Snake m_snake;
	int m_score;
public:
	PlayerCtrl(string name, Map &map, Point position, Color color, int kUp, int kLeft, int kDown, int kRight)
		: m_name(name), m_map(map), m_snake(map, position, color), m_score(0), m_speedLevel(0), 
		m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight) { }
	void UpdateDirection();
	E_MoveState Process(int timeDelta) override;

	void IncreaseScore(int score = 1) { m_score += score; }
	int get_Score() const { return m_score; }

	void IncreaseSpeed() { ++m_speedLevel; }
	int get_Speed() const { return int(SPEED_DELTA / pow(ACCELERATING_FACTOR, m_speedLevel)); }

	Color get_Color() const { return m_snake.get_color(); }
	string get_Name() const { return m_name; }
};