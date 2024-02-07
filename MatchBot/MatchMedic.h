#pragma once

typedef struct S_PLAYER_MATCH_DATA
{
	int Frags;
	int Deaths;
	int Team;
	int Account;
	int Defuser;
} P_PLAYER_MATCH_DATA, *LP_PLAYER_MATCH_DATA;

class CMatchMedic
{
public:
	// Matchbot changed state
	void SetState(int State);

	// Update player
	void PlayerUpdate(CBasePlayer* Player);

	// Account update
	void PlayerAddAccount(CBasePlayer* Player);

	// On player disconnect
	void PlayerDisconnect(edict_t* pEntity);

	// Player Get Into Game
	void PlayerGetIntoGame(CBasePlayer* Player);

private:
	std::map<std::string, P_PLAYER_MATCH_DATA> m_Data;
};

extern CMatchMedic gMatchMedic;