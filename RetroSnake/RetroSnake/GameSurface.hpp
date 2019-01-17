#ifndef GAME_SURFACE_HPP
#define GAME_SURFACE_HPP

#include "winapi.hpp"
#include "GameCtrl.hpp"

#include <vector>
#include <string>
#include <sstream>
using std::string;

class SurfaceText
{
	string m_text;
	const string PLAYER_SPACE = "        ";
public:
	SurfaceText(string text) : m_text(text) { }
	SurfaceText(string player1, string player2)
	{
		std::ostringstream oss;
		oss << player1 << PLAYER_SPACE << player2;
		m_text = oss.str();
	}
	template <typename T>
	SurfaceText(string prefix, T player, std::streamsize width, char fill, string suffix = "")
	{
		std::ostringstream oss;
		oss << prefix << std::setw(width) << std::setfill(fill) << player << suffix;
		m_text = oss.str();
	}
	template <typename T>
	SurfaceText(string prefix, T player1, T player2, std::streamsize width, char fill, string suffix = "")
	{
		std::ostringstream oss;
		oss << prefix << std::setw(width) << std::setfill(fill) << player1 << suffix
			<< PLAYER_SPACE << prefix << std::setw(width) << std::setfill(fill) << player2 << suffix;
		m_text = oss.str();
	}
	size_t size()const { return m_text.size(); }
	friend std::ostream& operator<<(std::ostream &os, const SurfaceText& text)
	{
		os << text.m_text;
		return os;
	}
};
using Msgs = std::vector<SurfaceText>;

void DrawBorder(int posXS, int posXE, int posYS, int posYE);
void DrawHollowBorder(int posXS, int posXE, int posYS, int posYE);
void UnfinishedSurface(int x, int y, DWORD millseconds, string text);
void StartSurface(size_t &selectIndex);
void OverSurface(const PlayerCtrl &winer, bool isWin);

void ShowMsg(Msgs &&msgs);

void ShowMsg(const PlayerCtrl &player1, const PlayerCtrl &player2);

#endif