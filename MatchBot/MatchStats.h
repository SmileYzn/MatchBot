#pragma once

// Get player auth index (SteamID)
#define GET_USER_AUTH(Edict) !(Edict->v.flags & FL_FAKECLIENT) ? g_engfuncs.pfnGetPlayerAuthId(Edict) : STRING(Edict->v.netname)

// Round Win Share
constexpr auto MANAGER_ASSISTANCE_DMG = 50.0f;	// Mininum damage to take assistance for player
constexpr auto MANAGER_RWS_MAP_TARGET = 0.70f;	// Round Win Share: Amount added to each player of winner team if map objective is complete by winner team
constexpr auto MANAGER_RWS_C4_EXPLODE = 0.30f;	// Round Win Share: Extra amount added to player from winner team that planted the bomb and bomb explode
constexpr auto MANAGER_RWS_C4_DEFUSED = 0.30f;	// Round Win Share: Extra amount added to player from winner team that defused the bomb;

// Flags for stats commands
constexpr auto CMD_ALL	= 0;		/* All Options */
constexpr auto CMD_HP	= BIT(0);	/* flag "a" */
constexpr auto CMD_DMG	= BIT(1);	/* flag "b" */
constexpr auto CMD_RDMG = BIT(2);	/* flag "c" */
constexpr auto CMD_SUM	= BIT(3);	/* flag "d" */

// Stats file path
constexpr auto STATS_SAVE_PATH = "cstrike/addons/matchbot/stats";

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

	// Hostname
	std::string HostName;

	// Map
	std::string Map;

	// Address
	std::string Address;

	// Game Mode (0 Leaders Sorted, 1 Random, 2 Not Sorted, 3 Skill Sorted, 4 Swap Teams, 5 Knife Round)
	int GameMode;

	// Has Knife Round
	bool KnifeRound;

	// Rounds Played
	int RoundsPlay;

	// Score Terrorists
	int ScoreTRs;

	// Score CTs
	int ScoreCTs;

	// Winner team of match
	int Winner;

	// Reset Data
	void Reset()
	{
		this->StartTime = 0;
		this->EndTime = 0;
		this->MaxRounds = 0;
		this->MaxRoundsOT = 0;
		this->HostName = "";
		this->Map = "";
		this->Address = "";
		this->GameMode = 0;
		this->KnifeRound = 0;
		this->RoundsPlay = 0;
		this->ScoreTRs = 0;
		this->ScoreCTs = 0;
		this->Winner = 0; 
	}

} P_MATCH_DATA, *LP_MATCH_DATA;

// Player stats
typedef struct S_PLAYER_STATS
{
	// Stats
	int Frags;					// BETA: Player Frags
	int Deaths;					// BETA: Player Deaths
	int Assists;				// BETA: Player Kill Assists
	int Headshots;				// BETA: Headshots by player
	int Shots;					// BETA: Shots by player
	int Hits;					// BETA: Hits done by player
	int HitsReceived;			// BETA: Hits received by player
	int Damage;					// BETA: Damage done by player
	int DamageReceived;			// BETA: Damage received by player
	long Money;					// BETA: Money Balance from player
	int BlindFrags;				// BETA: Player frags when blinded by flashbang
	float RoundWinShare;		// BETA: Round Win Share stats

	// Kill streak
	int KillStreak[MAX_CLIENTS + 1];

	// Versus
	int Versus[MAX_CLIENTS + 1];

	// HitBox
	int HitBoxAttack[9][2];		// BETA: Hitbox: 0 Hits, 1 Damage
	int HitBoxVictim[9][2];		// BETA: Hitbox: 0 Hits, 1 Damage

	// Weapon
	int Weapon[MAX_WEAPONS + 1][8]; // BETA: Weapon Stats: 0 Frags, 1 Deaths, 2 Headshots, 3 Shots, 4 Hits, 5 Hits Received, 6 Damage, 7 Damage Received

	// Rounds
	int RoundsPlay;				// BETA: Rounds Play
	int RoundsWin;				// BETA: Rounds Won
	int RoundsLose;				// BETA: Rounds Lose

	// Bomb
	int BombSpawn;				// BETA: Bomb Spawns
	int BombDrop;				// BETA: Bomb Drops
	int BombPlanting;			// BETA: Bomb Plant Attempts
	int BombPlanted;			// BETA: Bomb Plants
	int BombExploded;			// BETA: Bomb Explosions
	int BombDefusing;			// BETA: Bomb Defusing Attempts
	int BombDefusingKit;		// BETA: Bomb Defusing with Kit Attempts
	int BombDefused;			// BETA: Bomb Defuses
	int BombDefusedKit;			// BETA: Bomb Defuses with Kit

	// Sick Frags
	int OneShot;				// TODO: One Shot Frags (Except for AWP)
	int NoScope;				// TODO: No Scope Frags
	int FlyFrags;				// TODO: Flying Frags
	int WallFrags;				// TODO: Wallbganf Frags

	// Entries
	int FirstRoundFrags;		// BETA: First Frag of round
	int FirstRoundDeaths;		// BETA: First Death of round

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
		this->RoundWinShare = 0.0f;

		// Hitbox Attacker
		memset(this->HitBoxAttack, 0, sizeof(this->HitBoxAttack));

		// Hitbox Victim
		memset(this->HitBoxVictim, 0, sizeof(this->HitBoxVictim));

		// Kill Streak
		memset(this->KillStreak, 0, sizeof(this->KillStreak));

		// Versus
		memset(this->Versus, 0, sizeof(this->Versus));

		// Weapon
		memset(this->Weapon, 0, sizeof(this->Weapon));

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

		// Sick Frags
		this->OneShot = 0;
		this->NoScope = 0;
		this->FlyFrags = 0;
		this->WallFrags = 0;

		// Entries
		this->FirstRoundFrags = 0;
		this->FirstRoundDeaths = 0;
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
	int Winner;					// Player is winner of match

	// Stats by match states
	std::array<P_PLAYER_STATS, STATE_END + 1> Stats;

	// Reset Player Stats
	void ResetStats()
	{
		// Reset winner of match
		this->Winner = 0;

		// Reset Stats for each match state
		for (auto& PlayerStats : this->Stats)
		{
			// Reset Data
			PlayerStats.Reset();
		}
	}

	// Totalize Stats
	P_PLAYER_STATS GetStats()
	{
		// Stats Result
		P_PLAYER_STATS Result = { 0 };

		// Loop Array of stats
		for (const auto& row : this->Stats)
		{
			// Stats
			Result.Frags += row.Frags;
			Result.Deaths += row.Deaths;
			Result.Assists += row.Assists;
			Result.Headshots += row.Headshots;
			Result.Shots += row.Shots;
			Result.Hits += row.Hits;
			Result.HitsReceived += row.HitsReceived;
			Result.Damage += row.Damage;
			Result.DamageReceived += row.DamageReceived;
			Result.Money += row.Money;
			Result.BlindFrags += row.BlindFrags;
			Result.RoundWinShare += row.RoundWinShare;

			// Kill Streak, Versus
			for (int i = 0; i < MAX_CLIENTS; i++)
			{
				// Kill Streak
				Result.KillStreak[i] += row.KillStreak[i];

				// Versus
				Result.Versus[i] += row.Versus[i];
			}

			// Hitbox
			for (int i = 0; i < 9; i++)
			{
				// Hitbox as attacker
				Result.HitBoxAttack[i][0] += row.HitBoxAttack[i][0]; // Hits
				Result.HitBoxAttack[i][1] += row.HitBoxAttack[i][1]; // Damage

				// Hitbox as victim
				Result.HitBoxVictim[i][0] += row.HitBoxVictim[i][0]; // Hits
				Result.HitBoxVictim[i][1] += row.HitBoxVictim[i][1]; // Damage
			}

			// Weapon Stats
			for (int i = 0; i < MAX_WEAPONS; i++)
			{
				// Weapon Stats Loop
				for (int j = 0; j < 8; j++)
				{
					Result.Weapon[i][j] += row.Weapon[i][j];
				}
			}

			// Rounds
			Result.RoundsPlay += row.RoundsPlay;
			Result.RoundsWin += row.RoundsWin;
			Result.RoundsLose += row.RoundsLose;

			// Bomb
			Result.BombSpawn += row.BombSpawn;
			Result.BombDrop += row.BombDrop;
			Result.BombPlanting += row.BombPlanting;
			Result.BombPlanted += row.BombPlanted;
			Result.BombExploded += row.BombExploded;
			Result.BombDefusing += row.BombDefusing;
			Result.BombDefusingKit += row.BombDefusingKit;
			Result.BombDefused += row.BombDefused;
			Result.BombDefusedKit += row.BombDefusedKit;
		}

		// Stats
		return Result;
	}
};

// Round Events
typedef struct
{
	int			Round;			// Round Count
	float		Time;			// Round Time Seconds;
	int			Type;			// ROUND_NONE for player events, ScenarioEventEndRound for round events
	int			ScenarioEvent;	// Scenario Event End Round
	int			Winner;			// Winner team of event
	int			Loser;			// Loser team of event
	std::string Killer;			// Killer
	std::string Victim;			// Victim
	Vector		KillerOrigin;	// Killer Position
	Vector		VictimOrigin;	// Victim Position
	int			IsHeadShot;		// HeadShot
	int			ItemIndex;		// Item Index of event
	
} P_ROUND_EVENT, * LP_ROUND_EVENT;

class CMatchStats
{
public:
	// On Server activate
	void ServerActivate();

	// Match State
	void SetState(int State, bool KnifeRound);

	// Save Match Data Task callback
	static void SaveData(int State);

	// Save Data
	void SaveJson();
	
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

	// Round Event
	void OnEvent(GameEventType event, int ScenarioEvent, CBaseEntity* pEntity, class CBaseEntity* pEntityOther);

	// Chat
	void ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1);

	// Show Enemy HP
	bool ShowHP(CBasePlayer* Player, bool Command, bool InConsole);

	// Show Damage
	bool ShowDamage(CBasePlayer* Player, bool Command, bool InConsole);

	// Show Received Damage
	bool ShowReceivedDamage(CBasePlayer* Player, bool Command, bool InConsole);

	// Show Round Summary
	bool ShowSummary(CBasePlayer* Player, bool Command, bool InConsole);

	// Round end Stats
	static void RoundEndStats(int State);

private:
	// Match Data
	P_MATCH_DATA m_Data;

	// Player Data
	std::map<std::string, CMatchPlayerData> m_Player;

	// Round Event Data
	std::vector<P_ROUND_EVENT> m_RoundEvent;

	// Chat Data
	std::vector<std::string> m_ChatLog;

	// Round Stats (Damage and Hits)
	int m_RoundDmg[MAX_CLIENTS + 1][MAX_CLIENTS + 1];
	int m_RoundHit[MAX_CLIENTS + 1][MAX_CLIENTS + 1];

	// Round total damage by player
	int m_RoundDamage[MAX_CLIENTS + 1];

	// Round total damage by team
	int m_RoundDamageTeam[SPECTATOR + 1];

	// Round Frags by player
	int m_RoundFrags[MAX_CLIENTS + 1];

	// Round Versus by player
	int m_RoundVersus[MAX_CLIENTS + 1];

	// Total of Frags in round
	int m_RoundFragCount;

	// Match State
	size_t m_State = STATE_DEAD;

	// Match stats commands
	int m_StatsCommandFlags = CMD_ALL;
};

extern CMatchStats gMatchStats;