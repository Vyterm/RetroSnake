#ifndef GAME_GRAPHIC_HPP_INCLUDED
#define GAME_GRAPHIC_HPP_INCLUDED

#include "GameModel.hpp"
#include "GameLayer.hpp"
#include "winapi.hpp"

#include <iostream>
#include <string>

using std::string;

namespace game
{

	class RenderItem
	{
	private:
		string m_text;
		RenderColor m_foreColor;
		RenderColor m_backColor;
	public:
		RenderItem& operator=(const string &text) { m_text = text.substr(0, 2); }
		RenderItem& operator=(const RenderColor &color) { m_foreColor = color; }
		void Print(Vector2 position) const;
	};

	class RenderLayer : public layer<RenderItem, GAME_WIDTH+MAZE_WIDTH, GAME_HEIGHT>
	{
	private:
		RenderItem m_zCacheItems[LAYER_WIDTH][LAYER_HEIGHT];
	public:
		void PrintString(const Vector2 &position, const string &text, const RenderColor &color)
		{

		}
	};
}

#endif