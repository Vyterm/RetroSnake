#ifndef GAME_COLOR_HPP_INCLUDED
#define GAME_COLOR_HPP_INCLUDED

#include <iostream>

#pragma region Console 4bit Color

/*
亮	红	绿	蓝
1	1	1	1
0x0~0xF表示暗黑色至亮白色
*/
enum class E_4BitColor : unsigned short
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
struct ConsoleColor
{
	E_4BitColor fore, back;
	bool operator==(const ConsoleColor &rhs) const { return fore == rhs.fore && back == rhs.back; }
	bool operator!=(const ConsoleColor &rhs) const { return fore != rhs.fore || back != rhs.back; }
	void Set(const ConsoleColor& color) { fore = color.fore; back = color.back; }
	friend std::ostream& operator<<(std::ostream& os, ConsoleColor& color)
	{
		os << int(color.fore) << " " << int(color.back) << " ";
		return os;
	}
	friend std::istream& operator>>(std::istream& is, ConsoleColor& color)
	{
		int fore, back;
		is >> fore >> back;
		color.fore = E_4BitColor(fore);
		color.back = E_4BitColor(back);
		return is;
	}
};

constexpr E_4BitColor DEFAULT_FORE_COLOR = E_4BitColor::White;
constexpr E_4BitColor DEFAULT_BACK_COLOR = E_4BitColor::Gray;
constexpr ConsoleColor DEFAULT_COLOR = { DEFAULT_FORE_COLOR, DEFAULT_BACK_COLOR };

#pragma endregion

#pragma region Real 32bit Color

struct RenderColor
{
	typedef unsigned char ubyte;
	ubyte red : 8;
	ubyte green : 8;
	ubyte blue : 8;
	ubyte alpha : 8;
};

#pragma endregion

#endif