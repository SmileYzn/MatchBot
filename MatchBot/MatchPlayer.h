#pragma once

typedef struct S_PLAYER_INFO
{
	std::string Auth;
	std::string Name;
	std::string Address;
	std::string Flags;
	int AdminFlags;
	int Status;				// 0 Disconnected, 1 Connected, 2 In Game
	TeamName LastTeam;
} P_PLAYER_INFO, *LP_PLAYER_INFO;

class CMatchPlayer
{
public:
	// On Server Activate
	void ServerActivate();

	// Get Player Info
	LP_PLAYER_INFO GetInfo(int UserIndex);

	// On Player Connect
	bool PlayerConnect(edict_t* pEdict, const char* pszName, const char* pszAddress, char szRejectReason[128]);

	// On Player Join Game
	void PlayerGetIntoGame(CBasePlayer* Player);

	// On Player Change Team
	void PlayerSwitchTeam(CBasePlayer* Player);

	// On Player Disconnect
	void PlayerDisconnect(edict_t* pEdict);

	// Player Menu
	void PlayerMenu(CBasePlayer* Player);

	// Player menu handle
	static void PlayerMenuHandle(int EntityIndex, P_MENU_ITEM Item);

	// Show Player Info
	void ShowInfo(CBasePlayer* Player, int TargetIndex);

	// Player Menu Action Handler
	static void PlayerMenuActionHandle(int EntityIndex, P_MENU_ITEM Item);

	// Player Ban Menu Action Handler
	static void PlayerBanMenuActionHandle(int EntityIndex, P_MENU_ITEM Item);
	
private:
	std::unordered_map<int, P_PLAYER_INFO> m_Player;
};

extern CMatchPlayer gMatchPlayer;