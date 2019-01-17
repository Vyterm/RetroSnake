#ifndef GAME_EDITOR_HPP
#define GAME_EDITOR_HPP

#include <map>
#include <vector>

constexpr auto GAME_NAME = "贪吃蛇大作战(Console Version) by 郭弈天";
constexpr auto GAME_VERSION = "Version 0.7.4";

constexpr auto WIN_HEIGHT = 43;
constexpr auto WIN_WIDTH = 120;
constexpr auto GAME_HEIGHT = 40;
constexpr auto GAME_WIDTH = 40;
constexpr auto MSG_WIDTH = 20;
constexpr auto MSG_HEIGHT = 20;
constexpr auto MAZE_HEIGHT = 20;
constexpr auto MAZE_WIDTH = 20;

constexpr auto MAX_PLAYER_COUNT = 2;

#pragma region Color About

/*
亮	红	绿	蓝
1	1	1	1
0x0~0xF表示暗黑色至亮白色
*/
enum class E_Color : short
{
	Black = 0B0000,
	Blue = 0B0001,
	Green = 0B0010,
	Cyan = 0B0011,
	Red = 0B0100,
	Purple = 0B0101,
	Yellow = 0B0110,
	White = 0B0111,

	Gray = 0B1000,
	LBlue = 0B1001,
	LGreen = 0B1010,
	LCyan = 0B1011,
	LRed = 0B1100,
	LPurple = 0B1101,
	LYellow = 0B1110,
	LWhite = 0B1111,
};
struct Color
{
	E_Color fore, back;
	bool operator==(const Color &rhs) const { return fore == rhs.fore && back == rhs.back; }
	bool operator!=(const Color &rhs) const { return fore != rhs.fore || back != rhs.back; }
	void Set(const Color& color) { fore = color.fore; back = color.back; }
};

constexpr E_Color DEFAULT_FORE_COLOR = E_Color::White;
constexpr E_Color DEFAULT_BACK_COLOR = E_Color::Gray;
constexpr Color DEFAULT_COLOR = { DEFAULT_FORE_COLOR, DEFAULT_BACK_COLOR };

#pragma endregion

#pragma region Point About

struct Point
{
public:
	int x, y;
	bool operator==(const Point &rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Point &rhs) const { return x != rhs.x || y != rhs.y; }
	bool operator<(const Point &rhs) const { return x < rhs.x && y < rhs.y; }
	bool operator<=(const Point &rhs) const { return x <= rhs.x && y <= rhs.y; }
	bool operator>(const Point &rhs) const { return x > rhs.x && y > rhs.y; }
	bool operator>=(const Point &rhs) const { return x >= rhs.x && y >= rhs.y; }
	void Set(const Point& point) { x = point.x; y = point.y; }
};

#pragma endregion

#pragma region Cell Defines

enum class E_StaticCellType
{
	OpenSpace,
	JebelLand,
	GrassLand,
	MagmaLand,
	FrostLand,

	GermPoint,
	JumpPoint,
};

enum class E_ActiveCellType
{
	SnakeFood,
	SnakeHead,
	SnakeBody,
};

enum class E_FoodType
{
	/*无特殊效果*/ NormalEffect,
	/*使对方变长*/ AppendLength,
	/*使自己变短*/ RemoveLength,
	/*使自己增速*/ Acceleration,
	/*使对方减速*/ Deceleration,
	/*使对方反转*/ Reverse,
	/*自己变刚体*/ BuffStrong,
	/*对方变失控*/ BuffControl,
	/*自己变幽灵*/ BuffGhost,
};

#pragma endregion

struct CellModel
{
	E_StaticCellType type;
	E_Color foreColor;
	CellModel& operator=(E_StaticCellType type) { this->type = type; return *this; }
	CellModel& operator=(E_Color foreColor) { this->foreColor = foreColor; return *this; }
};

struct JumpModel
{
	Point src;
	Point dest;
	Color color;
};

struct PlayerModel
{
	Point position;
	E_Color foreColor;
};

template <size_t Width, size_t Height>
class MapModelTemplate
{
private:
	CellModel m_cellModels[Width][Height];
	std::map<E_FoodType, size_t> m_foodWeights;
	std::vector<JumpModel> m_jumpPoints;
	std::vector<Point> m_germPoints;
	size_t m_foodCount = 1;
	//const CellModel& get_Index(size_t x, size_t y) const { return m_cellModels[x + y * m_width]; }
	//void set_Index(size_t x, size_t y, const CellModel &cell) { m_cellModels[x + y * m_width] = cell; }
	CellModel& Index(int x, int y) { return m_cellModels[x][y]; }
	const CellModel& Index(int x, int y) const { return m_cellModels[x][y]; }
	CellModel& Index(const Point &position) { return m_cellModels[position.x][position.y]; }
	const CellModel& Index(const Point &position) const { return m_cellModels[position.x][position.y]; }
public:
	static const size_t WIDTH = Width;
	static const size_t HEIGHT = Height;
	MapModelTemplate()
	{
		m_foodWeights[E_FoodType::NormalEffect] = 20;
		m_foodWeights[E_FoodType::AppendLength] = 10;
		m_foodWeights[E_FoodType::RemoveLength] = 10;
		m_foodWeights[E_FoodType::Acceleration] = 10;
		m_foodWeights[E_FoodType::Deceleration] = 10;
		m_foodWeights[E_FoodType::Reverse] = 10;
		m_foodWeights[E_FoodType::BuffStrong] = 10;
		m_foodWeights[E_FoodType::BuffControl] = 10;
		m_foodWeights[E_FoodType::BuffGhost] = 10;
	}
	~MapModelTemplate() { }

	void Clear()
	{
		for (int x = 0; x < Width; ++x)
			for (int y = 0; y < Height; ++y)
				m_cellModels[x][y] = E_StaticCellType::OpenSpace;
		m_jumpPoints.erase(m_jumpPoints.begin(), m_jumpPoints.end());
		m_germPoints.erase(m_germPoints.begin(), m_germPoints.end());
	}

	#pragma region Land Shape

	void SetHollowLand(Point startPos, Point endPos, E_StaticCellType staticType)
	{
		for (int y = startPos.y; y <= endPos.y; ++y)
			for (int x = startPos.x; x <= endPos.x; ++x)
				if (x == startPos.x || x == endPos.x || y == startPos.y || y == endPos.y)
					m_cellModels[x][y] = staticType;
				else
					m_cellModels[x][y] = E_StaticCellType::OpenSpace;
	}

	void SetCloseyLand(Point startPos, Point endPos, E_StaticCellType staticType)
	{
		for (int y = startPos.y; y <= endPos.y; ++y)
			for (int x = startPos.x; x <= endPos.x; ++x)
				m_cellModels[x][y] = staticType;
	}

	void SetCross(Point position)
	{
		Index(position.x - 1, position.y) = Index(position.x, position.y) = Index(position.x + 1, position.y)
			= Index(position.x, position.y - 1) = Index(position.x, position.y + 1) = E_StaticCellType::JebelLand;
	}

	void SetType(Point position, E_StaticCellType type, E_Color foreColor = DEFAULT_FORE_COLOR)
	{
		if (type == E_StaticCellType::GermPoint)
			SetPlayer(position, foreColor);
		else
			Index(position.x, position.y) = { type, foreColor };
	}

	E_StaticCellType GetType(const Point &position) const
	{
		return Index(position.x, position.y).type;
	}

	E_Color GetColor(const Point &position) const
	{
		return Index(position).foreColor;
	}

	#pragma endregion

	#pragma region Jump Point

	bool SetJumpPoint(Point src, Point dest, E_Color foreColor)
	{
		for (auto &jpm : m_jumpPoints)
			if (jpm.src == src || jpm.src == dest || jpm.dest == src || jpm.dest == dest)
				return false;
		Index(src) = Index(dest) = { E_StaticCellType::JumpPoint, foreColor };
		m_jumpPoints.push_back({ src, dest, { foreColor, DEFAULT_BACK_COLOR } });
		return true;
	}

	#pragma endregion

	#pragma region Germ Point

	const std::vector<JumpModel>& GetJumpPoints() const
	{
		return m_jumpPoints;
	}

	void SetPlayer(Point position, E_Color foreColor)
	{
		Index(position) = { E_StaticCellType::GermPoint, foreColor };
		m_germPoints.push_back(position);
		if (PlayerCount() > MAX_PLAYER_COUNT)
		{
			auto invalidPlayer = m_germPoints[0];
			Index(invalidPlayer) = { E_StaticCellType::OpenSpace, DEFAULT_FORE_COLOR };
			m_germPoints.erase(m_germPoints.begin());
		}
	}
	const Point& GetPlayer(size_t index) const { return m_germPoints[index]; }
	size_t PlayerCount() const { return m_germPoints.size(); }

	#pragma endregion

	#pragma region Food Config

	size_t& FoodWeight(E_FoodType type) {  }
	size_t get_FoodCount() const { return m_foodCount; }
	void set_FoodCount(size_t foodCount) { m_foodCount = foodCount; }

	#pragma endregion
};

typedef MapModelTemplate<GAME_WIDTH + MAZE_WIDTH, GAME_HEIGHT> GameMapModel;

#endif