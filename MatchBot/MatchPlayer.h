#pragma once

typedef struct S_PLAYER_INFO
{
	int EntityIndex;					// Entity Index
	int UserId;							// User Index
	std::string Auth;					// Auth Id
	std::string Name;					// Player Name
	std::string Address;				// IP Address
	std::string Flags;					// Player Flags (String)
	int AdminFlags;						// Player Flags (Bitsum)
	int Status;							// 0 Disconnected, 1 Connected, 2 In Game
	std::string DcReason;				// Last Disconnect reason
	TeamName LastTeam;					// Last Team
	std::array<int, STATE_END + 1U> Frags;	// Frags by state
	std::array<int, STATE_END + 1U> Deaths;	// Deaths by state
	int Money;							// Last Money
} P_PLAYER_INFO, *LP_PLAYER_INFO;

class CMatchPlayer
{
public:
	// On Server Activate
	void ServerActivate();

	// Get Player Info
	LP_PLAYER_INFO GetInfo(const char* AuthId);
	
	// Get Player Info
	LP_PLAYER_INFO GetInfo(int UserIndex);

	// On Player Connect
	bool PlayerConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]);

	// On Player Join Game
	void PlayerGetIntoGame(CBasePlayer* Player);

	// On Player Change Team
	void PlayerSwitchTeam(CBasePlayer* Player);

	// Update money
	void PlayerAddAccount(CBasePlayer* Player, int Amount, RewardType Type, bool bTrackChange);

	// On Player Disconnect
	void PlayerDisconnect(edict_t* pEntity, bool crash, const char* Reason);

	// Player Menu
	void PlayerMenu(CBasePlayer* Player);

	// Player menu handle
	static void PlayerMenuHandle(int EntityIndex, P_MENU_ITEM Item);

	// Show Player Info
	void ShowInfo(CBasePlayer* Player, int UserIndex);

	// Player Menu Action Handler
	static void PlayerMenuActionHandle(int EntityIndex, P_MENU_ITEM Item);

	// Player Ban Menu Action Handler
	static void PlayerBanMenuActionHandle(int EntityIndex, P_MENU_ITEM Item);
	
private:
	std::map<std::string, P_PLAYER_INFO> m_Player;
};

extern CMatchPlayer gMatchPlayer;