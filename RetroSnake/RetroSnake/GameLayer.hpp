#ifndef GAME_LAYER_HPP_INCLUDED
#define GAME_LAYER_HPP_INCLUDED

#include "GameModel.hpp"

namespace game
{
	template <typename TItem, size_t Width, size_t Height>
	class layer
	{
	protected:
		TItem m_items[Width][Height];
		static const size_t LAYER_WIDTH = Width;
		static const size_t LAYER_HEIGHT = Height;
	public:
		layer() { }
		const TItem & GetItem(Vector2 position) const
		{
			return m_items[position.x][position.y];
		}
		void SetItem(Vector2 position, const TItem &item)
		{
			m_items[position.x][position.y] = item;
		}
		bool Exist(Vector2 position)
		{
			return position.x >= 0 && position.x < Width && position.y >= 0 && position.y < Height;
		}
	};
}

#endif