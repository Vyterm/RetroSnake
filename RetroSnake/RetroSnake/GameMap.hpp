#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "GameModel.hpp"
#include "winapi.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <vector>
#include <ctime>

using std::cout;
using std::cin;
using std::endl;
using std::string;

#pragma region CellType & Colors

enum class E_CellType
{
	None = 0,
	Land = 1,
	Food = 2,
	Head = 3,
	Body = 4,
	Jump = 5,
};

enum class E_SubType
{
	// 作为食物是普通食物，作为地形是山峰
	SubType0,
	// 作为食物使对方变长，作为地形是草地
	SubType1,
	// 作为食物使自己变短，作为地形是
	SubType2,
	// 作为食物使自己增速，作为地形是岩浆
	SubType3,
	// 作为食物使对方减速，作为地形是冰块
	SubType4,
	// 作为食物使对方反转，作为地形是
	SubType5,
	// 作为食物自己变刚体，作为地形是
	SubType6,
	// 作为食物对方变失控，作为地形是
	SubType7,
	// 作为食物自己变幽灵，作为地形是
	SubType8,
};

constexpr E_Color SubTypeColors[] = { E_Color::White, E_Color::LGreen, E_Color::LPurple, E_Color::LRed, E_Color::LBlue, E_Color::Black, E_Color::LYellow, E_Color::Green, E_Color::Cyan };

class MapItem
{
public:
	E_CellType type = E_CellType::None;
	E_SubType subType = E_SubType::SubType0;
	Color color = { DEFAULT_FORE_COLOR, DEFAULT_BACK_COLOR };
	Point jumpPoint;
	MapItem() { }
	MapItem(E_CellType type) { this->type = type; }
	MapItem(E_CellType type, Color color) { this->type = type; this->color = color; }
	MapItem(E_CellType type, E_SubType subType, Color color) { Set(type, subType, color); }
	MapItem& operator=(const MapItem &rhs)
	{
		this->type = rhs.type;
		this->subType = rhs.subType;
		this->color = rhs.color;
		this->jumpPoint = rhs.jumpPoint;
		return *this;
	}

	void SetQuote(const std::shared_ptr<void> &quote)
	{
		m_weakQuote = quote;
	}
	bool TryGetQuote(std::shared_ptr<void> &quote)
	{
		quote = m_weakQuote.lock();
		return m_weakQuote.expired();
	}
	/*
	亮	红	绿	蓝
	1	1	1	1
	0x0~0xF表示暗黑色至亮白色
	*/
	void Set(Color color) { Set(type, subType, color); }
	void Set(E_CellType type, Color color) { Set(type, E_SubType::SubType0, color); }
	void Set(E_CellType type, E_SubType subType, Color color)
	{
		this->type = type;
		this->subType = subType;
		this->color = color;
	}
	MapItem& operator=(E_CellType type)
	{
		this->type = type;
		return *this;
	}
	MapItem& operator=(E_SubType subType)
	{
		this->subType = subType;
		return *this;
	}
	MapItem& operator=(Color color)
	{
		this->color = color;
		return *this;
	}
	bool operator==(const MapItem &rhs) const { return type == rhs.type && subType == rhs.subType && color == rhs.color; }
	bool operator!=(const MapItem &rhs) const { return type != rhs.type || subType != rhs.subType || color != rhs.color; }
	bool operator==(const E_CellType &rhs) const { return rhs == type; }
	bool operator!=(const E_CellType &rhs) const { return rhs != type; }
	bool operator==(const E_SubType &rhs) const { return rhs == subType; }
	bool operator!=(const E_SubType &rhs) const { return rhs != subType; }
	bool operator==(const Color &rhs) const { return rhs == color; }
	bool operator!=(const Color &rhs) const { return rhs != color; }
	friend bool operator==(const E_CellType &lhs, const MapItem &rhs) { return lhs == rhs.type; }
	friend bool operator==(const E_SubType &lhs, const MapItem &rhs) { return lhs == rhs.subType; }
	friend bool operator==(const Color &lhs, const MapItem &rhs) { return lhs == rhs.color; }
private:
	std::weak_ptr<void> m_weakQuote;
};

#pragma endregion

template <int Width, int Height>
class LayerTemplate
{
protected:
	MapItem m_items[Width][Height];
public:
	const MapItem& operator[](Point position) const { return m_items[position.x][position.y]; }
	MapItem& operator[](Point position) { return m_items[position.x][position.y]; }
	const MapItem& Index(int x, int y) const { return m_items[x][y]; }
	MapItem& Index(int x, int y) { return m_items[x][y]; }
};
class PlayerCtrl;

template <size_t Width, size_t Height>
class MapTemplate
{
private:
	#pragma region Fields

	MapItem m_items[Width][Height];
	MapItem m_staticItems[Width][Height];

	bool &m_isUpdateUI;
	std::vector<PlayerCtrl*> m_players;
	size_t m_activePlayerCount = 0;

	Point m_position;
	MapItem m_zCacheItems[Width][Height];

	GameMapModel m_model;

	#pragma endregion

	#pragma region Load Map Model

	void LoadStaticCell(const GameMapModel &model, int ci, int ri)
	{
		Point position = { ci, ri };
		m_staticItems[ci][ri].Set(E_CellType::None, E_SubType::SubType0, DEFAULT_COLOR);
		m_items[ci][ri].Set(E_CellType::None, E_SubType::SubType0, DEFAULT_COLOR);
		switch (model.GetType(position))
		{
		case E_StaticCellType::OpenSpace:
			m_staticItems[ci][ri].Set(E_CellType::None, E_SubType::SubType0, DEFAULT_COLOR);
			break;
		case E_StaticCellType::JebelLand:
			m_staticItems[ci][ri].Set(E_CellType::Land, E_SubType::SubType0, DEFAULT_COLOR);
			break;
		case E_StaticCellType::GrassLand:
			m_staticItems[ci][ri].Set(E_CellType::Land, E_SubType::SubType1, { DEFAULT_BACK_COLOR, SubTypeColors[1] });
			break;
		case E_StaticCellType::MagmaLand:
			m_staticItems[ci][ri].Set(E_CellType::Land, E_SubType::SubType3, { DEFAULT_BACK_COLOR, SubTypeColors[3] });
			break;
		case E_StaticCellType::FrostLand:
			m_staticItems[ci][ri].Set(E_CellType::Land, E_SubType::SubType4, { DEFAULT_BACK_COLOR, SubTypeColors[4] });
			break;
		case E_StaticCellType::GermPoint:
			m_items[ci][ri].Set(E_CellType::Head, E_SubType::SubType0, { model.GetColor(position), DEFAULT_BACK_COLOR });
			break;
		case E_StaticCellType::JumpPoint:
			m_staticItems[ci][ri].Set(E_CellType::Jump, E_SubType::SubType0, { model.GetColor(position), DEFAULT_BACK_COLOR });
			break;
		}
	}

	void LoadPlayerCell(const GameMapModel &model)
	{
		m_activePlayerCount = model.PlayerCount();
		for (size_t i = 0; i < m_activePlayerCount; ++i)
			m_players[i]->Reset(model.GetPlayer(i));
		if (m_activePlayerCount == 1)
			m_players[0]->SetEnemy(*m_players[0]);
		else
		{
			m_players[0]->SetEnemy(*m_players[1]);
			m_players[1]->SetEnemy(*m_players[0]);
		}
	}

	void LoadJumpCell(const GameMapModel &model)
	{
		for (auto &jpm : model.GetJumpPoints())
		{
			m_items[jpm.src.x][jpm.src.y].Set(E_CellType::Jump, jpm.color);
			m_items[jpm.src.x][jpm.src.y].jumpPoint = jpm.dest;
			m_items[jpm.dest.x][jpm.dest.y].Set(E_CellType::Jump, jpm.color);
			m_items[jpm.dest.x][jpm.dest.y].jumpPoint = jpm.src;
		}
	}

	#pragma endregion

public:
	#pragma region Construct & Destruct

	MapTemplate(bool &updateUI) : m_isUpdateUI(updateUI)
	{
		m_players.push_back(new PlayerCtrl("玩家一", *this, updateUI, { E_Color::LCyan, DEFAULT_BACK_COLOR }, 'W', 'A', 'S', 'D'));
		m_players.push_back(new PlayerCtrl("玩家二", *this, updateUI, { E_Color::LWhite,DEFAULT_BACK_COLOR }, VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT));
		for (auto &player : m_players)
			player->Clear();
		m_position = { 0, 0 };
	}
	~MapTemplate()
	{
		for (auto &pPlayer : m_players)
			delete pPlayer;
	}

	void SetModel(const GameMapModel &model)
	{
		m_model = model;
	}

	#pragma endregion

	#pragma region Load Map Model Interfaces

	void LoadModel(const GameMapModel &model)
	{
		LoadStaticModel(model);
		LoadPlayerCell(model);
		LoadJumpCell(model);
		GenerateRandomFood(model.get_FoodCount());
	}

	void LoadStaticModel(const GameMapModel &model)
	{
		for (int ci = 0; ci < Width; ++ci)
			for (int ri = 0; ri < Height; ++ri)
				LoadStaticCell(model, ci, ri);
	}

	#pragma endregion

	#pragma region Reuse Methods

	void Reset()
	{
		srand((unsigned)time(nullptr));
		ClearCell();
		LoadModel(m_model);
		SetColor(DEFAULT_COLOR);
		system("cls");
		Draw();
	}

	PlayerCtrl& GetPlayer(int index) { return m_activePlayerCount == 1 || index == 0 ? *m_players[0] : *m_players[1]; }
	PlayerCtrl* CheckOver()
	{
		PlayerCtrl *winer = nullptr;
		if (m_activePlayerCount == 2)
			winer = !m_players[0]->IsAlive() ? m_players[1] : !m_players[1]->IsAlive() ? m_players[0] : nullptr;
		else
			winer = !m_players[0]->IsAlive() ? m_players[0] : nullptr;
		if (nullptr == winer) return winer;
		m_players[0]->Clear();
		m_players[1]->Clear();
		vyt::timer::get_instance().HandleClock();
		return winer;
	}

	#pragma endregion

	#pragma region Render Methods

	MapItem MixCell(int x, int y)
	{
		MapItem upperLayer = m_items[x][y] == E_CellType::None ? m_staticItems[x][y] : m_items[x][y];
		if (m_staticItems[x][y] != E_CellType::Land) return upperLayer;
		if (m_staticItems[x][y] == E_SubType::SubType0)
			upperLayer.Set(DEFAULT_COLOR);
		if (upperLayer.type != E_CellType::Body && upperLayer.type != E_CellType::Head) return upperLayer;
		else if (m_staticItems[x][y] == E_SubType::SubType1)
		{
			auto type = upperLayer.type == E_CellType::Head ? E_CellType::Head : E_CellType::Land;
			auto subType = upperLayer.type == E_CellType::Head ? E_SubType::SubType0 : E_SubType::SubType1;
			Color color = { upperLayer.color.fore, m_staticItems[x][y].color.back };
			upperLayer.Set(type, subType, color);
		}
		else if (m_staticItems[x][y] == E_SubType::SubType4)
		{
			auto type = upperLayer.type;
			auto subType = upperLayer.subType;
			Color color = { upperLayer.color.fore, m_staticItems[x][y].color.back };
			upperLayer.Set(type, subType, color);
		}
		return upperLayer;
	}

	static Color ToSubColor(E_SubType subType)
	{
		return { SubTypeColors[int(subType)], DEFAULT_BACK_COLOR };
	}

	static string ToString(const MapItem &item)
	{
		static const string images[] = { "  ", "■", "☆", "◎", "¤", "※" };
		//static const string images[] = { "  ", "〓", "❀", "◎", "¤", "※" };
		if (item.type == E_CellType::Land)
		{
			switch (item.subType)
			{
			case E_SubType::SubType1:
				return "≡";
			case E_SubType::SubType3:
				return "≈";
			case E_SubType::SubType4:
				return "〓";
			}
		}
		return images[int(item.type)];
	}

	void DrawCell(int x, int y, bool isForce)
	{
		auto item = MixCell(x, y);
		if (!isForce && m_zCacheItems[x][y] == item)
			return;
		m_zCacheItems[x][y] = item;
		DrawCell(m_position.x + x, m_position.y + y, m_zCacheItems[x][y]);
	}

	static void DrawCell(int x, int y, const MapItem &item)
	{
		DrawCell(x, y, item.color, ToString(item));
	}

	static void DrawCell(int x, int y, Color color, const string &text)
	{
		SetPosition(x, y);
		SetColor(color);
		cout << text;
	}

	void Draw(bool isForce = false)
	{
		for (int ri = 0; ri < Height; ++ri)
			for (int ci = 0; ci < Width; ++ci)
				DrawCell(ci, ri, isForce);
	}

	void ClearCell()
	{
		for (int ri = 0; ri < Height; ++ri)
			for (int ci = 0; ci < Width; ++ci)
				m_zCacheItems[ci][ri] = m_items[ci][ri] = MapItem();
	}

	#pragma endregion

	#pragma region Create Methods

	bool SearchEmptyPosition(Point &emptyPoint)
	{
		std::vector<Point> emptyPoints;
		for (int ri = 0; ri < GAME_HEIGHT; ++ri)
			for (int ci = 0; ci < GAME_WIDTH; ++ci)
				if (E_CellType::None == m_staticItems[ci][ri] && E_CellType::None == m_items[ci][ri])
					emptyPoints.push_back({ ci,ri });
		if (0 == emptyPoints.size())
			return false;
		emptyPoint = emptyPoints[rand() % emptyPoints.size()];
		return true;
	}

	bool GenerateRandomFood()
	{
		Point emptyPoint;
		if (!SearchEmptyPosition(emptyPoint))
			return false;
		auto randomType = rand() % 100;
		E_SubType subType = randomType < 0 ? E_SubType::SubType0 :
			randomType < 15 ? E_SubType::SubType1 :
			randomType < 30 ? E_SubType::SubType2 :
			randomType < 50 ? E_SubType::SubType3 :
			randomType < 70 ? E_SubType::SubType4 :
			randomType < 75 ? E_SubType::SubType5 :
			randomType < 80 ? E_SubType::SubType6 : E_SubType::SubType7;

		m_items[emptyPoint.x][emptyPoint.y].Set(E_CellType::Food, subType, { SubTypeColors[int(subType)] ,DEFAULT_BACK_COLOR });
		return true;
	}

	bool GenerateRandomFood(size_t count)
	{
		for (size_t i = 0; i < count; ++i)
			if (!GenerateRandomFood())
				return false;
		return true;
	}

	#pragma endregion

	#pragma region CellType Methods

	const MapItem& operator[](Point position) const { return m_items[position.x][position.y]; }
	MapItem& operator[](Point position) { return m_items[position.x][position.y]; }
	const MapItem& Index(int x, int y) const { return m_items[x][y]; }
	MapItem& Index(int x, int y) { return m_items[x][y]; }
	const MapItem& GetItem(Point position) { return E_CellType::None == m_items[position.x][position.y] ? m_staticItems[position.x][position.y] : m_items[position.x][position.y]; }
	const MapItem& GetStaticItem(Point position) { return m_staticItems[position.x][position.y]; }

	bool MoveAble(int x, int y)
	{
		auto item = GetItem({ x, y });
		return E_CellType::None == item.type ||
			(E_CellType::Land == item.type && E_SubType::SubType1 == item.subType) ||
			//(E_CellType::Land == item.type && E_SubType::SubType4 == item.subType) ||
			(E_CellType::Land == item.type && E_SubType::SubType4 == item.subType);
	}

	bool IsBlocked(const Point &position)
	{
		bool isBlocked = true;
		isBlocked &= !MoveAble(position.x + 1, position.y);
		isBlocked &= !MoveAble(position.x - 1, position.y);
		isBlocked &= !MoveAble(position.x, position.y + 1);
		isBlocked &= !MoveAble(position.x, position.y - 1);
		return isBlocked;
	}

	#pragma endregion
};
typedef MapTemplate<GAME_WIDTH + MAZE_WIDTH, GAME_HEIGHT> GameMap;

#endif
