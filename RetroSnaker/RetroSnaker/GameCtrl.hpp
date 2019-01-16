#include "GameModel.hpp"

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
	Map &m_map;
	Snake m_snake;
	int m_score;
public:
	PlayerCtrl(Map &map, Point position, Color color, int kUp, int kLeft, int kDown, int kRight)
		: m_map(map), m_snake(map, position, color), m_score(0), m_speedLevel(1), 
		m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight) { }
	void UpdateDirection();
	E_MoveState Process(int timeDelta) override;

	void IncreaseScore(int score = 1) { m_score += score; }
	int get_Score() const { return m_score; }

	void IncreaseSpeed() { ++m_speedLevel; }
	int get_Speed() const { return m_speedLevel; }
};