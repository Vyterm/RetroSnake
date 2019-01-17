#ifndef GAME_EDITOR_HPP
#define GAME_EDITOR_HPP

#include <map>
#include <vector>

constexpr auto MAX_PLAYER_COUNT = 2;
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
	/*无特殊效果*/ NormalEffect	,
	/*使对方变长*/ AppendLength	,
	/*使自己变短*/ RemoveLength	,
	/*使自己增速*/ Acceleration	,
	/*使对方减速*/ Deceleration	,
	/*使对方反转*/ Reverse		,
	/*自己变刚体*/ BuffStrong	,
	/*对方变失控*/ BuffControl	,
	/*自己变幽灵*/ BuffGhost		,
};

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

struct CellModel
{
	E_StaticCellType type;
	CellModel& operator=(E_StaticCellType type) { this->type = type; return *this; }
};

struct JumpPointModel
{
	Point src;
	Point dest;
	Color color;
};

class GameMapModel
{
private:
	size_t m_width, m_height;
	CellModel *m_cellModels;
	std::map<E_FoodType, size_t> m_foodWeights;
	std::vector<JumpPointModel> m_jumpPoints;
	std::vector<Point> m_germPoints;
	size_t m_foodCount = 1;
	//const CellModel& get_Index(size_t x, size_t y) const { return m_cellModels[x + y * m_width]; }
	//void set_Index(size_t x, size_t y, const CellModel &cell) { m_cellModels[x + y * m_width] = cell; }
	CellModel& Index(int x, int y) { return m_cellModels[x + y * m_width]; }
	const CellModel& Index(int x, int y) const { return m_cellModels[x + y * m_width]; }
public:
	GameMapModel(size_t width, size_t height) : m_width(width), m_height(height), m_cellModels(new CellModel[width*height])
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
	~GameMapModel() { delete[] m_cellModels; }

	void SetHollowLand(int startPosX, int width, int startPosY, int height, E_StaticCellType staticType)
	{
		int endPosX = startPosX + width - 1, endPosY = startPosY + height - 1;
		for (int y = startPosY; y <= endPosY; ++y)
			for (int x = startPosX; x <= endPosX; ++x)
				if (x == startPosX || x == endPosX || y == startPosY || y == endPosY)
					m_cellModels[x + y * m_width] = staticType;
				else
					m_cellModels[x + y * m_width] = E_StaticCellType::OpenSpace;
	}

	void SetCloseyLand(int startPosX, int width, int startPosY, int height, E_StaticCellType staticType)
	{
		int endPosX = startPosX + width - 1, endPosY = startPosY + height - 1;
		for (int y = startPosY; y <= endPosY; ++y)
			for (int x = startPosX; x <= endPosX; ++x)
				m_cellModels[x + y * m_width] = staticType;
	}

	void SetCross(int x, int y)
	{
		Index(x - 1, y) = Index(x, y) = Index(x + 1, y)
			= Index(x, y - 1) = Index(x, y + 1) = E_StaticCellType::JebelLand;
	}

	E_StaticCellType GetType(int x, int y)
	{
		return Index(x, y).type;
	}

	bool SetJumpPoint(Point src, Point dest, Color color)
	{
		for (auto &jpm : m_jumpPoints)
			if (jpm.src == src || jpm.src == dest || jpm.dest == src || jpm.dest == dest)
				return false;
		m_jumpPoints.push_back({ src, dest, color });
		return true;
	}
	const std::vector<JumpPointModel>& GetJumpPoints()
	{
		return m_jumpPoints;
	}

	void SetPlayer(int x, int y)
	{
		Index(x, y) = E_StaticCellType::GermPoint;
		m_germPoints.push_back({ x, y });
		if (PlayerCount() > MAX_PLAYER_COUNT)
			m_germPoints.erase(m_germPoints.begin());
	}
	const Point& GetPlayer(size_t index) const { return m_germPoints[index]; }
	size_t PlayerCount() const { return m_germPoints.size(); }

	size_t& FoodWeight(E_FoodType type) {  }
	size_t get_FoodCount() const { return m_foodCount; }
	void set_FoodCount(size_t foodCount) { m_foodCount = foodCount; }
};

#endif