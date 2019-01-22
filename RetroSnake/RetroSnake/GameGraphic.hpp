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
		RenderItem() { }
		RenderItem(RenderColor backColor) : m_backColor(backColor) {}
		RenderItem& operator=(const string &text) { m_text = text; return *this; }
		RenderItem& operator=(const RenderColor &color) { m_foreColor = color; return *this; }
		bool operator==(const RenderItem& rhs) const
		{
			return m_text == rhs.m_text &&
				m_foreColor == rhs.m_foreColor &&
				m_backColor == rhs.m_backColor;
		}
		bool operator!=(const RenderItem& rhs) const
		{
			return m_text != rhs.m_text ||
				m_foreColor != rhs.m_foreColor ||
				m_backColor != rhs.m_backColor;
		}
		void Print(Vector2 position) const;
	};

	class RenderLayer : public layer<RenderItem, GAME_WIDTH+MAZE_WIDTH, GAME_HEIGHT>
	{
	private:
		RenderItem m_zCacheItems[LAYER_WIDTH][LAYER_HEIGHT];
		void DrawCell(size_t x, size_t y, bool isForce);
	public:
		RenderLayer() { }
		void SetString(const Vector2 &position, const string &text, const RenderColor &color);
		void Draw();
		void Draw(bool isForce);
	};
}

#endif