#include "GameEditor.hpp"
#include "GameCtrl.hpp"
#include "vytTimer.hpp"

#include <iostream>
#include <iomanip>

using namespace std;

static const E_Color itemColors[] =
{
	E_Color::White,
	E_Color::LRed,
	E_Color::LYellow,
	E_Color::LGreen,
	E_Color::LCyan,
	E_Color::LPurple,
	E_Color::LBlue,
	E_Color::LWhite,
};

#pragma region Editor Painter

void EditorPainter::set_Type(E_EditType type)
{
	m_pointSet.Clear(m_model);
	m_type = type;
}

void EditorPainter::set_ForeColor(E_Color foreColor)
{
	if (m_pointSet.isValid)
		m_model.SetType(m_pointSet.position, m_cellType, foreColor);
	m_foreColor = foreColor;
}

void EditorPainter::set_CellType(E_StaticCellType cellType)
{
	m_pointSet.Clear(m_model);
	m_cellType = cellType;
}

bool EditorPainter::IsDoublePoint() const
{
	return m_type != E_EditType::PenEraser || m_cellType == E_StaticCellType::JumpPoint;
}

bool EditorPainter::DrawEditLeftKey(Point &position)
{
	bool isDoublePoint = IsDoublePoint();
	if (isDoublePoint)
	{
		if (m_pointSet.isValid)
		{
			m_pointSet.isValid = false;
			Point minPos = { m_pointSet.position.x < position.x ? m_pointSet.position.x : position.x, m_pointSet.position.y < position.y ? m_pointSet.position.y : position.y };
			Point maxPos = { m_pointSet.position.x > position.x ? m_pointSet.position.x : position.x, m_pointSet.position.y > position.y ? m_pointSet.position.y : position.y };
			if (m_cellType == E_StaticCellType::JumpPoint)
				m_model.SetJumpPoint(m_pointSet.position, position, m_foreColor);
			else if (m_cellType == E_StaticCellType::GermPoint)
				m_model.SetType(position, m_cellType, m_foreColor);
			else if (m_type == E_EditType::HollowSet)
				m_model.SetHollowLand(minPos, maxPos, m_cellType);
			else if (m_type == E_EditType::CloseySet)
				m_model.SetCloseyLand(minPos, maxPos, m_cellType);
			else
				return false;
			return true;
		}
		else
		{
			m_pointSet.isValid = true;
			m_pointSet.position = position;
		}
	}
	m_model.SetType(position, m_cellType, m_foreColor);
	return true;
}

bool EditorPainter::DrawEditRightKey(Point &position)
{
	if (!m_pointSet.Clear(m_model))
		m_model.SetType(position, E_StaticCellType::OpenSpace);
	return true;
}

bool EditorPainter::DrawEdit(Point position, E_EditMode mode)
{
	if (mode == E_EditMode::LeftKey)
		return DrawEditLeftKey(position);
	else if (mode == E_EditMode::RightKey)
		return DrawEditRightKey(position);
	return false;
}

bool PointSet::Clear(GameMapModel & model)
{
	if (!isValid) return false;

	isValid = false;
	model.SetType(position, E_StaticCellType::OpenSpace);
	return true;

}

#pragma endregion

#pragma region Record Proc

void GameEditor::ObtainRecord()
{
	INPUT_RECORD stcRecord = { 0 };
	DWORD dwRead;
	ReadConsoleInput(m_hStdin, &stcRecord, 1, &dwRead);
	m_recordQueue.push(stcRecord);
}

bool GameEditor::HandleRecord()
{
	bool breakFlag = true;
	while (!m_recordQueue.empty())
	{
		INPUT_RECORD &stcRecord = m_recordQueue.front();
		if (stcRecord.EventType == KEY_EVENT)
			breakFlag &= KeyEventProc(stcRecord.Event.KeyEvent);
		else if (stcRecord.EventType == MOUSE_EVENT)
			breakFlag &= MouseEventProc(stcRecord.Event.MouseEvent);
		m_recordQueue.pop();
	}
	return breakFlag;
}

bool GameEditor::KeyEventProc(KEY_EVENT_RECORD ker)
{
	if (ker.wVirtualKeyCode == VK_ESCAPE) return false;
	if (!m_isEditingNumber) return true;
	if (!ker.bKeyDown)
		cout << int(ker.wVirtualKeyCode) << "键盘弹起" << endl;
	return true;
}

inline void TryUpdatePainter(const MOUSE_EVENT_RECORD &mer, EditorPainter &painter)
{
	if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 116 && mer.dwMousePosition.Y < 6)
	{
		int type = (mer.dwMousePosition.X/2 - 42) / 4 + (mer.dwMousePosition.Y / 4) * 4;
		painter.set_CellType(type > 6 ? painter.get_CellType() : E_StaticCellType(type));
	}
	else if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 114 && mer.dwMousePosition.Y == 7)
	{
		int type = (mer.dwMousePosition.X/2 - 42) / 5;
		painter.set_Type(type > 3 ? painter.get_Type() : E_EditType(type));
	}
	else if (mer.dwMousePosition.X > 80 && mer.dwMousePosition.X < 116 && mer.dwMousePosition.Y > 8 && mer.dwMousePosition.Y <= 12)
	{
		int index = (mer.dwMousePosition.X / 2 - 42) / 4 + ((mer.dwMousePosition.Y - 8) / 3) * 4;
		painter.set_ForeColor(itemColors[index]);
	}
	else if (mer.dwMousePosition.X >= 86 && mer.dwMousePosition.X < 94 && mer.dwMousePosition.Y == 15)
	{
		string path = OpenFile();
	}
	else if (mer.dwMousePosition.X >= 104 && mer.dwMousePosition.X < 112 && mer.dwMousePosition.Y == 15)
	{
		string path = SaveFile();
	}
}

inline void TryPaint(const MOUSE_EVENT_RECORD &mer, EditorPainter &painter)
{
	Point position = { mer.dwMousePosition.X / 2, mer.dwMousePosition.Y };
	if ((position.x >= 1 && position.x <= 38 && position.y >= 1 && position.y <= 38) ||
		(position.x >= 41 && position.x <= 58 && position.y >= 21 && position.y <= 38))
	{
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED)
			painter.DrawEdit(position, E_EditMode::LeftKey);
		else if (mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
			painter.DrawEdit(position, E_EditMode::RightKey);
	}
}

bool GameEditor::MouseEventProc(MOUSE_EVENT_RECORD mer)
{
	if (m_isEditingNumber)
	{
		if (mer.dwEventFlags != 0)
			return true;
		else
			m_isEditingNumber = false;
	}
	SetPosition(41, 18);
	switch (mer.dwEventFlags)
	{
		break;
	case DOUBLE_CLICK:
		cout << "鼠标双击" << endl;
		break;
	case 0:
		TryUpdatePainter(mer, m_painter);
		if (m_painter.IsDoublePoint())
			TryPaint(mer, m_painter);
	case MOUSE_MOVED:
		if (mer.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED || mer.dwButtonState == RIGHTMOST_BUTTON_PRESSED)
		{
			if (!m_painter.IsDoublePoint())
				TryPaint(mer, m_painter);
			cout << "MM鼠标单击";
		}
		cout << "X:" << setw(2) << setfill('0') << (mer.dwMousePosition.X/2) << "  Y:" << setw(2) << setfill('0') << mer.dwMousePosition.Y << endl;
		break;
	default:
		break;
	}
	return true;
}

#pragma endregion

#pragma region Editor Refresh

//class HighLightTimer : public vyt::timer::handler
//{
//	GameEditor &m_editor;
//public:
//	HighLightTimer(GameEditor &editor) : vyt::timer::handler(100, true), m_editor(editor) { }
//	void Invoke() { m_editor.Refresh(); }
//};

void GameEditor::Refresh()
{
	// ToDo: With timer, but ReadConsoleInput it's blocking mode, and can't use multi-thread, so doesn't implement.
	//static int invokeCount = 0;
	//static bool isHighLight = false;
	//isHighLight = ++invokeCount % 5 == 0;
	static bool isHighLight = true;
	E_Color selectForeColor = m_painter.get_ForeColor();

	static MapItem items[] =
	{
		{E_CellType::None, { DEFAULT_FORE_COLOR, E_Color::Black }},
		{E_CellType::Land, E_SubType::SubType0, { DEFAULT_FORE_COLOR, DEFAULT_BACK_COLOR }},
		{E_CellType::Land, E_SubType::SubType1, { E_Color::Black, E_Color::LGreen }},
		{E_CellType::Land, E_SubType::SubType3, { E_Color::Black, E_Color::LRed }},
		{E_CellType::Land, E_SubType::SubType4, { E_Color::Black, E_Color::LBlue }},
		{E_CellType::Head, DEFAULT_COLOR},
		{E_CellType::Jump, DEFAULT_COLOR},
	};
	static const string itemNames[] =
	{
		"空地",
		"山峰",
		"草丛",
		"岩浆",
		"冰块",
		"出生点",
		"传送点",
		"左画右擦",
		"中空填充",
		"完整填充",
		"白色",
		"红色",
		"黄色",
		"绿色",
		"青色",
		"紫色",
		"蓝色",
		"亮白",
		"加载地图",
		"保存地图",
	};
	items[int(E_StaticCellType::GermPoint)].Set({ selectForeColor, DEFAULT_BACK_COLOR });
	items[int(E_StaticCellType::JumpPoint)].Set({ selectForeColor, DEFAULT_BACK_COLOR });

	E_StaticCellType selectType = m_painter.get_CellType();
	E_EditType selectEditType = m_painter.get_Type();
	int startX = 42, offset = 4, startY = 2;
	for (int i = 0; i < 7; ++i)
	{
		Color textColor = int(selectType) == i ? Color({ E_Color::LWhite, DEFAULT_BACK_COLOR }) : DEFAULT_COLOR;
		GameMap::DrawCell(startX + offset * (i%4), startY + (i / 4) * 2, textColor, itemNames[i]);
		GameMap::DrawCell(startX + offset * (i%4), startY + (i / 4) * 2 + 1, items[i]);
	}

	startX = 42, offset = 5, startY = 7;
	for (int i = 0; i < 3; ++i)
	{
		Color textColor = int(selectEditType) == i ? Color({ E_Color::LWhite, DEFAULT_BACK_COLOR }) : DEFAULT_COLOR;
		GameMap::DrawCell(startX + offset * (i%4), startY + (i / 4) * 2, textColor, itemNames[i + 7]);
	}

	startX = 42, offset = 4, startY = 9;
	for (int i = 0; i < 8; ++i)
	{
		Color textColor = selectForeColor == itemColors[i] ? Color({ E_Color::LWhite, DEFAULT_BACK_COLOR }) : DEFAULT_COLOR;
		GameMap::DrawCell(startX + offset * (i % 4), startY + (i / 4) * 2, textColor, itemNames[i + 10]);
		GameMap::DrawCell(startX + offset * (i % 4), startY + (i / 4) * 2 + 1, {DEFAULT_FORE_COLOR, itemColors[i]}, "      ");
	}

	startX = 43, offset = 9, startY = 16;
	for (int i = 0; i < 2; ++i)
	{
		Color textColor = DEFAULT_COLOR;
		GameMap::DrawCell(startX + offset * (i % 4), startY + (i / 4) * 2, textColor, itemNames[i + 18]);
	}

	SetColor(DEFAULT_COLOR);
}

#pragma endregion

#pragma region Editor Main

GameEditor::GameEditor() : m_painter(m_mapModel)
{
	New();
	m_hStdin = GetStdHandle(STD_INPUT_HANDLE);
	SetConsoleMode(m_hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
}

void GameEditor::Run()
{
	SetColor(DEFAULT_COLOR);

	DWORD dwMode;
	GetConsoleMode(m_hStdin, &dwMode);
	system("cls");// CLS will reset console mode
	//SetConsoleMode(m_hStdin, ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);
	SetConsoleMode(m_hStdin, dwMode);

	bool isUpdateUI;
	GameMap map(isUpdateUI);
	do
	{
		ObtainRecord();
		vyt::timer::get_instance().HandleClock();
		map.LoadStaticModel(m_mapModel);
		map.Draw();
		Refresh();
	} while (HandleRecord());
}

void GameEditor::New()
{
	m_mapModel.Clear();
	m_mapModel.SetHollowLand({ 0, 0 }, { (GAME_WIDTH + 0 - 1), (GAME_HEIGHT + 0 - 1) }, E_StaticCellType::JebelLand);
	m_mapModel.SetHollowLand({ GAME_WIDTH, MSG_HEIGHT }, { (MAZE_WIDTH + GAME_WIDTH - 1), (MAZE_HEIGHT + MSG_HEIGHT - 1) }, E_StaticCellType::JebelLand);
	m_mapModel.SetHollowLand({ GAME_WIDTH, 0 }, { (MSG_WIDTH + GAME_WIDTH - 1), (MSG_HEIGHT + 0 - 1) }, E_StaticCellType::JebelLand);
}

void GameEditor::Load()
{
}

void GameEditor::Save()
{
}

#pragma endregion
