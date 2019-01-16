#include "GameModel.hpp"

const string Map::_images[] = { " ", "#", "F", "@", "O", "J" };

const Color Map::DefaultColor = { 7,0 };

Snake::Snake(Map & map, Point position, Color color) : m_head(new SnakePart(position)), m_tail(m_head), m_color(color)
{
	map[position].Set(E_CellType::Head, color);
}

Snake::Snake(Snake && snake)
{
	if (this == &snake) return;
	this->m_head = snake.m_head;
	snake.m_head = nullptr;
	this->m_tail = snake.m_tail;
	snake.m_tail = nullptr;
	this->m_color = snake.m_color;
}

Snake::~Snake()
{
	while (nullptr != m_head)
	{
		auto tmp = m_head;
		m_head = m_head->m_last;
		delete tmp;
	}
	m_tail = nullptr;
}

void Snake::TailToHead(Map &map, Point position)
{
	SnakePart *newTail;
	if (nullptr != m_tail->m_next)
	{
		newTail = m_tail->m_next;
		m_tail->m_next->m_last = nullptr;
		m_tail->m_next = nullptr;
	}
	else if (m_tail == m_head)
	{
		newTail = m_head;
		m_head->m_next = new SnakePart(position);
		m_tail = m_head->m_next;
	}
	else
		throw "Bad tail";
	m_tail->m_last = m_head;
	m_head->m_next = m_tail;

	map[m_tail->m_position].Set(E_CellType::None, Map::DefaultColor);
	map[m_head->m_position].Set(E_CellType::Body, m_color);
	m_tail->m_position = position;
	map[m_tail->m_position].Set(E_CellType::Head, m_color);
	m_head = m_tail;
	m_tail = newTail;
}

void Snake::ExtendHead(Map &map, Point position)
{
	m_head->m_next = new SnakePart(position);
	m_head->m_next->m_last = m_head;
	map[m_head->m_position].Set(E_CellType::Body, m_color);
	m_head = m_head->m_next;
	map[m_head->m_position].Set(E_CellType::Head, m_color);
}

void Snake::ExtendTail(Map & map, Point position)
{
	m_tail->m_last = new SnakePart(position);
	m_tail->m_last->m_next = m_tail;
	m_tail = m_tail->m_last;
	map[m_tail->m_position].Set(E_CellType::Body, m_color);
}

void Snake::RemoveTail(Map & map)
{
	auto newTail = m_tail->m_next;
	map[m_tail->m_position].Set(E_CellType::None, Map::DefaultColor);
	delete m_tail;
	m_tail = newTail;
}

bool Snake::Contains(Point position)
{
	auto tmpBody = m_head;
	while (nullptr != tmpBody)
	{
		if (tmpBody->m_position == position)
			return true;
		tmpBody = tmpBody->m_last;
	}
	return false;
}