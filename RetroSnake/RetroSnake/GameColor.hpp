#pragma once

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