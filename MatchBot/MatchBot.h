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
	// On Server Activate Event
	void ServerActivate();

	// On Server Deactivate Event
	void ServerDeactivate();

	// Enable match bot
	void Enable();

	// Disable match bot
	void Disable();

	// Set match to run next stage
	static void NextState(int State);

	// Get current match state
	int GetState();

	// Get current match state name
	const char* GetState(int State);

	// Set a new match state
	void SetState(int State);

	// Get score of team
	int GetScore(TeamName Team);

	// Get Overtime score of team
	int GetScoreOT(TeamName Team);

	// Get current match round
	int GetRound();

	// Get Match BOT tag
	const char* GetTag();

	// Get team name
	const char* GetTeam(TeamName Team, bool ShortName);

	// Swap scores event
	void SwapScores();

	// Swap teams event
	static void SwapTeams(int ShowMessage);

	// Get Knife Round Mode
	bool GetKnifeRound();

	// Set Knife Round
	void SetKnifeRound(bool PlayKnifeRound);

	// On player connect
	bool PlayerConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]);

	// On player select a new team
	bool PlayerJoinTeam(CBasePlayer* Player, int NewTeam);

	// On player get into game
	void PlayerGetIntoGame(CBasePlayer* Player);

	// On player disconnect
	void PlayerDisconnect();

	// Show match bot status
	void Status(CBasePlayer* Player);

	// Show match scores
	void Scores(CBasePlayer* Player, bool Method);

	// Show MOTD help
	void Help(CBasePlayer* Player, bool AdminHelp);

	// Round estart event
	void RoundStart();

	// Round end event
	void RoundEnd(int winStatus, ScenarioEventEndRound event, float tmDelay);

	// Round restart event
	void RoundRestart(bool PreRestart);

	// Update game description
	void UpdateGameName();

	// Start vote map
	void StartVoteMap(CBasePlayer* Player);

	// Start vote team
	void StartVoteTeam(CBasePlayer* Player);

	// Start match
	void StartMatch(CBasePlayer* Player);

	// Stop current running match
	void StopMatch(CBasePlayer* Player);

	// Restart current match
	void RestartMatch(CBasePlayer* Player);

	// End Match with a winner
	void EndMatch(TeamName Loser, TeamName Winner);

private:
	// Match Bot States
	int m_State = STATE_DEAD;

	// Play Knife Round
	bool m_PlayKnifeRound = false;

	// Scores
	int m_Score[SPECTATOR + 1][STATE_END];

	// Overtime Scores
	int m_ScoreOT[SPECTATOR + 1];

	// Defaut game description
	char m_GameDesc[32] = { 0 };

	// Match Bot Config Variables
	cvar_t* m_Config[STATE_END + 1];

public:
	// Match Bot Variables
	cvar_t* m_MatchTag = nullptr;
	cvar_t* m_Language = nullptr;
	cvar_t* m_AdminPrefix = nullptr;
	cvar_t* m_PlayerPrefix = nullptr;
	cvar_t* m_PlayersMin = nullptr;
	cvar_t* m_PlayersMax = nullptr;
	cvar_t* m_PlayRounds = nullptr;
	cvar_t* m_PlayRoundsOT = nullptr;
	cvar_t* m_PlayMode = nullptr;
	cvar_t* m_ReadyType = nullptr;
	cvar_t* m_ReadyTime = nullptr;
	cvar_t* m_TeamPickupType = nullptr;
	cvar_t* m_TeamPickMenu = nullptr;
	cvar_t* m_VotePercent = nullptr;
	cvar_t* m_VoteMap = nullptr;
	cvar_t* m_VoteMapType = nullptr;
	cvar_t* m_VoteMapAuto = nullptr;
	cvar_t* m_VoteMapFail = nullptr;
	cvar_t* m_KnifeRound = nullptr;
	cvar_t* m_ScoreType = nullptr;
	cvar_t* m_TeamScore = nullptr;
	cvar_t* m_PlayerScore = nullptr;
	cvar_t* m_GameName = nullptr;
	cvar_t* m_PlayerVoteKick = nullptr;
	cvar_t* m_PlayerVoteMap = nullptr;
	cvar_t* m_PlayerVotePause = nullptr;
	cvar_t* m_PlayerVoteRestart = nullptr;
	cvar_t* m_PlayerVoteSurrender = nullptr;
	cvar_t* m_RoundEndStats = nullptr;
	cvar_t* m_StatsCommands = nullptr;
	cvar_t* m_PlayersMinCheck = nullptr;
	cvar_t* m_RestrictWeapons = nullptr;
	cvar_t* m_ExtraSmokeCount = nullptr;
	cvar_t* m_PauseTime = nullptr;
	cvar_t* m_RetryMode = nullptr;
	cvar_t* m_RetryTime = nullptr;
	cvar_t* m_HelpFile = nullptr;
	cvar_t* m_HelpFileAdmin = nullptr;
};

extern CMatchBot gMatchBot;
