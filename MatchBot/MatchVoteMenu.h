#pragma once

// Needed players in team to Vote Kick
#define VOTE_MENU_KICK_PLAYERS_MIN 5

// Vote Percentage
#define VOTE_MENU_PERCENTAGE 0.70f

typedef struct S_PLAYER_VOTES
{
	// Vote Kick Array
	std::array<bool, MAX_CLIENTS + 1> VoteKick;

	// Vote Map Array
	std::array<bool, MAX_VOTE_MAPS> VoteMap;

	// Vote Pause Array
	std::array<bool, SPECTATOR + 1> VotePause;

	// Vote Restart Array
	std::array<bool, STATE_END + 1> VoteRestart;

	void Reset()
	{
		// Reset Vote Kick
		std::fill(this->VoteKick.begin(), this->VoteKick.end(), false);

		// Reset Vote Map
		std::fill(this->VoteMap.begin(), this->VoteMap.end(), false);

		// Reset Vote Pause
		std::fill(this->VotePause.begin(), this->VotePause.end(), false);

		// Reset Vote Restart
		std::fill(this->VoteRestart.begin(), this->VoteRestart.end(), false);
	}
} P_PLAYER_VOTES, * LP_PLAYER_VOTES;

class CMatchVoteMenu
{
public:
	// Server Activate
	void ServerActivate();

	// Player Disconnect
	void PlayerDisconnect(edict_t* pEdict);

	// Main Vote Menu
	bool Menu(CBasePlayer* Player);
	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);

	// Vote Kick 
	bool VoteKick(CBasePlayer* Player);
	static void VoteKickHandle(int EntityIndex, P_MENU_ITEM Item);

	// Vote Map
	bool VoteMap(CBasePlayer* Player);
	static void VoteMapHandle(int EntityIndex, P_MENU_ITEM Item);

	// Vote Pause
	bool VotePause(CBasePlayer* Player);

	// Vote Restart
	bool VoteRestart(CBasePlayer* Player);

	// Player Votes
 	std::array<P_PLAYER_VOTES, MAX_CLIENTS + 1> m_Votes = { 0 };

	// Map List
	std::map<size_t, std::string> m_MapList;
};

extern CMatchVoteMenu gMatchVoteMenu;