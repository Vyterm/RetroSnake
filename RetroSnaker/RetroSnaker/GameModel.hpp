#pragma once
#include "GameLib.hpp"

template <int Width, int Height>
class MapTemplate
{
	E_MapItem m_items[Width][Height];
public:
	E_MapItem * operator[](int x) { return m_items[x]; }
	E_MapItem operator[](Point position) { return m_items[position.x][position.y]; }
};
typedef MapTemplate<GAME_WIDTH, GAME_HEIGHT> Map;