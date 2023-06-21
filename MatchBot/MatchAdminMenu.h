#pragma once

class CMatchAdminMenu
{
public:
	// Main Menu
	void MainMenu(int EntityIndex);
	static void MainMenuHandle(int EntityIndex, P_MENU_ITEM Item);

	// Kick Menu
	void KickMenu(int EntityIndex);
	static void KickMenuHandle(int EntityIndex, P_MENU_ITEM Item);

	// Ban Menu
	void BanMenu(int EntityIndex);
	static void BanMenuHandle(int EntityIndex, P_MENU_ITEM Item);

	// Kill Menu
	void SlayMenu(int EntityIndex);
	static void SlayMenuHandle(int EntityIndex, P_MENU_ITEM Item);

	// Team Menu
	void TeamMenu(int EntityIndex);
	static void TeamMenuHandle(int EntityIndex, P_MENU_ITEM Item);

	// Map Menu
	void MapMenu(int EntityIndex);
	static void MapMenuHandle(int EntityIndex, P_MENU_ITEM Item);

	// Control Match BOT menu
	void ControlMenu(int EntityIndex);
	static void ControlMenuHandle(int EntityIndex, P_MENU_ITEM Item);

	// Send global message
	bool Message(CBasePlayer* Player);

	// Send rcon command
	bool Rcon(CBasePlayer* Player);

	// Swap teams
	void SwapTeams(int EntityIndex);
};

extern CMatchAdminMenu gMatchAdminMenu;

