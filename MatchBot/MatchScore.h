#pragma once

class CMatchScore
{
public:
    // On server actiave event
    void ServerActivate();

	// Update Game Name
	void UpdateGameName();

	// Get TeamScore enabled
	bool GetScoreBoardTeam();

	// TeamScore HL1 Message
	static bool TeamScore(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity);

	// Get PlayerScore enabled
	bool GetScoreBoardPlayer();

	// ScoreInfo HL1 Message
	static bool ScoreInfo(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity);

private:
	// Defaut game description
	char m_GameDesc[32] = { 0 };

	// Store team scores in scoreboard
	cvar_t* m_TeamScore;

	// Store player scores in scoreboard
	cvar_t* m_PlayerScore;

	// Display states and scores at game description
	cvar_t* m_GameName;
};

extern CMatchScore gMatchScore;