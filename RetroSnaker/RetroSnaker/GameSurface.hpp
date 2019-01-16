#ifndef GAME_SURFACE_HPP
#define GAME_SURFACE_HPP

#include "GameLib.hpp"
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

inline void ShowMsg(const PlayerCtrl &player1, const PlayerCtrl &player2)
{
	if (&player1 == &player2)
	{
		ShowMsg({
			{ "" },
			{ "" },
			{ "玩家一" },
			{ "分数", player1.get_Score(), 3, '0' },
			{ "速度", player1.get_Speed(), 3, '0' },
			{ "刚体:", player1.get_BuffRemainSecond(E_BuffType::Unstoppable), 2, '0', "秒" },
			{ "失控:", player1.get_BuffRemainSecond(E_BuffType::Incontrollable), 2, '0', "秒" },
			{ "打滑:", player1.get_BuffRemainSecond(E_BuffType::Slippage), 2, '0', "秒" },
			{ "" },
			{ "" },
			{ "W,A,S,D操作玩家一" },
			{ "" },
			{ "空格键暂停游戏" },
			});
	}
	else
	{
		ShowMsg({
			{ "" },
			{ "" },
			{ "玩家一", "玩家二" },
			{ "分数", player1.get_Score(), player2.get_Score(), 3, '0' },
			{ "速度", player1.get_Speed(), player2.get_Speed(), 3, '0' },
			{ "刚体:", player1.get_BuffRemainSecond(E_BuffType::Unstoppable), player2.get_BuffRemainSecond(E_BuffType::Unstoppable), 2, '0', "秒" },
			{ "失控:", player1.get_BuffRemainSecond(E_BuffType::Incontrollable), player2.get_BuffRemainSecond(E_BuffType::Incontrollable), 2, '0', "秒" },
			{ "打滑:", player1.get_BuffRemainSecond(E_BuffType::Slippage), player2.get_BuffRemainSecond(E_BuffType::Slippage), 2, '0', "秒" },
			{ "" },
			{ "" },
			{ "W,A,S,D操作玩家一" },
			{ "↑,↓,←,→操作玩家二" },
			{ "空格键暂停游戏" },
			});
	}
}

#endif