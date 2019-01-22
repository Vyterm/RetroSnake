#include "GameGraphic.hpp"

#include <iostream>

using namespace std;

inline E_4BitColor To4BitColor(const RenderColor &realColor)
{
	E_4BitColor color4bit = E_4BitColor::Black;
	color4bit = color4bit | E_4BitColor(realColor.get_red() == 0 ? E_4BitColor::Black : realColor.get_red() == 1 ? E_4BitColor::Red : E_4BitColor::LRed);
	color4bit = color4bit | E_4BitColor(realColor.get_green() == 0 ? E_4BitColor::Black : realColor.get_green() == 1 ? E_4BitColor::Green : E_4BitColor::LGreen);
	color4bit = color4bit | E_4BitColor(realColor.get_blue() == 0 ? E_4BitColor::Black : realColor.get_blue() == 1 ? E_4BitColor::Blue : E_4BitColor::LBlue);
	if (color4bit == E_4BitColor::Black)
		color4bit = DEFAULT_BACK_COLOR;
	return color4bit;
}

void game::RenderItem::Print(Vector2 position) const
{
	SetPosition(position.x, position.y);
	SetColor({ To4BitColor(m_foreColor), To4BitColor(m_backColor) });
	cout << m_text;
}

void game::RenderLayer::DrawCell(size_t x, size_t y, bool isForce)
{
	auto &item = m_items[x][y];
	if (!isForce && m_zCacheItems[x][y] == item) return;
	m_zCacheItems[x][y] = item;
	item.Print({ int(x), int(y) });
}

void game::RenderLayer::SetString(const Vector2 & position, const string & text, const RenderColor & color)
{
	size_t index = 0;
	while (index < text.size())
	{
		m_items[position.x + index/2][position.y] = text.substr(index, 2);
		m_items[position.x + index/2][position.y] = color;
		index += 2;
	}
}

void game::RenderLayer::Draw()
{
	Draw(false);
}

void game::RenderLayer::Draw(bool isForce)
{
	for (size_t x = 0; x < LAYER_WIDTH; ++x)
		for (size_t y = 0; y < LAYER_HEIGHT; ++y)
			DrawCell(x, y, isForce);
}
