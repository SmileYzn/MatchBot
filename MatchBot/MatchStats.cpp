#include "precompiled.h"

CMatchStats gMatchStats;

void CMatchStats::ServerActivate()
{
	// Make Directory to make sure that exists
	gMatchUtil.MakeDirectory(STATS_SAVE_PATH);

	// Reset Player Data
	this->m_Player.clear();

	// Reset Match Data
	this->m_Data.Reset();

	// Clear events data
	this->m_RoundEvent.clear();

	// Clear chat history
	this->m_Chat.clear();

	// Match State
	this->m_State = STATE_DEAD;

	// Match stats commands
	this->m_StatsCommandFlags = CMD_ALL;

	// Register SayText message
	gMatchMessage.RegisterHook("SayText", this->SayText);
}

// Get Players
std::map<std::string, CMatchPlayerData> CMatchStats::GetPlayers()
{
	return this->m_Player;
}

// Change States
void CMatchStats::SetState(int State, bool KnifeRound)
{
	// New Match State
	this->m_State = State;

	// All commands are enabled by default
	this->m_StatsCommandFlags = CMD_ALL;

	// If variable is not null
	if (gMatchBot.m_StatsCommands)
	{
		// If string is not null
		if (gMatchBot.m_StatsCommands->string)
		{
			// If string is not empty
			if (gMatchBot.m_StatsCommands->string[0] != '\0')
			{
				// Read menu flags from team pickup variable
				this->m_StatsCommandFlags |= gMatchAdmin.ReadFlags(gMatchBot.m_StatsCommands->string);
			}
		}
	}

	// Reset all stats data for this match
	if (State == STATE_START)
	{
		// Reset Match Data
		this->m_Data.Reset();

		// Clear events data
		this->m_RoundEvent.clear();

		// Clear chat history
		this->m_Chat.clear();

		// Resert Player Match Data
		for (auto & Player : this->m_Player)
		{
			// Reset Player Stats
			Player.second.ResetStats();
		}
	}
	// Store match data on First Half
	else if(State == STATE_FIRST_HALF)
	{
		// Start Time
		this->m_Data.StartTime = time(0);

		// Max Rounds
		this->m_Data.MaxRounds = (int)gMatchBot.m_PlayRounds->value;

		// Max OT Rounds
		this->m_Data.MaxRoundsOT = (int)gMatchBot.m_PlayRoundsOT->value;

		// Hostname
		this->m_Data.HostName = g_engfuncs.pfnCVarGetString("hostname");

		// Map
		this->m_Data.Map = STRING(gpGlobals->mapname);

		// Server address
		this->m_Data.Address = g_engfuncs.pfnCVarGetString("net_address");

		// Get Game Mode
		this->m_Data.GameMode = gMatchVoteTeam.GetMode();

		// Has Knife Round
		this->m_Data.KnifeRound = KnifeRound;

		// Winner of match
		this->m_Data.Winner = 0;
	}
	// Store end time if is end
	else if(State == STATE_END)
	{
		// End Time
		this->m_Data.EndTime = time(0);

		// Rounds Played
		this->m_Data.RoundsPlay = gMatchBot.GetRound();

		// Terrorists Score
		this->m_Data.ScoreTRs = gMatchBot.GetScore(TERRORIST);

		// CTs Score
		this->m_Data.ScoreCTs = gMatchBot.GetScore(CT);

		// Winner of match
		this->m_Data.Winner = (this->m_Data.ScoreTRs != this->m_Data.ScoreCTs) ? (this->m_Data.ScoreTRs > this->m_Data.ScoreCTs ? 1 : 2) : 0;

		// Get players
		auto Players = gMatchUtil.GetPlayers(true, true);

		// Loop players
		for (auto Player : Players)
		{
			// Get player auth
			auto Auth = GET_USER_AUTH(Player->edict());

			// If is not null
			if (Auth)
			{
				// If Auth Index is not null
				if (Auth[0] != '\0')
				{
					// Set Team
					this->m_Player[Auth].Team = Player->m_iTeam;

					// If Player is in winner team
					if (Player->m_iTeam == (TeamName)this->m_Data.Winner)
					{
						// Set winner
						this->m_Player[Auth].Winner = 1;
					}
				}
			}
		}

		// Save Data
		gMatchTask.Create(TASK_SAVE_STATS, 2.0f, false, (void*)this->SaveData, STATE_END);
	}
}

// Save Match Data Task
void CMatchStats::SaveData(int State)
{
	// Save JSON data
	gMatchStats.SaveJson();
}

// Save Match Data
void CMatchStats::SaveJson()
{
	// Save data
	nlohmann::ordered_json Data;

	// Server Data
	Data["server"] =
	{
		{"StartTime", this->m_Data.StartTime},
		{"EndTime", this->m_Data.EndTime},
		{"MaxRounds", this->m_Data.MaxRounds},
		{"MaxRoundsOT", this->m_Data.MaxRoundsOT},
		{"HostName", this->m_Data.HostName},
		{"Map", this->m_Data.Map},
		{"Address", this->m_Data.Address},
		{"GameMode", this->m_Data.GameMode},
		{"KnifeRound", this->m_Data.KnifeRound},
		{"RoundsPlay", this->m_Data.RoundsPlay},
		{"ScoreTRs", this->m_Data.ScoreTRs},
		{"ScoreCTs", this->m_Data.ScoreCTs},
		{"Winner", this->m_Data.Winner},
	};

	// Player Data
	for (auto const& Player : this->m_Player)
	{
		// Auth Index
		const char* Auth = Player.first.c_str();

		// If has Auth Index
		if (Auth)
		{
			// If Auth Index is not null
			if (Auth[0] != '\0')
			{
				// Get final stats
				auto Stats = this->m_Player[Auth].GetStats();
				//
				// Insert Final Stats on JSON
				Data["stats"][Auth] =
				{
					// Player Information
					{"ConnectTime", Player.second.ConnectTime},
					{"JoinTeamTime", Player.second.JoinTeamTime},
					{"GetIntoGameTime", Player.second.GetIntoGameTime},
					{"DisconnectedTime", Player.second.DisconnectedTime},
					{"Team", Player.second.Team},
					{"Winner", Player.second.Winner},
					{"UserIndex", Player.second.UserIndex},
					{"Name", Player.second.Name},
					//
					// Player Stats
					{"Frags", Stats.Frags},
					{"Deaths", Stats.Deaths},
					{"Assists", Stats.Assists},
					{"Headshots", Stats.Headshots},
					{"Shots", Stats.Shots},
					{"Hits", Stats.Hits},
					{"HitsReceived", Stats.HitsReceived},
					{"Damage", Stats.Damage},
					{"DamageReceived", Stats.DamageReceived},
					{"Money", Stats.Money},
					{"BlindFrags", Stats.BlindFrags},
					{"RoundWinShare", Stats.RoundWinShare},
					//
					// Round Stats
					{"RoundsPlay", Stats.RoundsPlay},
					{"RoundsWin", Stats.RoundsWin},
					{"RoundsLose", Stats.RoundsLose},
					//
					// Bomb Stats
					{"BombSpawn", Stats.BombSpawn},
					{"BombDrop", Stats.BombDrop},
					{"BombPlanting", Stats.BombPlanting},
					{"BombPlanted", Stats.BombPlanted},
					{"BombExploded", Stats.BombExploded},
					{"BombDefusing", Stats.BombDefusing},
					{"BombDefusingKit", Stats.BombDefusingKit},
					{"BombDefused", Stats.BombDefused},
					{"BombDefusedKit", Stats.BombDefusedKit},
					//
					// Hitbox stats
					{"HitBoxAttack", Stats.HitBoxAttack},
					{"HitBoxVictim", Stats.HitBoxVictim},
					//
					// Kill Streaks
					{"KillStreak", Stats.KillStreak},
					//
					// Round Versus
					{"Versus", Stats.Versus},
					//
					// Weapon Stats
					{"Weapon", Stats.Weapon},
					//
					// Sick Frags
					{"OneShot", Stats.OneShot},
					{"NoScope", Stats.NoScope},
					{"FlyFrags", Stats.FlyFrags},
					{"WallFrags", Stats.WallFrags},
					//
					// Entries
					{"FirstRoundFrags", Stats.FirstRoundFrags},
					{"FirstRoundDeaths", Stats.FirstRoundDeaths},
				};
			}
		}
	}

	// Round Events
	for (auto const& Event : this->m_RoundEvent)
	{
		// Insert event
		Data["events"][std::to_string(Event.Round)].push_back
		({
			{"Round",Event.Round},
			{"Time",Event.Time},
			{"Type",Event.Type},
			{"ScenarioEvent", Event.ScenarioEvent},
			{"Winner",Event.Winner},
			{"Loser",Event.Loser},
			{"Killer",Event.Killer.c_str()},
			{"KillerOrigin",{Event.KillerOrigin[0],Event.KillerOrigin[1],Event.KillerOrigin[2]}},
			{"Victim",Event.Victim.c_str()},
			{"VictimOrigin",{Event.VictimOrigin[0],Event.VictimOrigin[1],Event.VictimOrigin[2]}},
			{"IsHeadShot",Event.IsHeadShot},
			{"ItemIndex",Event.ItemIndex},
		});
	}

	// Chat History
	if (!this->m_Chat.empty())
	{
		// For each line
		for (auto const& Chat : this->m_Chat)
		{
			// Push line
			Data["chat"].push_back
			({
				{"Size",Chat.Size},
				{"Auth",Chat.Auth},
				{"Time",Chat.Time},
				{"Team",Chat.Team},
				{"Alive",Chat.Alive},
				{"Message",Chat.Message}
			});
		}
	}

	// Report System
	auto ReportData = gMatchReport.GetData();

	if (!ReportData.empty())
	{
		for (auto const& Report : ReportData)
		{
			// Insert report data
			Data["report"].push_back
			({
				{"Player",Report.Player},
				{"PlayerTeam",Report.PlayerTeam},
				{"Target",Report.Target},
				{"TargetTeam",Report.TargetTeam},
				{"Reason",Report.Reason},
				{"MatchState",Report.MatchState},
				{"Time",Report.Time},
				{"MatchState",Report.MatchState}
			});
		}
	}

	// Store Data
	if (Data.size())
	{
		// File path buffer
		char Buffer[MAX_PATH] = { 0 };

		char DateTime[24] = { 0 };

		strftime(DateTime, sizeof(DateTime), "%Y%m%d_%H%M%S", localtime(&this->m_Data.EndTime));
		
		// Format Path with match end time
		Q_snprintf(Buffer, sizeof(Buffer), "%s/%s.json", STATS_SAVE_PATH, DateTime);

		// Create file with path buffer
		std::ofstream DataFile(Buffer);

		// Put Stats data to file buffer
		DataFile << Data;

		// Close file
		DataFile.close();
	}
}

// On Player Connect
bool CMatchStats::PlayerConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	auto Auth = GET_USER_AUTH(pEntity);

	if (Auth)
	{
		// Set Connection time
		this->m_Player[Auth].ConnectTime = time(0);

		// User Index
		this->m_Player[Auth].UserIndex = g_engfuncs.pfnGetPlayerUserId(pEntity);

		// If name is not null
		if (pszName)
		{
			// Set Name
			this->m_Player[Auth].Name = pszName;
		}
	}

	return true;
}

// When player try to join in a team, we do things
void CMatchStats::PlayerJoinTeam(CBasePlayer* Player, int Slot)
{
	auto Auth = GET_USER_AUTH(Player->edict());

	if (Auth)
	{
		// Update team when player change
		this->m_Player[Auth].Team = Player->m_iTeam;

		// Joined Team Time
		this->m_Player[Auth].JoinTeamTime = time(0);

		// User Index
		this->m_Player[Auth].UserIndex = g_engfuncs.pfnGetPlayerUserId(Player->edict());

		// If name is not null
		if (STRING(Player->edict()->v.netname))
		{
			// Update Name
			this->m_Player[Auth].Name = STRING(Player->edict()->v.netname);
		}
	}
}

// While player get into game (Enter in any TERRORIST or CT Team)
void CMatchStats::PlayerGetIntoGame(CBasePlayer* Player)
{
	auto Auth = GET_USER_AUTH(Player->edict());

	if (Auth)
	{
		// Get into game time
		this->m_Player[Auth].GetIntoGameTime = time(0);

		// Update new team
		this->m_Player[Auth].Team = Player->m_iTeam;

		// User Index
		this->m_Player[Auth].UserIndex = g_engfuncs.pfnGetPlayerUserId(Player->edict());

		// If name is not null
		if (STRING(Player->edict()->v.netname))
		{
			// Update Name
			this->m_Player[Auth].Name = STRING(Player->edict()->v.netname);
		}
	}
}

// When player disconnect from game
void CMatchStats::PlayerDisconnect(edict_t* pEdict)
{
	// Get user auth index
	auto Auth = GET_USER_AUTH(pEdict);

	// If is not null
	if (Auth)
	{
		// Save disconnection time
		this->m_Player[Auth].DisconnectedTime = time(0);
	}
}

// Round Restart
void CMatchStats::RoundRestart(bool PreRestart)
{
	// If match is live
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		// If has ReGameDLL_CS Api
		if (g_pGameRules)
		{
			// If is complete reset
			if (CSGameRules()->m_bCompleteReset)
			{
				// Loop all saved players
				for (auto & row : this->m_Player)
				{
					// Reset Player Stats of state
					row.second.Stats[this->m_State].Reset();
				}
			}
		}
	}
}

// Round Start
void CMatchStats::RoundStart()
{
	// If match is live
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		// Clear Round Damage
		memset(this->m_RoundDmg, 0, sizeof(this->m_RoundDmg));

		// Clear Round Hits
		memset(this->m_RoundHit, 0, sizeof(this->m_RoundHit));

		// Clear total round damage
		memset(this->m_RoundDamage, 0, sizeof(this->m_RoundDamage));

		// Clear total round damage by team
		memset(this->m_RoundDamageTeam, 0, sizeof(this->m_RoundDamageTeam));

		// Clear round frags
		memset(this->m_RoundFrags, 0, sizeof(this->m_RoundFrags));

		// Clear round versus
		memset(this->m_RoundVersus, 0, sizeof(this->m_RoundVersus));

		// Clear round frag count
		this->m_RoundFragCount = 0;
	}
}

// Round End
void CMatchStats::RoundEnd(int winStatus, ScenarioEventEndRound eventScenario, float tmDelay)
{
	// If match is live
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		// If has round winner
		if (winStatus == WINSTATUS_TERRORISTS || winStatus == WINSTATUS_CTS)
		{
			// Set winner as TERRORIST or CT
			auto Winner = winStatus == WINSTATUS_TERRORISTS ? TERRORIST : CT;

			// Get players
			auto Players = gMatchUtil.GetPlayers(true, true);

			// Loop players
			for (auto Player : Players)
			{
				// Get player auth
				auto Auth = GET_USER_AUTH(Player->edict());

				// If is not null
				if (Auth)
				{
					// Increment rounds played
					this->m_Player[Auth].Stats[this->m_State].RoundsPlay++;

					// Entity Index
					auto EntityIndex = Player->entindex();

					// If the player team is winner team
					if (Player->m_iTeam == Winner)
					{
						// Rounds Win
						this->m_Player[Auth].Stats[this->m_State].RoundsWin++;

						// If player has done some damage on round
						if (this->m_RoundDamage[EntityIndex] > 0)
						{
							// Set as initial round win share
							float RoundWinShare = this->m_RoundDamage[EntityIndex];

							// If is more than zero
							if (this->m_RoundDamage[EntityIndex] > 0)
							{
								// Calculate based on round team damage
								RoundWinShare = (RoundWinShare / this->m_RoundDamageTeam[Winner]);
							}
							
							// If round has won by any of bomb objective (Explode or defuse)
							if (CSGameRules()->m_bBombDefused || CSGameRules()->m_bTargetBombed)
							{
								// Increment round win share based on map objective target
								RoundWinShare = (MANAGER_RWS_MAP_TARGET * RoundWinShare);
							}

							// Increment round win share on player stats
							this->m_Player[Auth].Stats[this->m_State].RoundWinShare += RoundWinShare;
						}

						// If has frags on that round
						if (this->m_RoundFrags[EntityIndex] > 0)
						{
							// Increment kill streaks
							this->m_Player[Auth].Stats[this->m_State].KillStreak[this->m_RoundFrags[EntityIndex]]++;
						}

						// Check for player versus stats
						if (this->m_RoundVersus[EntityIndex] > 0)
						{
							// Increment versus stats
							this->m_Player[Auth].Stats[this->m_State].Versus[this->m_RoundVersus[EntityIndex]]++;
						}
					}
					else
					{
						// Increment rounds lose
						this->m_Player[Auth].Stats[this->m_State].RoundsLose++;
					}
				}
			}

			// Round End Event
			this->OnEvent((winStatus == WINSTATUS_TERRORISTS) ? EVENT_TERRORISTS_WIN : EVENT_CTS_WIN, (int)(eventScenario), nullptr, nullptr);

			// Round End Stats
			gMatchTask.Create(TASK_ROUND_END_STATS, 1.0f, false, (void*)this->RoundEndStats, this->m_State);
		}
	}
}

// Player Damage Event
void CMatchStats::PlayerDamage(CBasePlayer* Victim, entvars_t* pevInflictor, entvars_t* pevAttacker, float& flDamage, int bitsDamageType)
{
	// If match is live
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		// If victim is not killed by bomb
		if (!Victim->m_bKilledByBomb)
		{
			// Get attacker
			auto Attacker = UTIL_PlayerByIndexSafe(ENTINDEX(pevAttacker));

			// If is not null
			if (Attacker)
			{
				// If attacker is not victim
				if (Victim->entindex() != Attacker->entindex())
				{
					// If both are players
					if (Victim->IsPlayer() && Attacker->IsPlayer())
					{
						// If victim can receive damage form attacker
						if (CSGameRules()->FPlayerCanTakeDamage(Victim, Attacker))
						{
							// Victim Auth
							auto VictimAuth = GET_USER_AUTH(Victim->edict());

							// Attacker Auth
							auto AttackerAuth = GET_USER_AUTH(Attacker->edict());

							// Damage Taken
							auto DamageTaken = (int)(Victim->m_iLastClientHealth - clamp(Victim->edict()->v.health, 0.0f, Victim->edict()->v.health));

							// Item Index
							auto ItemIndex = (bitsDamageType & DMG_EXPLOSION) ? WEAPON_HEGRENADE : ((Attacker->m_pActiveItem) ? Attacker->m_pActiveItem->m_iId : WEAPON_NONE);

							// Attacker Damage
							this->m_Player[AttackerAuth].Stats[this->m_State].Damage += DamageTaken;

							// Attacker Hits
							this->m_Player[AttackerAuth].Stats[this->m_State].Hits++;

							// Attacker HitBox Hits
							this->m_Player[AttackerAuth].Stats[this->m_State].HitBoxAttack[Victim->m_LastHitGroup][0]++;

							// Attacker HitBox Damage
							this->m_Player[AttackerAuth].Stats[this->m_State].HitBoxAttack[Victim->m_LastHitGroup][1] += DamageTaken;

							// Attacker Hits
							this->m_Player[AttackerAuth].Stats[this->m_State].Weapon[ItemIndex][4]++;

							// Attacker Damage
							this->m_Player[AttackerAuth].Stats[this->m_State].Weapon[ItemIndex][7] += DamageTaken;

							// Victim Damage Received
							this->m_Player[VictimAuth].Stats[this->m_State].DamageReceived += DamageTaken;

							// Victim Hits Received
							this->m_Player[VictimAuth].Stats[this->m_State].HitsReceived++;

							// Victim HitBox Hits
							this->m_Player[VictimAuth].Stats[this->m_State].HitBoxVictim[Victim->m_LastHitGroup][0]++;

							// Victim HitBox Damage
							this->m_Player[VictimAuth].Stats[this->m_State].HitBoxVictim[Victim->m_LastHitGroup][1] += DamageTaken;

							// Victim Hits Received
							this->m_Player[VictimAuth].Stats[this->m_State].Weapon[ItemIndex][5]++;

							// Victim Damage
							this->m_Player[VictimAuth].Stats[this->m_State].Weapon[ItemIndex][6] += DamageTaken;

							// Attacker entity index
							auto AttackerIndex = Attacker->entindex();

							// Victim entity index
							auto VictimIndex = Victim->entindex();

							// Attacker Round Damage
							this->m_RoundDmg[AttackerIndex][VictimIndex] += DamageTaken;

							// Attacker Round Hits
							this->m_RoundHit[AttackerIndex][VictimIndex]++;

							// Total Round Damage
							this->m_RoundDamage[AttackerIndex] += DamageTaken;

							// Total Round Damage by team
							this->m_RoundDamageTeam[Attacker->m_iTeam] += DamageTaken;
						}
					}
				}
			}
		}
	}
}

// Player Killed Event
void CMatchStats::PlayerKilled(CBasePlayer* Victim, entvars_t* pevKiller, entvars_t* pevInflictor)
{
	// If match is live
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		// If victim is not killed by bomb
		if (!Victim->m_bKilledByBomb)
		{
			// Get attacker
			auto Attacker = UTIL_PlayerByIndexSafe(ENTINDEX(pevKiller));

			// If attacker is found
			if (Attacker)
			{
				// Attacker entity index
				auto AttackerIndex = Attacker->entindex();

				// Victim entity index
				auto VictimIndex = Victim->entindex();

				// If attacker is not victim
				if (VictimIndex != AttackerIndex)
				{
					// If both are players
					if (Victim->IsPlayer() && Attacker->IsPlayer())
					{
						// Victim Auth
						auto VictimAuth = GET_USER_AUTH(Victim->edict());

						// Attacker Auth
						auto AttackerAuth = GET_USER_AUTH(Attacker->edict());

						// Item Index
						auto ItemIndex = (Victim->m_bKilledByGrenade) ? WEAPON_HEGRENADE : ((Attacker->m_pActiveItem) ? Attacker->m_pActiveItem->m_iId : WEAPON_NONE);

						// Frags
						this->m_Player[AttackerAuth].Stats[this->m_State].Frags++;

						// Weapon Frags
						this->m_Player[AttackerAuth].Stats[this->m_State].Weapon[ItemIndex][0]++;

						// If attacker is blind
						if (Attacker->IsBlind())
						{
							// If victim not killed by HE
							if (!Victim->m_bKilledByGrenade)
							{
								// Increment frags
								this->m_Player[AttackerAuth].Stats[this->m_State].BlindFrags++;
							}
						}

						// Deaths
						this->m_Player[VictimAuth].Stats[this->m_State].Deaths++;

						// Weapon Deaths
						this->m_Player[VictimAuth].Stats[this->m_State].Weapon[ItemIndex][1]++;

						// If last hit is in head
						if (Victim->m_LastHitGroup == 1)
						{
							// Headshots
							this->m_Player[AttackerAuth].Stats[this->m_State].Headshots++;

							// Weapon Headshots
							this->m_Player[AttackerAuth].Stats[this->m_State].Weapon[ItemIndex][2]++;
						}

						// Round Frags
						this->m_RoundFrags[AttackerIndex]++;

						// If Round Frag Count is empty
						if (!this->m_RoundFragCount)
						{
							// Increment First Round Frags
							this->m_Player[AttackerAuth].Stats[this->m_State].FirstRoundFrags++;

							// Increment First Round Deaths
							this->m_Player[VictimAuth].Stats[this->m_State].FirstRoundDeaths++;
						}

						// Increment Round Frag Count
						this->m_RoundFragCount++;

						// If item is not AWP
						if (ItemIndex != WEAPON_AWP)
						{
							// If last victim health is 100
							if (Victim->m_iLastClientHealth >= 100)
							{
								// If last damage amount is 100
								if (Attacker->m_lastDamageAmount >= 100)
								{
									// Increment one shot
									this->m_Player[AttackerAuth].Stats[this->m_State].OneShot++;
								}
							}
						}

						// If is AWP, Scout, G3SG1 or SG550
						if (ItemIndex == WEAPON_AWP || ItemIndex == WEAPON_SCOUT || ItemIndex == WEAPON_G3SG1 || ItemIndex == WEAPON_SG550)
						{
							// If player is in default fov
							if (Attacker->m_iClientFOV == DEFAULT_FOV)
							{
								// Increment no-scope frags
								this->m_Player[AttackerAuth].Stats[this->m_State].NoScope++;
							}
						}

						// If is not HE Grenade
						if (ItemIndex != WEAPON_HEGRENADE)
						{
							// If attacker is not seeing victim
							if (!Attacker->m_izSBarState[SBAR_ID_TARGETTYPE])
							{
								// If is not in observer mode
								if (!Attacker->IsObserver())
								{
									// Increment wallbang frags
									this->m_Player[AttackerAuth].Stats[this->m_State].WallFrags++;
								}
							}
						}

						// If victim is not on ground
						if (!(Victim->edict()->v.flags & FL_ONGROUND))
						{
							// If player is falling
							if (Victim->m_flFallVelocity > 0.0f)
							{
								// Increment flying frags
								this->m_Player[AttackerAuth].Stats[this->m_State].FlyFrags++;
							}
						}

						// If has ReGameDLL_CS Api
						if (g_pGameRules)
						{
							// Player Count
							int NumAliveTR = 0, NumAliveCT = 0, NumDeadTR = 0, NumDeadCT = 0;

							// Count Players in teams dead and alive
							CSGameRules()->InitializePlayerCounts(NumAliveTR, NumAliveCT, NumDeadTR, NumDeadCT);

							// Loop Max Clients
							for (int i = 1; i <= gpGlobals->maxClients; ++i)
							{
								// Get Temporary Player Data
								auto Temp = UTIL_PlayerByIndexSafe(i);

								// If is not null
								if (Temp)
								{
									// If Player is in any team
									if (Temp->m_iTeam == TERRORIST || Temp->m_iTeam == CT)
									{
										// Get entity index
										auto TempIndex = Temp->entindex();

										// If Temporary player is not attacker
										if (TempIndex != AttackerIndex)
										{
											// Get auth index
											auto TempAuth = GET_USER_AUTH(Temp->edict());

											// If is not null
											if (TempAuth)
											{
												// Temporary Entity Index
												auto TempEntityIndex = Temp->entindex();

												// If is not empty
												if (TempAuth[0] != '\0')
												{
													// Check kill assistence
													if (this->m_RoundDmg[TempEntityIndex][VictimIndex] >= (int)MANAGER_ASSISTANCE_DMG)
													{
														// Increment assistence count
														this->m_Player[TempAuth].Stats[this->m_State].Assists++;
													}

													// If has not round versus set in this round
													if (this->m_RoundVersus[TempEntityIndex] <= 0)
													{
														// If temporary player is Terrorist
														if (Temp->m_iTeam == TERRORIST)
														{
															// If has one Terrorist alive (Temporary player is last player alive)
															if (NumAliveTR == 1)
															{
																// Set amount of players verus temporary players is alive
																this->m_RoundVersus[TempEntityIndex] = NumAliveCT;
															}
														}
														// If temporary player is CT
														else if (Temp->m_iTeam == CT)
														{
															// If has one CT alive (Temporary player is last player alive)
															if (NumAliveCT == 1)
															{
																// Set amount of players verus temporary players is alive
																this->m_RoundVersus[TempEntityIndex] = NumAliveTR;
															}
														}
													}
												}
											}
										}
									}
								}
							}

						}
						//
						// Player died event
						this->OnEvent(EVENT_PLAYER_DIED, ROUND_NONE, Victim, Attacker);
					}
				}
			}
		}
	}
}

// Set Animation
void CMatchStats::PlayerSetAnimation(CBasePlayer* Player, PLAYER_ANIM playerAnim)
{
	// If match is live
	if(this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		// If is attack1 or attack2 commands
		if ((playerAnim == PLAYER_ATTACK1) || (playerAnim == PLAYER_ATTACK2))
		{
			// IF player has an active item on hand
			if (Player->m_pActiveItem)
			{
				// If that item has a index
				if (Player->m_pActiveItem->m_iId != WEAPON_NONE)
				{
					// If is not HE Grenade, C4, Smoke Grenade, Flashbang or Shield
					if (Player->m_pActiveItem->m_iId != WEAPON_HEGRENADE && Player->m_pActiveItem->m_iId != WEAPON_C4 && Player->m_pActiveItem->m_iId != WEAPON_SMOKEGRENADE && Player->m_pActiveItem->m_iId != WEAPON_FLASHBANG && Player->m_pActiveItem->m_iId != WEAPON_SHIELDGUN)
					{
						// Get playter auth index
						auto Auth = GET_USER_AUTH(Player->edict());

						// if is not null
						if (Auth)
						{
							// Increment total shots
							this->m_Player[Auth].Stats[this->m_State].Shots++;

							// Increment Shots
							this->m_Player[Auth].Stats[this->m_State].Weapon[Player->m_pActiveItem->m_iId][3]++;
						}
					}
				}
			}
		}
	}
}

// Player Add Account Event
void CMatchStats::PlayerAddAccount(CBasePlayer* Player, int amount, RewardType type, bool bTrackChange)
{
	// If match is live
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		// If is money comes from anything
		if (type != RT_NONE)
		{
			// Get player auth index
			auto Auth = GET_USER_AUTH(Player->edict());

			// If is not null
			if (Auth)
			{
				// Increment amounbt owned by player
				this->m_Player[Auth].Stats[this->m_State].Money += amount;
			}
		}
	}
}

// Make Bomber
void CMatchStats::PlayerMakeBomber(CBasePlayer* Player)
{
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		if (Player->m_bHasC4)
		{
			auto Auth = GET_USER_AUTH(Player->edict());

			if (Auth)
			{
				this->m_Player[Auth].Stats[this->m_State].BombSpawn++;
			}
		}
	}
}

// Bomb Drop
void CMatchStats::PlayerDropItem(CBasePlayer* Player, const char* pszItemName)
{
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		if (pszItemName)
		{
			if (!Player->IsAlive())
			{
				if (Q_strcmp(pszItemName, "weapon_c4") == 0)
				{
					auto Auth = GET_USER_AUTH(Player->edict());

					if (Auth)
					{
						this->m_Player[Auth].Stats[this->m_State].BombDrop++;
					}
				}
			}
		}
	}
}

// Bomb Plant
void CMatchStats::PlantBomb(entvars_t* pevOwner, bool Planted)
{
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		auto Player = UTIL_PlayerByIndexSafe(ENTINDEX(pevOwner));

		if (Player)
		{
			auto Auth = GET_USER_AUTH(Player->edict());

			if (Auth)
			{
				if (Planted)
				{
					this->m_Player[Auth].Stats[this->m_State].BombPlanted++;

					// Bomb Planted Event
					this->OnEvent(EVENT_BOMB_PLANTED, ROUND_NONE, Player, nullptr);
				}
				else
				{
					this->m_Player[Auth].Stats[this->m_State].BombPlanting++;
				}
			}
		}
	}
}

// Defuse Bomb Start
void CMatchStats::DefuseBombStart(CBasePlayer* Player)
{
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		auto Auth = GET_USER_AUTH(Player->edict());

		if (Auth)
		{
			this->m_Player[Auth].Stats[this->m_State].BombDefusing++;

			if (Player->m_bHasDefuser)
			{
				this->m_Player[Auth].Stats[this->m_State].BombDefusingKit++;
			}
		}
	}
}

// Defuse Bomb End
void CMatchStats::DefuseBombEnd(CBasePlayer* Player, bool Defused)
{
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		auto Auth = GET_USER_AUTH(Player->edict());

		if (Auth)
		{
			if (Defused)
			{
				this->m_Player[Auth].Stats[this->m_State].BombDefused++;

				if (Player->m_bHasDefuser)
				{
					this->m_Player[Auth].Stats[this->m_State].BombDefusedKit++;
				}

				// Incremet round win share by bomb defusion
				this->m_Player[Auth].Stats[this->m_State].RoundWinShare += MANAGER_RWS_C4_DEFUSED;

				// Bomb Defused Event
				this->OnEvent(EVENT_BOMB_DEFUSED, ROUND_NONE, Player, nullptr);
			}
		}
	}
}

// Explode Bomb
void CMatchStats::ExplodeBomb(CGrenade* pThis, TraceResult* ptr, int bitsDamageType)
{
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		if (pThis->m_bIsC4)
		{
			if (pThis->pev->owner)
			{
				auto Player = UTIL_PlayerByIndexSafe(ENTINDEX(pThis->pev->owner));

				if (Player)
				{
					auto Auth = GET_USER_AUTH(Player->edict());

					if (Auth)
					{
						// Increment Bomb Exploded Stats
						this->m_Player[Auth].Stats[this->m_State].BombExploded++;

						// Incremet round win share by bomb explosion
						this->m_Player[Auth].Stats[this->m_State].RoundWinShare += MANAGER_RWS_C4_EXPLODE;

						// Bomb Exploded event
						this->OnEvent(EVENT_BOMB_EXPLODED, ROUND_NONE, Player, nullptr);
					}
				}
			}
		}
	}
}

// On Round Event Data
void CMatchStats::OnEvent(GameEventType event, int ScenarioEvent, CBaseEntity* pEntity, class CBaseEntity* pEntityOther)
{
	// Round Event data
	P_ROUND_EVENT Event = { 0 };
	//
	// Round Count
	Event.Round = gMatchBot.GetRound();
	//
	// If has ReGameDLL API Game Rules
	if (g_pGameRules)
	{
		Event.Time = CSGameRules()->GetRoundRemainingTimeReal();
	}
	//
	// Store type of event
	Event.Type = event;
	//
	// Store event scenario
	Event.ScenarioEvent = ScenarioEvent;
	//
	// Switch of event
	switch (event)
	{
		// Tell bots the player is killed (argumens: 1 = victim, 2 = killer)
		case EVENT_PLAYER_DIED: 
		{
			// Get Victim
			auto Victim = UTIL_PlayerByIndexSafe(pEntity->entindex());

			// Get Killer
			auto Killer = UTIL_PlayerByIndexSafe(pEntityOther->entindex());

			// If event has victim abnd killer
			if (Victim && Killer)
			{
				// Get Killer auth index
				Event.Killer = GET_USER_AUTH(Killer->edict());

				// Get Killer Origin
				Event.KillerOrigin = Killer->edict()->v.origin;

				// Get Victim auth index
				Event.Victim = GET_USER_AUTH(Victim->edict());

				// Get Victim Origin
				Event.VictimOrigin = Victim->edict()->v.origin;

				// Get Winner of event (Team of Killer)
				Event.Winner = Killer->m_iTeam;

				// Get Loser of event (Team of Victim)
				Event.Loser = Victim->m_iTeam;

				// Check if victim was killed by a headshot
				Event.IsHeadShot = Victim->m_bHeadshotKilled ? 1 : 0;

				// Default weapon is empty
				Event.ItemIndex = WEAPON_NONE;

				// If killer has active item on hand
				if (Killer->m_pActiveItem)
				{
					// Store item index, and check if is HE Grenade
					Event.ItemIndex = (Victim->m_bKilledByGrenade ? WEAPON_HEGRENADE : Killer->m_pActiveItem->m_iId);
				}
			}
			
			//
			break;
		}
		// Tell bots the bomb has been planted (argumens: 1 = planter, 2 = NULL)
		case EVENT_BOMB_PLANTED: 
		{
			// Get Bomb Planter
			auto Planter = UTIL_PlayerByIndexSafe(pEntity->entindex());

			// If found
			if (Planter)
			{
				// Store as killer auth index
				Event.Killer = GET_USER_AUTH(Planter->edict());

				// Store killer origin
				Event.KillerOrigin = Planter->edict()->v.origin;
			}
			
			// Winner of that event is Terrorists
			Event.Winner = TERRORIST;

			// Loser of that event is CTs
			Event.Loser = CT;

			// Is not headshot, rofl
			Event.IsHeadShot = 0;

			// Weapon is C4
			Event.ItemIndex = WEAPON_C4;

			//
			break;
		}
		// Tell the bots the bomb is defused (argumens: 1 = defuser, 2 = NULL)
		case EVENT_BOMB_DEFUSED: 
		{
			auto Defuser = UTIL_PlayerByIndexSafe(pEntity->entindex());

			if (Defuser)
			{
				Event.Killer = GET_USER_AUTH(Defuser->edict());

				Event.KillerOrigin = Defuser->edict()->v.origin;

				Event.IsHeadShot = Defuser->m_bHasDefuser ? 1 : 0;
			}

			Event.Winner = CT;

			Event.Loser = TERRORIST;

			Event.ItemIndex = WEAPON_NONE;

			break;
		}
		// Let the bots hear the bomb exploding (argumens: 1 = NULL, 2 = NULL)
		case EVENT_BOMB_EXPLODED: 
		{
			auto Planter = UTIL_PlayerByIndexSafe(pEntity->entindex());

			if (Planter)
			{
				Event.Killer = GET_USER_AUTH(Planter->edict());

				Event.KillerOrigin = Planter->edict()->v.origin;
			}

			Event.Winner = TERRORIST;

			Event.Loser = CT;

			Event.IsHeadShot = 0;

			Event.ItemIndex = WEAPON_C4;

			break;
		}
		// Tell bots the terrorists won the round (argumens: 1 = NULL, 2 = NULL)
		case EVENT_TERRORISTS_WIN: 
		{
			Event.Winner = TERRORIST;

			Event.Loser = CT;

			Event.IsHeadShot = false;

			Event.ItemIndex = WEAPON_NONE;

			break;
		}
		// Tell bots the CTs won the round (argumens: 1 = NULL, 2 = NULL)
		case EVENT_CTS_WIN: 
		{
			Event.Winner = CT;

			Event.Loser = TERRORIST;

			Event.IsHeadShot = false;

			Event.ItemIndex = WEAPON_NONE;

			break;
		}
	}

	// Insert Event Data
	this->m_RoundEvent.push_back(Event);
}

// Show Enemy HP
bool CMatchStats::ShowHP(CBasePlayer* Player, bool Command, bool InConsole)
{
	if (!Command || (this->m_StatsCommandFlags & CMD_HP))
	{
		if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
		{
			if (g_pGameRules)
			{
				// Removed || CSGameRules()->IsFreezePeriod(), showing full enemy team hp on freezetime its not needed.
				if (!Player->IsAlive() || CSGameRules()->m_bRoundTerminating)
				{
					if (Player->m_iTeam == TERRORIST || Player->m_iTeam == CT)
					{
						auto StatsCount = 0;

						auto Players = gMatchUtil.GetPlayers(true, true);

						for (auto Target : Players)
						{
							if (Target->m_iTeam != Player->m_iTeam)
							{
								if (Target->IsAlive())
								{
									StatsCount++;

									gMatchUtil.SayText
									(
										Player->edict(),
										Target->entindex(),
										_T("^3%s^1 with %d HP (%d AP)"),
										STRING(Target->edict()->v.netname),
										(int)Target->edict()->v.health,
										(int)Target->edict()->v.armorvalue
									);
								}
							}
						}

						if (!StatsCount && !InConsole)
						{
							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("No one is alive."));
						}

						return true;
					}
				}
			}
		}

		gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));
	}

	return false;
}

// Show Damage
bool CMatchStats::ShowDamage(CBasePlayer* Player, bool Command, bool InConsole)
{
	if (!Command || (this->m_StatsCommandFlags & CMD_DMG))
	{
		if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
		{
			if (g_pGameRules)
			{
				if (!Player->IsAlive() || CSGameRules()->m_bRoundTerminating || CSGameRules()->IsFreezePeriod())
				{
					if (Player->m_iTeam == TERRORIST || Player->m_iTeam == CT)
					{
						auto StatsCount = 0;

						auto Players = gMatchUtil.GetPlayers(true, true);

						for (auto Target : Players)
						{
							if (this->m_RoundHit[Player->entindex()][Target->entindex()])
							{
								StatsCount++;

								if (!InConsole)
								{
									gMatchUtil.SayText
									(
										Player->edict(),
										Target->entindex(),
										_T("Hit ^3%s^1 %d time(s) (Damage %d)"),
										STRING(Target->edict()->v.netname),
										this->m_RoundHit[Player->entindex()][Target->entindex()],
										this->m_RoundDmg[Player->entindex()][Target->entindex()]
									);
								}
								else
								{
									gMatchUtil.ClientPrint
									(
										Player->edict(),
										PRINT_CONSOLE,
										_T("* Hit %s %d time(s) (Damage %d)"),
										STRING(Target->edict()->v.netname),
										this->m_RoundHit[Player->entindex()][Target->entindex()],
										this->m_RoundDmg[Player->entindex()][Target->entindex()]
									);
								}
							}
						}

						if (!StatsCount && !InConsole)
						{
							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You haven't hit anyone this round."));
						}

						return true;
					}
				}
			}
		}

		if (!InConsole)
		{
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));
		}
	}

	return false;
}

// Show Received Damage
bool CMatchStats::ShowReceivedDamage(CBasePlayer* Player, bool Command, bool InConsole)
{
	if (!Command || (this->m_StatsCommandFlags & CMD_RDMG))
	{
		if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
		{
			if (g_pGameRules)
			{
				if (!Player->IsAlive() || CSGameRules()->m_bRoundTerminating || CSGameRules()->IsFreezePeriod())
				{
					if (Player->m_iTeam == TERRORIST || Player->m_iTeam == CT)
					{
						auto StatsCount = 0;

						auto Players = gMatchUtil.GetPlayers(true, true);

						for (auto Target : Players)
						{
							if (this->m_RoundHit[Target->entindex()][Player->entindex()])
							{
								StatsCount++;

								gMatchUtil.SayText
								(
									Player->edict(),
									Target->entindex(),
									_T("Hit by ^3%s^1 %d time(s) (Damage %d)"),
									STRING(Target->edict()->v.netname),
									this->m_RoundHit[Target->entindex()][Player->entindex()],
									this->m_RoundDmg[Target->entindex()][Player->entindex()]
								);
							}
						}

						if (!StatsCount && !InConsole)
						{
							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You weren't hit this round."));
						}

						return true;
					}
				}
			}
		}

		gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));
	}

	return false;
}

// Show Round Summary
bool CMatchStats::ShowSummary(CBasePlayer* Player, bool Command, bool InConsole)
{
	if (!Command || (this->m_StatsCommandFlags & CMD_SUM))
	{
		if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
		{
			if (g_pGameRules)
			{
				if (!Player->IsAlive() || CSGameRules()->m_bRoundTerminating || CSGameRules()->IsFreezePeriod())
				{
					if (Player->m_iTeam == TERRORIST || Player->m_iTeam == CT)
					{
						auto StatsCount = 0;

						auto Players = gMatchUtil.GetPlayers(true, true);

						for (auto Target : Players)
						{
							if (this->m_RoundHit[Player->entindex()][Target->entindex()] || this->m_RoundHit[Target->entindex()][Player->entindex()])
							{
								StatsCount++;

								if (!InConsole)
								{
									gMatchUtil.SayText
									(
										Player->edict(),
										Target->entindex(),
										_T("(%d dmg | %d hits) to (%d dmg | %d hits) from ^3%s^1 (%d HP)"),
										this->m_RoundDmg[Player->entindex()][Target->entindex()],
										this->m_RoundHit[Player->entindex()][Target->entindex()],
										this->m_RoundDmg[Target->entindex()][Player->entindex()],
										this->m_RoundHit[Target->entindex()][Player->entindex()],
										STRING(Target->edict()->v.netname),
										Target->IsAlive() ? (int)Target->edict()->v.health : 0
									);
								}
								else
								{
									gMatchUtil.ClientPrint
									(
										Player->edict(),
										PRINT_CONSOLE,
										_T("* (%d dmg | %d hits) to (%d dmg | %d hits) from %s (%d HP)"),
										this->m_RoundDmg[Player->entindex()][Target->entindex()],
										this->m_RoundHit[Player->entindex()][Target->entindex()],
										this->m_RoundDmg[Target->entindex()][Player->entindex()],
										this->m_RoundHit[Target->entindex()][Player->entindex()],
										STRING(Target->edict()->v.netname),
										Target->IsAlive() ? (int)Target->edict()->v.health : 0
									);
								}
							}
						}

						if (!StatsCount && !InConsole)
						{
							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("No stats in this round."));
						}

						return true;
					}
				}
			}
		}

		if (!InConsole)
		{
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));
		}
	}

	return false;
}

// Round End Stats
void CMatchStats::RoundEndStats(int State)
{
	// Get round end stats type
	auto Type = (int)(gMatchBot.m_RoundEndStats->value);

	// If is enabled
	if (Type > 0)
	{
		// Get Players
		auto Players = gMatchUtil.GetPlayers(true, false);

		// Loop each player
		for (auto Player : Players)
		{
			// Show Damage command
			if (Type == 1 || Type == 3)
			{
				gMatchStats.ShowDamage(Player, false, (Type == 3));
			}
			// Show Summary command
			else if (Type == 2 || Type == 4)
			{
				gMatchStats.ShowSummary(Player, false, (Type == 4));
			}
		}
	}
}

// SayText message
bool CMatchStats::SayText(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity)
{
	// If match is live
	if (gMatchBot.GetState() >= STATE_FIRST_HALF)
	{
		// If has entity target
		if (pEntity)
		{
			// Get sender index of message
			auto SenderID = gMatchMessage.GetLong(0);
			//
			// If sender index and target entity index is same (To prevent hook for each entity)
			if (SenderID == ENTINDEX(pEntity))
			{
				// Get CBasePlayer data
				auto Player = UTIL_PlayerByIndexSafe(SenderID);

				// If is not null
				if (Player)
				{
					// Get argument 1
					auto Format = gMatchMessage.GetString(1);

					// If is not empty
					if (Format)
					{
						// Get argument 3
						auto TextMsg = gMatchMessage.GetString(3);

						// If is not empty
						if (TextMsg)
						{
							if (!Q_stricmp("#Cstrike_Chat_All", Format) || !Q_stricmp("#Cstrike_Chat_AllDead", Format))
							{
								gMatchStats.InsertChat(Player, TextMsg);
							}
						}
					}
				}
			}
		}
	}

	// Do not block original message call
	return false;
}

// Insert say on chat log of player
void CMatchStats::InsertChat(CBasePlayer* Player, const char* TextMsg)
{
	// If message is not empty
	if (TextMsg)
	{
		// Get playter auth index
		auto Auth = GET_USER_AUTH(Player->edict());

		// if is not null
		if (Auth)
		{
			// Chat log struct
			P_PLAYER_CHAT Row;

			// Size
			Row.Size = this->m_Chat.size();

			// Auth
			Row.Auth = Auth;

			// Time
			Row.Time = time(0);

			// Player is alive?
			Row.Alive = Player->IsAlive();

			// Team
			Row.Team = Player->m_iTeam;

			// Message text
			Row.Message = TextMsg;

			// Insert chat
			this->m_Chat.push_back(Row);
		}
	}
}
