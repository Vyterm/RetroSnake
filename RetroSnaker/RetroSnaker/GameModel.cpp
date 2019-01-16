#include "GameModel.hpp"


Snake::Snake(Color color) : m_head(nullptr), m_tail(m_head), m_color(color), m_twinkleColor(color)
{
}

Snake::Snake(Snake && snake)
{
	if (this == &snake) return;
	this->m_head = snake.m_head;
	snake.m_head = nullptr;
	this->m_tail = snake.m_tail;
	snake.m_tail = nullptr;
	this->m_color = snake.m_color;
	this->m_twinkleColor = snake.m_twinkleColor;
}

Snake::~Snake()
{
	Clear();
}

void Snake::Reset(Map & map, Point position)
{
	Clear();
	m_tail = m_head = new SnakePart(position);
	map[position].Set(E_CellType::Head, m_color);
	m_twinkleColor = m_color;
}

void Snake::Clear()
{
	while (nullptr != m_head)
	{
		auto tmp = m_head;
		m_head = m_head->m_last;
		delete tmp;
	}
	m_tail = nullptr;
}

void Snake::Twinkle(Map &map, const Color & color)
{
	m_twinkleColor = color;
	auto tmp = m_head->m_last;
	while (nullptr != tmp)
	{
		map[tmp->m_position].color = color;
		tmp = tmp->m_last;
	}
}

void Snake::Reverse(Map & map)
{
	map[m_head->m_position].Set(E_CellType::Body, m_twinkleColor);
	map[m_tail->m_position].Set(E_CellType::Head, m_twinkleColor);
	SnakePart *current = m_tail, *last = nullptr, *next = nullptr;
	while (nullptr != current)
	{
		next = current->m_next;
		current->m_next = last;
		current->m_last = next;
		last = current;
		current = next;
	}
	m_head = m_tail;
	m_tail = last;
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

	map[m_tail->m_position].Set(E_CellType::None, DEFAULT_COLOR);
	map[m_head->m_position].Set(E_CellType::Body, m_twinkleColor);
	m_tail->m_position = position;
	map[m_tail->m_position].Set(E_CellType::Head, m_twinkleColor);
	m_head = m_tail;
	m_tail = newTail;
}

void Snake::ExtendHead(Map &map, Point position)
{
	m_head->m_next = new SnakePart(position);
	m_head->m_next->m_last = m_head;
	map[m_head->m_position].Set(E_CellType::Body, m_twinkleColor);
	m_head = m_head->m_next;
	map[m_head->m_position].Set(E_CellType::Head, m_twinkleColor);
}

void Snake::ExtendTail(Map & map, Point position)
{
	m_tail->m_last = new SnakePart(position);
	m_tail->m_last->m_next = m_tail;
	m_tail = m_tail->m_last;
	map[m_tail->m_position].Set(E_CellType::Body, m_twinkleColor);
}

void Snake::RemoveTail(Map & map)
{
	auto newTail = m_tail->m_next;
	map[m_tail->m_position].Set(E_CellType::None, DEFAULT_COLOR);
	delete m_tail;
	m_tail = newTail;
	m_tail->m_last = nullptr;
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
