#pragma once
#include "GameModel.hpp"
#include "winapi.hpp"

#include <queue>

enum class E_EditType
{
	PenEraser,
	HollowSet,
	CloseySet,
};

enum class E_EditMode
{
	LeftKey,
	RightKey,
};

struct PointSet
{
	PointSet(Point position, bool isValid) : position(position), isValid(isValid) {}
	Point position;
	bool isValid;
	bool Clear(GameMapModel &model);
};

class EditorPainter
{
	GameMapModel &m_model;
	E_EditType m_type;
	E_Color m_foreColor;
	E_StaticCellType m_cellType;
	PointSet m_pointSet;
public:
	E_EditType get_Type() const { return m_type; }
	void set_Type(E_EditType type);
	E_Color get_ForeColor() const { return m_foreColor; }
	void set_ForeColor(E_Color foreColor);
	E_StaticCellType get_CellType() const { return m_cellType; }
	void set_CellType(E_StaticCellType cellType);
	bool IsDoublePoint() const;
	bool DrawEditLeftKey(Point &position);
	bool DrawEditRightKey(Point &position);
public:
	EditorPainter(GameMapModel &model)
		: m_model(model), m_type(E_EditType::PenEraser), m_foreColor(DEFAULT_FORE_COLOR), m_cellType(E_StaticCellType::OpenSpace), m_pointSet({0,0}, false) { }
	bool DrawEdit(Point position, E_EditMode mode);
};

class GameEditor
{
	HANDLE m_hStdin;
	GameMapModel m_mapModel;
	EditorPainter m_painter;

	std::queue<INPUT_RECORD> m_recordQueue;
	bool m_isEditingNumber;
	void ObtainRecord();
	bool HandleRecord();
	bool KeyEventProc(KEY_EVENT_RECORD ker);
	bool MouseEventProc(MOUSE_EVENT_RECORD mer);
public:
	GameEditor();
	void Run();
	void New();
	void Load();
	void Save();
	void Refresh();
};
