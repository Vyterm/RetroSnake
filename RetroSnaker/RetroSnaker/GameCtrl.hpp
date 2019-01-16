#include "GameModel.hpp"

class DirectionCtrl
{
protected:
	E_Direction m_direction;
public:
	virtual E_MoveState Process() = NULL;
	virtual ~DirectionCtrl() {}
};

class PlayerCtrl : public virtual DirectionCtrl
{
	int m_kUp, m_kLeft, m_kDown, m_kRight;
	Map &m_map;
	Snake m_snake;
public:
	PlayerCtrl(Map &map, Point position, Color color, int kUp, int kLeft, int kDown, int kRight) : m_map(map), m_snake(map, position, color),
		m_kUp(kUp), m_kLeft(kLeft), m_kDown(kDown), m_kRight(kRight) { }
	void UpdateDirection();
	E_MoveState Process() override;
};