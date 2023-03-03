#pragma once

// Match Bot States
constexpr int STATE_DEAD = 0;
constexpr int STATE_WARMUP = 1;
constexpr int STATE_START = 2;
constexpr int STATE_FIRST_HALF = 3;
constexpr int STATE_HALFTIME = 4;
constexpr int STATE_SECOND_HALF = 5;
constexpr int STATE_OVERTIME = 6;
constexpr int STATE_END = 7;

// Match Bot State Names
const char MATCH_BOT_STATES[][32] =
{
	"Dead",
	"Warmup",
	"Starting",
	"First Half",
	"Half Time",
	"Second Half",
	"Overtime",
	"Finished"
};

// Match BOT Team Names
const char MATCH_BOT_TEAM_STR[][20] =
{
	"Unassigned",
	"Terrorists",
	"Counter-Terrorists",
	"Spectators"
};

// Match BOT Team Names (Short)
const char MATCH_BOT_TEAM_SHORT[][20] =
{
	"NONE",
	"TR",
	"CT",
	"SPEC"
};

class CMatchBot
{
public:
	void ServerActivate();
	void ServerDeactivate();

	static bool TeamScore(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity);
	static bool ScoreInfo(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity);

	void Enable();
	void Disable();

	static void NextState(int State);

	int GetState();
	const char* GetState(int State);
	void SetState(int State);

	int GetScore(TeamName Team);
	int GetScoreOT(TeamName Team);
	int GetRound();

	const char* GetTag();
	const char* GetTeam(TeamName Team, bool ShortName);

	void SwapTeams();

	bool PlayerConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]);
	bool PlayerJoinTeam(CBasePlayer* Player, int NewTeam);
	void PlayerGetIntoGame(CBasePlayer* Player);
	void PlayerDisconnect(edict_t* pEdict);
	bool PlayerHasRestrictItem(CBasePlayer* Player, ItemID item, ItemRestType type);

	void Status(CBasePlayer* Player);
	void Scores(CBasePlayer* Player, bool Method);
	void Help(CBasePlayer* Player, bool AdminHelp);

	void RoundStart();
	void RoundEnd(int winStatus, ScenarioEventEndRound event, float tmDelay);

	void StartVoteMap(CBasePlayer* Player);
	void StartVoteTeam(CBasePlayer* Player);

	void StartMatch(CBasePlayer* Player);
	void StopMatch(CBasePlayer* Player);
	void RestartMatch(CBasePlayer* Player);

private:
	// Match Bot States
	int m_State = STATE_DEAD;

	// Play Knife Round
	bool m_PlayKnifeRound = false;

	// Scores
	int m_Score[SPECTATOR + 1][STATE_END] = { 0 };

	// Overtime Scores
	int m_ScoreOT[SPECTATOR + 1] = { 0 };

	// Match Bot Variables
	cvar_t* m_MatchTag = nullptr;
	cvar_t* m_PlayersMin = nullptr;
	cvar_t* m_PlayersMax = nullptr;
	cvar_t* m_PlayRounds = nullptr;
	cvar_t* m_PlayRoundsOT = nullptr;
	cvar_t* m_PlayMode = nullptr;
	cvar_t* m_ReadyType = nullptr;
	cvar_t* m_ReadyTime = nullptr;
	cvar_t* m_TeamPickupType = nullptr;
	cvar_t* m_VoteMap = nullptr;
	cvar_t* m_VoteMapType = nullptr;
	cvar_t* m_KnifeRound = nullptr;

	// Match Bot Config Variables
	cvar_t* m_Config[STATE_END + 1] = { nullptr };
};

extern CMatchBot gMatchBot;
