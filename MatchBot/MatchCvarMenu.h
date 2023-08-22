#pragma once

// Default cvar menu file
constexpr auto MB_CVAR_MENU_FILE = "cstrike/addons/matchbot/cvars.json";

typedef struct S_CVAR_MENU_INFO
{
	int Access;
	cvar_t* Variable;
	std::vector<std::string> Values;
} P_CVAR_MENU_INFO, *LP_CVAR_MENU_INFO;

class CMatchCvarMenu
{
public:
	void ServerActivate();
	void Menu(CBasePlayer* Player);
	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);
	void UpdateValue(int ItemIndex);

private:
	int m_CvarCount = 0;
	std::map<int, P_CVAR_MENU_INFO> m_Data;
};

extern CMatchCvarMenu gMatchCvarMenu;