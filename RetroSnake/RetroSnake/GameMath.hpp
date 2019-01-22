#ifndef GAME_MATH_HPP_INCLUDED
#define GAME_MATH_HPP_INCLUDED

#include <iostream>

struct Vector2
{
public:
	int x, y;
	bool operator==(const Vector2 &rhs) const { return x == rhs.x && y == rhs.y; }
	bool operator!=(const Vector2 &rhs) const { return x != rhs.x || y != rhs.y; }
	bool operator<(const Vector2 &rhs) const { return x < rhs.x && y < rhs.y; }
	bool operator<=(const Vector2 &rhs) const { return x <= rhs.x && y <= rhs.y; }
	bool operator>(const Vector2 &rhs) const { return x > rhs.x && y > rhs.y; }
	bool operator>=(const Vector2 &rhs) const { return x >= rhs.x && y >= rhs.y; }
	void Set(const Vector2& point) { x = point.x; y = point.y; }
	friend std::ostream& operator<<(std::ostream& os, Vector2& point)
	{
		os << point.x << " " << point.y << " ";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, Vector2& point)
	{
		is >> point.x >> point.y;
		return is;
	}
};

#endif