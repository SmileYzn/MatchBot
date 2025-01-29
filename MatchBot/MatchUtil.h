#pragma once

// Print SayText Colors
constexpr auto PRINT_TEAM_DEFAULT = 0;
constexpr auto PRINT_TEAM_GREY = -1;
constexpr auto PRINT_TEAM_RED = -2;
constexpr auto PRINT_TEAM_BLUE = -3;

// Client Print
constexpr auto PRINT_NOTIFY = 1;
constexpr auto PRINT_CONSOLE = 2;
constexpr auto PRINT_CHAT = 3;
constexpr auto PRINT_CENTER = 4;
constexpr auto PRINT_RADIO = 5;

// Map List File
#define MB_MAP_LIST_FILE "cstrike/addons/matchbot/maplist.txt"

class CMatchUtil
{
public:
	// Make Directory
	int MakeDirectory(const char* Path);

	// Register console variable
	cvar_t* CvarRegister(const char* Name, const char* Value);

	// Send Team Info to player
	void TeamInfo(edict_t* pEntity, int playerIndex, const char* pszTeamName);

	// Make Death Message
	void MakeDeathMessage(edict_t* pKiller, edict_t* pVictim, bool Headshot, const char* Weapon);
	
	// Send say text message
	void SayText(edict_t* pEntity, int Sender, const char* Format, ...);

	// Print client text message
	void ClientPrint(edict_t* pEntity, int msg_dest, const char* Format, ...);

	// Send client console commands
	void ClientCommand(edict_t *pEntity, const char *Format, ...);

	// Get player count in team
	int GetCount(TeamName Team);

	// Get player count in teams
	std::array<int, SPECTATOR + 1U> GetCount();

	// Get players as array and return count
	int GetPlayers(CBasePlayer* Players[MAX_CLIENTS], bool InGameOnly, bool IncludeBots);

	// Get players vector
	std::vector<CBasePlayer*> GetPlayers(bool InGameOnly, bool IncludeBots);

	// Get team players in vector
	std::vector<CBasePlayer*> GetPlayers(TeamName Team, bool IncludeBots);

	// Get player by user index
	CBasePlayer* GetPlayerByUserId(int UserIndex);
	
	// Execute server command
	void ServerCommand(const char* Format, ...);

	// Fix unsigned int16
	unsigned short FixedUnsigned16(float value, float scale);

	//  Fix signed int16
	short FixedSigned16(float value, float scale);

	// Set hudmessage parameters
	hudtextparms_t HudParam(int red, int green, int blue, float x, float y, int effects, float fxtime, float holdtime, float fadeintime, float fadeouttime, int channel);

	// Send hudmessage
	void HudMessage(edict_t* pEntity, hudtextparms_t textparms, const char* Format, ...);

	// Format string returning constant char array result
	const char* FormatString(const char* Format, ...);

	// Load Maplist from maps.ini file
	std::map<int, std::string> GetMapList(bool CurrentMap);

	// Show MOTD to player
	void ShowMotd(edict_t* pEntity, char* Motd, unsigned int MotdLength);

	// Replace all in string
	void ReplaceAll(std::string& String, const std::string& From, const std::string& To);

	// Drop client from server
	void DropClient(int EntityIndex, const char* Format, ...);

	// New Lines and colors
	int ParseLinesAndColors(char* Buffer);

	// Find Player
	CBasePlayer* FindPlayer(std::string Target);

	// Get Steam ID with bots check
	const char* GetPlayerAuthId(edict_t* pEntity);
};

extern CMatchUtil gMatchUtil;

