#pragma once

// Match Data
typedef struct S_MATCH_DATA
{
	// BETA: Server Name
	std::string HostName;

	// BETA: Map Name
	std::string Map;

	// BETA: Server Address
	std::string Address;

	// BETA: [0] Match Start Time, [1] Match End Time
	std::array<time_t, 2> Time;

	// BETA: Game Mode: 0 Leaders Sorted, 1 Random Teams, 2 Not Sorted, 3 Skill Sorted, 4 Swap Teams, 5 Knife Round
	int GameMode;

	// BETA: Match will have Knife Round
	bool KnifeRound;

	// Reset all data
	void Reset()
	{
		// Reset host name
		this->HostName = "";

		// Reset Map Name
		this->Map = "";

		// Reset Address
		this->Address = "";

		// Reset match time
		this->Time.fill(0U);

		// Reset game mode
		this->GameMode = 0;

		// Match has Knife Round
		this->KnifeRound = 0;

	}
} P_MATCH_DATA, *LP_MATCH_DATA;

// Round Events
typedef struct S_ROUND_EVENT
{
	// BETA: Round Count
	int	Round;

	// BETA: Round Time Seconds;
	float Time;

	// BETA: Game Event (GameEvent.h)
	int	GameEvent;

	// BETA: Scenario Event
	int	ScenarioEvent;

	// BETA: Winner team of event
	int	Winner;

	// BETA: Loser team of event
	int	Loser;

	// BETA: Killer Steam Id
	std::string Killer;

	// BETA: Victim Steam Id
	std::string Victim;

	// BETA: Killer Position
	Vector KillerOrigin;

	// BETA: Victim Position
	Vector VictimOrigin;

	// BETA: HeadShot
	bool IsHeadShot;

	// BETA: Item Index of event
	int	ItemIndex;
} P_ROUND_EVENT, *LP_ROUND_EVENT;

// Weapon Stats
typedef struct S_WEAPON_STATS
{
	int Frags;				// TODO: Player Frags
	int Deaths;				// TODO: Player Deaths
	int Headshots;			// TODO: Headshots
	int Shots;				// TODO: Shots
	int Hits;				// TODO: Hits
	int HitsReceived;		// TODO: Hits Received
	int Damage;				// TODO: Damage Taken
	int DamageReceived;		// TODO: Damage Receeived
} P_WEAPON_STATS, * LP_WEAPON_STATS;

// Round Stats
typedef struct S_ROUND_STATS
{
	int Frags;				// TODO: Player Frags
	int Deaths;				// TODO: Player Deaths
	int Headshots;			// TODO: Headshots
	int Shots;				// TODO: Shots
	int Hits;				// TODO: Hits
	int HitsReceived;		// TODO: Hits Received
	int Damage;				// TODO: Damage Taken
	int DamageReceived;		// TODO: Damage Receeived
	bool BombDefused;		// TODO: Bomb Defused
	bool BombExploded;		// TODO: Bomb Exploded
	int Versus;				// TODO: Player is versus X players
	float KillTime;			// TODO: Last Kill Time

	// Damage dealt to other players
	std::map<std::string, int> PlayerDamage;

	// Reset
	void Reset()
	{
		this->Frags = 0;
		this->Deaths = 0;
		this->Headshots = 0;
		this->Shots = 0;
		this->Hits = 0;
		this->HitsReceived = 0;
		this->Damage = 0;
		this->DamageReceived = 0;
		this->BombDefused = 0;
		this->BombExploded = 0;
		this->Versus = 0;
		this->KillTime = 0.0f ;
	}
} P_ROUND_STATS, *LP_ROUND_STATS;

// Player Stats
typedef struct S_PLAYER_STATS
{
	// Stats
	int Frags;					// TODO: Player Frags
	int Deaths;					// TODO: Player Deaths
	int Assists;				// TODO: Player Kill Assists
	int Shots;					// TODO: Shots by player
	int Hits;					// TODO: Hits done by player
	int HitsReceived;			// TODO: Hits received by player
	int Damage;					// TODO: Damage taken by player
	int DamageReceived;			// TODO: Damage received by player
	long Money;					// TODO: Money Balance from player
	int Suicides;				// TODO: Suicide Count

	// TODO: Round Win Share stats
	float RoundWinShare;

	// Kill Rarity (Sick Stats)
	int Headshots;				// TODO: Headshots by player (KILLRARITY_HEADSHOT)
	int BlindFrags;				// TODO: Killer was blind (KILLRARITY_KILLER_BLIND)
	int BlindDeaths;			// TODO: Player deaths when blinded by flashbang
	int NoScope;				// TODO: No Scope Frags (KILLRARITY_NOSCOPE)
	int WallFrags;				// TODO: Wallbang Frags (KILLRARITY_PENETRATED)
	int SmokeFrags;				// TODO: Frags inside smoke (KILLRARITY_THRUSMOKE)
	int AssistedFlash;			// TODO: Flash Assists (KILLRARITY_ASSISTEDFLASH)
	int Dominations;			// TODO: Killer player began dominating the victim (NOTE: this flag is set once) (KILLRARITY_DOMINATION_BEGAN)
	int DominationRepeats;		// TODO: Continues domination by the killer (KILLRARITY_DOMINATION)
	int Revenges;				// TODO: Revenge by the killer (KILLRARITY_REVENGE)
	int OneShot;				// TODO: One Shot Frags (Except for AWP)
	int FlyFrags;				// TODO: Flying Frags
	int	GodLikes;				// TODO: Count of times when a player killed the other accompanied by the wall
	int DoubleKill;				// TODO: Double Kill
	int Blind;					// BETA: Player was blinded
	int Footsteps;				// BETA: Footstep count
	int Jumps;					// BETA: Jump count
	int LandedFromHeight;		// BETA: Player is fell with some damage
	int WeaponFired;			// BETA: Player is attack
	int WeaponFiredEmpty;		// BETA: Player is attack without clip ammo
	int WeaponReloaded;			// BETA: Player is reloading his weapon

	// TODO: Count of Knife Duels in match (0 Wins, 1 Loses)
	std::array<int, 2> KnifeDuels;

	// Entries
	int EntryFrags;				// TODO: Entry Frag of round
	int EntryDeaths;			// TODO: Entry Death of round

	// Rounds
	int RoundPlay;				// TODO: Rounds Play
	int RoundWin;				// TODO: Rounds Won
	int RoundLose;				// TODO: Rounds Lose

	// Bomb
	int BombSpawn;				// TODO: Bomb Spawns
	int BombDrop;				// TODO: Bomb Drops
	int BombPlanting;			// TODO: Bomb Plant Attempts
	int BombPlanted;			// TODO: Bomb Plants
	int BombExploded;			// TODO: Bomb Explosions
	int BombDefusing;			// TODO: Bomb Defusing Attempts
	int BombDefusingKit;		// TODO: Bomb Defusing with Kit Attempts
	int BombDefused;			// TODO: Bomb Defuses
	int BombDefusedKit;			// TODO: Bomb Defuses with Kit

	// TODO: Kill streak
	std::array<int, MAX_CLIENTS + 1> KillStreak;

	// TODO: Versus: 1 vs X win situations
	std::array<int, MAX_CLIENTS + 1> Versus;

	// TODO: HitBox (0 Hits, 1 Damage, 2 Hits Received, 3 Damage Received)
	std::array<std::array<int, 4>, 9> HitBox;

	// TODO: Weapon Stats
	std::map<int, P_WEAPON_STATS> Weapon;

	// Clear stats
	void Reset()
	{
		// Stats
		this->Frags = 0;
		this->Deaths = 0;
		this->Assists = 0;
		this->Shots = 0;
		this->Hits = 0;
		this->HitsReceived = 0;
		this->Damage = 0;
		this->DamageReceived = 0;
		this->Money = 0;
		this->Suicides = 0;

		// Round Win Share stats
		this->RoundWinShare = 0.0f;

		// Kill Rarity
		this->Headshots = 0;
		this->BlindFrags = 0;
		this->BlindDeaths = 0;
		this->NoScope = 0;
		this->WallFrags = 0;
		this->SmokeFrags = 0;
		this->AssistedFlash = 0;
		this->Dominations = 0;
		this->DominationRepeats = 0;
		this->Revenges = 0;
		this->OneShot = 0;
		this->FlyFrags = 0;
		this->GodLikes = 0;
		this->DoubleKill = 0;
		this->Blind = 0;
		this->Footsteps = 0;
		this->Jumps = 0;
		this->LandedFromHeight = 0;
		this->WeaponFired = 0;
		this->WeaponFiredEmpty = 0;
		this->WeaponReloaded = 0;

		// Entries
		this->EntryFrags = 0;
		this->EntryDeaths = 0;

		// Kill streak
		this->KillStreak.fill(0);

		// Versus: 1 vs X win situations
		this->Versus.fill(0);

		// HitBox (0 Hits, 1 Damage, 1 Hits Received, 3 Damage Received)
		this->HitBox.fill({ 0 });

		// Weapon Stats
		this->Weapon.clear();

		// Rounds
		this->RoundPlay = 0;
		this->RoundWin = 0;
		this->RoundLose = 0;

		// Bomb
		this->BombSpawn = 0;
		this->BombDrop = 0;
		this->BombPlanting = 0;
		this->BombPlanted = 0;
		this->BombExploded = 0;
		this->BombDefusing = 0;
		this->BombDefusingKit = 0;
		this->BombDefused = 0;
		this->BombDefusedKit = 0;
	}
} P_PLAYER_STATS, *LP_PLAYER_STATS;

// Player Chat
typedef struct S_PLAYER_CHAT
{
	// TODO: Global Time
	time_t Time;

	// TODO: Match State
	int State;

	// TODO: Player Team
	int Team;

	// TODO: Alive
	int Alive;

	// TODO: Message
	std::string Message;
} P_PLAYER_CHAT, *LP_PLAYER_CHAT;

// Player Data
typedef struct S_PLAYER_DATA
{
	// BETA: Joined Game Time
	time_t JoinGameTime;	

	// BETA: Disconnected Time
	time_t DisconnectTime;		

	// BETA: Player Name
	std::string Name;

	// BETA: Player Team
	int Team;

	// BETA: Is Winner of match
	int	Winner;

	// TODO: Player Stats
	std::map<int, P_PLAYER_STATS> Stats;

	// TODO: Player chat log
	std::vector<P_PLAYER_CHAT> Chat;

	// TODO: Round Stats
	P_ROUND_STATS Round;
} P_PLAYER_DATA, * LP_PLAYER_DATA;

class CMatchStats
{
public:
	// On match bot new state
	void SetState(int State, bool KnifeRound);

	// On player enter in game
	void PlayerGetIntoGame(CBasePlayer* Player);

	// On player disconnect
	void PlayerDisconnect(edict_t* pEntity);

	// On player switch team
	void PlayerSwitchTeam(CBasePlayer* Player);

	// On Round Restart
	void RoundRestart();

	// On send death message
	void SendDeathMessage(CBaseEntity* Killer, CBasePlayer* Victim, CBasePlayer* Assister, entvars_t* pevInflictor, const char* killerWeaponName, int iDeathMessageFlags, int iRarityOfKill);

	// On BOT manager event
	void CBotManager_OnEvent(GameEventType GameEvent, CBaseEntity* pEntity, CBaseEntity* pOther);

	// On Round event
	void RoundEvent(GameEventType GameEvent, CBasePlayer* Killer, CBasePlayer* Victim);
	
private:
	// Match State
	int m_State = STATE_DEAD;

	// BETA: Match Data
	P_MATCH_DATA m_Match;

	// BETA: Round Events
	std::vector<P_ROUND_EVENT> m_Event;

	// TODO: Player Data
	std::map<std::string, P_PLAYER_DATA> m_Player;
};

extern CMatchStats gMatchStats;