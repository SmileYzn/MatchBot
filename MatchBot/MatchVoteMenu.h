#pragma once

typedef struct S_PLAYER_VOTES
{
	// Vote Kick Array
	std::array<bool, MAX_CLIENTS + 1> VoteKick;

	// Vote Map Array
	std::array<bool, MAX_VOTE_MAPS + 1> VoteMap;

	// Vote Pause Array
	std::array<bool, SPECTATOR + 1U> VotePause;

	// Vote Restart Array
	std::array<bool, STATE_END + 1U> VoteRestart;

	// Vote Surrender Array
	std::array<bool, SPECTATOR + 1U> VoteSurrender;

	// Vote Stop Array
	std::array<bool, MAX_CLIENTS + 1> VoteStop;

	// Reset all votes
	void Reset()
	{
		// Reset Vote Kick
		this->VoteKick.fill(false);

		// Reset Vote Map
		this->VoteMap.fill(false);

		// Reset Vote Pause
		this->VotePause.fill(false);

		// Reset Vote Restart
		this->VoteRestart.fill(false);

		// Reset Vote Surrender
		this->VoteSurrender.fill(false);

		// Reset Vote Stop
		this->VoteStop.fill(false);
	}
} P_PLAYER_VOTES, * LP_PLAYER_VOTES;

class CMatchVoteMenu
{
public:
	// Match Bot States
	void SetState(int State);

	// Player Disconnect
	void PlayerDisconnect(edict_t* pEntity);

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

	// Vote Surrender
	bool VoteSurrender(CBasePlayer* Player);

	// Vote Stop
	bool VoteStop(CBasePlayer* Player);

	// Player Votes
 	std::array<P_PLAYER_VOTES, MAX_CLIENTS + 1> m_Votes;

	// Map List
	std::map<int, std::string> m_MapList;
};

extern CMatchVoteMenu gMatchVoteMenu;