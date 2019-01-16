#ifndef GAME_MAP_HPP
#define GAME_MAP_HPP

#include "GameLib.hpp"
#include "GameEditor.hpp"
#include "GameColor.hpp"

#include <iostream>
#include <string>
#include <memory>
#include <vector>

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

template <int Width, int Height>
class MapTemplate
{
private:
	#pragma region Fields

	MapItem m_items[Width][Height];
	MapItem m_staticItems[Width][Height];

	std::vector<PlayerCtrl*> m_players;
	int m_activePlayerCount = 0;

	Point m_position;
	MapItem m_zCacheItems[Width][Height];

	GameMapModel m_model;

	#pragma endregion

	#pragma region Load Map Model

	void LoadModel(GameMapModel &model)
	{
		std::vector<Point> specialPoints = LoadStaticModel(model);
		LoadPlayerModel(specialPoints, model);
		GenerateRandomFood(model.get_FoodCount());
	}

	std::vector<Point> LoadStaticModel(GameMapModel & model)
	{
		std::vector<Point> specialPoints;
		for (int ci = 0; ci < Width; ++ci)
			for (int ri = 0; ri < Height; ++ri)
				LoadStaticCell(model, ci, ri, specialPoints);
		return specialPoints;
	}

	void LoadStaticCell(GameMapModel & model, int ci, int ri, std::vector<Point> &specialPoints)
	{
		switch (model.Index(ci, ri).type)
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
		case E_StaticCellType::JumpPoint:
			specialPoints.push_back({ ci, ri });
			break;
		}
	}

	void LoadPlayerModel(std::vector<Point> &specialPoints, GameMapModel & model)
	{
		m_activePlayerCount = 0;
		for (auto &point : specialPoints)
		{
			auto cellModel = model.Index(point.x, point.y);
			if (cellModel.type == E_StaticCellType::GermPoint)
			{
				++m_activePlayerCount;
				auto &player = (m_activePlayerCount == 1 ? *m_players[0] : *m_players[1]);
				player.Reset(point);
			}
		}
		if (m_activePlayerCount == 1)
			m_players[0]->SetEnemy(*m_players[0]);
		else
		{
			m_players[0]->SetEnemy(*m_players[1]);
			m_players[1]->SetEnemy(*m_players[0]);
		}
	}

	#pragma endregion

public:
	#pragma region Construct & Destruct

	MapTemplate(size_t playerCount, bool &updateUI) : m_model(Width, Height)
	{
		m_players.push_back(new PlayerCtrl("玩家一", *this, updateUI, { E_Color::LCyan, DEFAULT_BACK_COLOR }, 'W', 'A', 'S', 'D'));
		m_players.push_back(new PlayerCtrl("玩家二", *this, updateUI, { E_Color::LWhite,DEFAULT_BACK_COLOR }, VK_UP, VK_LEFT, VK_DOWN, VK_RIGHT));
		m_position = { 0, 0 };
		m_model.SetHollowLand(0, GAME_WIDTH, 0, GAME_HEIGHT, E_StaticCellType::JebelLand);
		m_model.SetHollowLand(GAME_WIDTH, MAZE_WIDTH, MSG_HEIGHT, MAZE_HEIGHT, E_StaticCellType::JebelLand);
		m_model.SetHollowLand(GAME_WIDTH, MSG_WIDTH, 0, MSG_HEIGHT, E_StaticCellType::JebelLand);
		m_model.SetCloseyLand(10, 20, 1, 10, E_StaticCellType::GrassLand);
		m_model.SetCloseyLand(1, 38, 38, 1, E_StaticCellType::MagmaLand);
		m_model.SetCloseyLand(1, 38, 36, 2, E_StaticCellType::FrostLand);
		m_model.SetCross(5, 5);
		m_model.SetCross(34, 5);
		m_model.SetCross(5, 34);
		m_model.SetCross(34, 34);
		if (playerCount == 1)
			m_model.Index(GAME_WIDTH / 2, GAME_HEIGHT / 2) = E_StaticCellType::GermPoint;
		else
		{
			m_model.Index(GAME_WIDTH / 2 - 5, GAME_HEIGHT / 2) = E_StaticCellType::GermPoint;
			m_model.Index(GAME_WIDTH / 2 + 5, GAME_HEIGHT / 2) = E_StaticCellType::GermPoint;
		}
		m_model.set_FoodCount(3);
	}
	~MapTemplate()
	{
		for (auto &pPlayer : m_players)
			delete pPlayer;
	}

	#pragma endregion

	#pragma region Reuse Methods

	void Reset()
	{
		srand((unsigned)time(nullptr));
		ClearCell();
		LoadModel(m_model);
		// ToDo: Refactor to model
		GenerateEntryPoint({ E_Color::LBlue, DEFAULT_BACK_COLOR });
		GenerateEntryPoint({ E_Color::LPurple, DEFAULT_BACK_COLOR });
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
		TimerManager::get_instance().HandleClock();
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
		SetPosition(m_position.x + x, m_position.y + y);
		auto color = m_zCacheItems[x][y].color;
		SetColor(color);
		cout << ToString(m_zCacheItems[x][y]);
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

	bool GenerateEntryPoint(const Color &color)
	{
		Point emptyPoint;
		if (!SearchEmptyPosition(emptyPoint))
			return false;
		m_items[emptyPoint.x][emptyPoint.y].Set(E_CellType::Jump, color);
		// ToDo: Temp solution, wait to refactor.
		Point jumpPoint = { rand() % (MAZE_WIDTH - 2) + (GAME_WIDTH + 1),rand() % (MAZE_HEIGHT - 2) + (MSG_HEIGHT + 1) };
		m_items[emptyPoint.x][emptyPoint.y].jumpPoint = jumpPoint;
		m_items[jumpPoint.x][jumpPoint.y].Set(E_CellType::Jump, color);
		m_items[jumpPoint.x][jumpPoint.y].jumpPoint = emptyPoint;

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
