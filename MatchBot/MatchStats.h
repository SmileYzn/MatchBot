#pragma once

// Get player auth index (SteamID)
#define GET_USER_AUTH(Edict) !(Edict->v.flags & FL_FAKECLIENT) ? GETPLAYERAUTHID(Edict) : STRING(Edict->v.netname)

// Match Data
typedef struct S_MATCH_DATA
{
	// Match Start Time
	time_t StartTime;

	// Match End Time
	time_t EndTime;

	// Match Rounds
	int MaxRounds;

	// Match OT ROunds
	int MaxRoundsOT;

	// Map
	std::string Map;

	// Team Type (0 Leaders Sorted, 1 Random, 2 Not Sorted, 3 Skill Sorted, 4 Swap Teams)
	int TeamType;

	// Has Knife Round
	bool KnifeRound;

	// Rounds Played
	int RoundsPlay;

	// Score Terrorists
	int ScoreTRs;

	// Score CTs
	int ScoreCTs;
} P_MATCH_DATA, *LP_MATCH_DATA;

// Player stats
typedef struct S_PLAYER_STATS
{
	// Stats
	int Frags;					// TEST: Player Frags
	int Deaths;					// TEST: Player Deaths
	int Assists;				// TODO: Player Kill Assists
	int Headshots;				// TEST: Headshots by player
	int Shots;					// TEST: Shots by player
	int Hits;					// TEST: Hits done by player
	int HitsReceived;			// TEST: Hits received by player
	int Damage;					// TEST: Damage done by player
	int DamageReceived;			// TEST: Damage received by player
	long Money;					// TEST: Money Balance from player
	int BlindFrags;				// TEST: Player frags when blinded by flashbang

	// HitBox
	int HitBoxAttack[9][2];		// TEST: Hitbox: 0 Hits, 1 Damage
	int HitBoxVictim[9][2];		// TEST: Hitbox: 0 Hits, 1 Damage

	// Rounds
	int RoundsPlay;				// TEST: Rounds Play
	int RoundsWin;				// TEST: Rounds Won
	int RoundsLose;				// TEST: Rounds Lose

	// Bomb
	int BombSpawn;				// TEST: Bomb Spawns
	int BombDrop;				// TEST: Bomb Drops
	int BombPlanting;			// TEST: Bomb Plant Attempts
	int BombPlanted;			// TEST: Bomb Plants
	int BombExploded;			// TEST: Bomb Explosions
	int BombDefusing;			// TEST: Bomb Defusing Attempts
	int BombDefusingKit;		// TEST: Bomb Defusing with Kit Attempts
	int BombDefused;			// TEST: Bomb Defuses
	int BombDefusedKit;			// TEST: Bomb Defuses with Kit

	void Reset()
	{
		// Stats
		this->Frags = 0;
		this->Deaths = 0;
		this->Assists = 0;
		this->Headshots = 0;
		this->Shots = 0;
		this->Hits = 0;
		this->Damage = 0;
		this->DamageReceived = 0;
		this->Money = 0;
		this->BlindFrags = 0;

		// Hitbox Attacker
		memset(this->HitBoxAttack, 0, sizeof(this->HitBoxAttack));

		// Hitbox Victim
		memset(this->HitBoxVictim, 0, sizeof(this->HitBoxVictim));

		// Rounds
		this->RoundsPlay = 0;
		this->RoundsWin = 0;
		this->RoundsLose = 0;

		// Bomb
		this->BombSpawn = 0;
		this->BombDrop = 0;
		this->BombPlanting = 0;
		this->BombPlanted = 0;
		this->BombExploded = 0;
		this->BombDefusing = 0;
		this->BombDefused = 0;
		this->BombDefusingKit = 0;
		this->BombDefusedKit = 0;
	};
} P_PLAYER_STATS, *LP_PLAYER_STATS;

class CMatchPlayerData
{
public:
	// Timers
	time_t ConnectTime;			// Connection Time
	time_t GetIntoGameTime;		// Get Into Game Time
	time_t DisconnectedTime;	// Disconnection Time

	// Team
	TeamName Team;				// Player Team

	// Stats by match states
	std::array<P_PLAYER_STATS, STATE_END + 1> Stats;
};

class CMatchStats
{
public:
	// Match State
	void SetState(int State);

	// Get Score (0 Frags, 1 Deaths)
	int* GetScore(int EntityIndex);
	
	// Connect
	bool PlayerConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]);

	// JoinTeam
	bool PlayerJoinTeam(CBasePlayer* Player, int NewTeam);

	// Get Into Game
	void PlayerGetIntoGame(CBasePlayer* Player);

	// Disconnected
	void PlayerDisconnect(edict_t* pEdict);

	// Round Restart
	void RoundRestart(bool PreRestart);

	// Round Start
	void RoundStart();

	// Round End
	void RoundEnd(int winStatus, ScenarioEventEndRound eventScenario, float tmDelay);

	// Player Damage
	void PlayerDamage(CBasePlayer* Victim, entvars_t* pevInflictor, entvars_t* pevAttacker, float& flDamage, int bitsDamageType);

	// Player Killed
	void PlayerKilled(CBasePlayer* Victim, entvars_t* pevKiller, entvars_t* pevInflictor);

	// Set Animation
	void PlayerSetAnimation(CBasePlayer* Player, PLAYER_ANIM playerAnim);

	// Player Add Account Event
	void PlayerAddAccount(CBasePlayer* Player, int amount, RewardType type, bool bTrackChange);

	// Make Bomber
	void PlayerMakeBomber(CBasePlayer* Player);

	// Bomb Drop
	void PlayerDropItem(CBasePlayer* Player, const char* pszItemName);

	// Bomb Plant
	void PlantBomb(entvars_t* pevOwner, bool Planted);

	// Defuse Bomb Start
	void DefuseBombStart(CBasePlayer* Player);

	// Defuse Bomb End
	void DefuseBombEnd(CBasePlayer* Player, bool Defused);

	// Explode Bomb
	void ExplodeBomb(CGrenade* pThis, TraceResult* ptr, int bitsDamageType);

	// Round end Stats
	void RoundEndStats();

	// Show Enemy HP
	bool ShowHP(CBasePlayer* Player);

	// Show Damage
	bool ShowDamage(CBasePlayer* Player, bool InConsole);

	// Show Received Damage
	bool ShowReceivedDamage(CBasePlayer* Player);

	// Show Round Summary
	bool ShowSummary(CBasePlayer* Player, bool InConsole);

private:
	// Match Data
	// Match Server Data Goes Here!

	// Player Data
	std::map<std::string, CMatchPlayerData> m_Player;

	// Round Stats (Damage and Hits)
	int m_RoundDmg[MAX_CLIENTS + 1][MAX_CLIENTS + 1];
	int m_RoundHit[MAX_CLIENTS + 1][MAX_CLIENTS + 1];

	// Player Scores
	int m_Score[MAX_CLIENTS + 1][2];

	// Match State
	size_t m_State = STATE_DEAD;

	// Match Data
	P_MATCH_DATA m_Data ;
};

extern CMatchStats gMatchStats;