#include "precompiled.h"

CMatchStats gMatchStats;

// On match bot new state
void CMatchStats::SetState(int State, bool KnifeRound)
{
	// Set current state
	this->m_State = State;

	// Switch states
	switch (this->m_State)
	{
		case STATE_DEAD:
		{
			// Clear all player data
			this->m_Player.clear();
			break;
		}
		case STATE_WARMUP:
		{
			// Reset match data
			this->m_Match.Reset();

			// Reset round event data
			this->m_Event.clear();

			// Loop each player
			for (auto& Player : this->m_Player)
			{
				// Clear Chat Log
				Player.second.Chat.clear();

				// Clear player round stats
				Player.second.Round.Reset();
			}
			break;
		}
		case STATE_START:
		{
			// Reset match data
			this->m_Match.Reset();

			// Reset round event data
			this->m_Event.clear();
			break;
		}
		case STATE_FIRST_HALF:
		{
			// Set Hostname
			this->m_Match.HostName = (gMatchBot.m_Hostname) ? gMatchBot.m_Hostname->string : "";

			// Set Map
			this->m_Match.Map = STRING(gpGlobals->mapname);

			// Set address
			this->m_Match.Address = (gMatchBot.m_NetAddress) ? gMatchBot.m_NetAddress->string : "";

			// Set match start time
			this->m_Match.Time[0] = time(nullptr);

			// Set match end time
			this->m_Match.Time[1] = 0;

			// Reset game mode
			this->m_Match.GameMode = gMatchVoteTeam.GetMode();

			// Match has Knife Round
			this->m_Match.KnifeRound = KnifeRound;

			// Reset round event data
			this->m_Event.clear();

			// Loop each player
			for (auto& Player : this->m_Player)
			{
				// Clear player stats
				Player.second.Stats[State].Reset();

				// Clear Chat Log
				Player.second.Chat.clear();

				// Clear player round stats
				Player.second.Round.Reset();
			}
			break;
		}
		case STATE_HALFTIME:
		case STATE_SECOND_HALF:
		case STATE_OVERTIME:
		{
			// Loop each player
			for (auto& Player : this->m_Player)
			{
				// Clear player stats
				Player.second.Stats[State].Reset();

				// Clear player round stats
				Player.second.Round.Reset();
			}
			break;
		}
		case STATE_END:
		{
			// Set match end time
			this->m_Match.Time[1] = time(nullptr);

			// Loop player list
			for (auto& Player : this->m_Player)
			{
				// Clear winner of match
				Player.second.Winner = 0;

				// If is in winner team
				if (Player.second.Team == static_cast<int>(gMatchBot.GetWinner()))
				{
					// Set player as winner team
					Player.second.Winner = 1;
				}
			}
			break;
		}
	}
}

// On player enter in game
void CMatchStats::PlayerGetIntoGame(CBasePlayer* Player)
{
	// Get Steam ID
	auto Auth = gMatchUtil.GetPlayerAuthId(Player->edict());

	// If is not null
	if (Auth)
	{
		// Set joined time
		this->m_Player[Auth].JoinGameTime = time(0);

		// Set name
		this->m_Player[Auth].Name = STRING(Player->edict()->v.netname);

		// Set team
		this->m_Player[Auth].Team = static_cast<int>(Player->m_iTeam);
	}
}

// On player disconnect
void CMatchStats::PlayerDisconnect(edict_t* pEntity)
{
	// Get Steam ID
	auto Auth = gMatchUtil.GetPlayerAuthId(pEntity);

	// If is not null
	if (Auth)
	{
		// Set disconnect time
		this->m_Player[Auth].DisconnectTime = time(0);
	}
}

// On player switch team
void CMatchStats::PlayerSwitchTeam(CBasePlayer* Player)
{
	// Get Steam ID
	auto Auth = gMatchUtil.GetPlayerAuthId(Player->edict());

	// If is not null
	if (Auth)
	{
		// Set name
		this->m_Player[Auth].Name = STRING(Player->edict()->v.netname);

		// Set team
		this->m_Player[Auth].Team = static_cast<int>(Player->m_iTeam);
	}
}

// On Round Restart
void CMatchStats::RoundRestart()
{
	// If match is not dead
	if (this->m_State != STATE_DEAD)
	{
		// If has CSGameRules
		if (g_pGameRules)
		{
			// If is complete reset
			if (CSGameRules()->m_bCompleteReset)
			{
				// Get match state
				auto State = gMatchBot.GetState();

				// Loop all saved players
				for (auto& Player : this->m_Player)
				{
					// Reset player stats of this state
					Player.second.Stats[State].Reset();

					// Reset round stats
					Player.second.Round.Reset();
				}
			}
		}
	}
}

// On send death message
void CMatchStats::SendDeathMessage(CBaseEntity* Killer, CBasePlayer* Victim, CBasePlayer* Assister, entvars_t* pevInflictor, const char* killerWeaponName, int iDeathMessageFlags, int iRarityOfKill)
{
	// If match is live
	if ((this->m_State == STATE_FIRST_HALF) || (this->m_State == STATE_SECOND_HALF) || (this->m_State == STATE_OVERTIME))
	{

	}
}

// On BOT manager event
void CMatchStats::CBotManager_OnEvent(GameEventType GameEvent, CBaseEntity* pEntity, CBaseEntity* pOther)
{
	// If match is live
	if ((this->m_State == STATE_FIRST_HALF) || (this->m_State == STATE_SECOND_HALF) || (this->m_State == STATE_OVERTIME))
	{
		// Player Event Data
		this->PlayerEvent(GameEvent, pEntity, pOther);

		// Match Event Data
		this->MatchEvent(GameEvent, pEntity, pOther);

		// Round Event Data
		this->RoundEvent(GameEvent, pEntity, pOther);
	}
}

// On Player event
void CMatchStats::PlayerEvent(GameEventType GameEvent, CBaseEntity* pEntity, CBaseEntity* pOther)
{
	// Switch event
	switch (GameEvent)
	{
		case EVENT_PLAYER_BLINDED_BY_FLASHBANG:
		{
			// If entity is not null
			if (!FNullEnt(pEntity))
			{
				// Cast to CBasePlayer
				auto Player = static_cast<CBasePlayer*>(pEntity);

				// If is not null
				if (Player)
				{
					// If is blind
					if (Player->IsBlind())
					{
						// If is full blind
						if (Player->m_blindAlpha >= 255)
						{
							// Get Steam ID
							auto Auth = gMatchUtil.GetPlayerAuthId(Player->edict());

							// If is not null
							if (Auth)
							{
								// Player blind count
								this->m_Player[Auth].Stats[this->m_State].Blind++;
							}
						}
					}
				}
			}
			break;
		}
		case EVENT_PLAYER_FOOTSTEP:
		{
			// If entity is not null
			if (!FNullEnt(pEntity))
			{
				// Cast to CBasePlayer
				auto Player = static_cast<CBasePlayer*>(pEntity);

				// If is not null
				if (Player)
				{
					// Get player move vars
					auto pMove = g_ReGameApi->GetPlayerMove();

					// If is not null
					if (pMove)
					{
						// If steps left is equal of next step sound
						if (pMove->iStepLeft == pMove->flTimeStepSound)
						{
							// Get Steam ID
							auto Auth = gMatchUtil.GetPlayerAuthId(Player->edict());

							// If is not null
							if (Auth)
							{
								// Player footsteps count
								this->m_Player[Auth].Stats[this->m_State].Footsteps++;
							}
						}
					}
				}
			}
			break;
		}
		case EVENT_PLAYER_JUMPED:
		{
			// If entity is not null
			if (!FNullEnt(pEntity))
			{
				// Cast to CBasePlayer
				auto Player = static_cast<CBasePlayer*>(pEntity);

				// If is not null
				if (Player)
				{
					// Get Steam ID
					auto Auth = gMatchUtil.GetPlayerAuthId(Player->edict());

					// If is not null
					if (Auth)
					{
						// Player jump count
						this->m_Player[Auth].Stats[this->m_State].Jumps++;
					}
				}
			}
			break;
		}
		case EVENT_PLAYER_DIED:
		{
			// Item index
			auto ItemIndex = 0;

			// Headshot
			auto LastHitGroup = 0;

			// If entity is not null
			if (!FNullEnt(pEntity))
			{
				// Cast to CBasePlayer
				auto Victim = static_cast<CBasePlayer*>(pEntity);

				// If is not null
				if (Victim)
				{
					// Get Steam ID
					auto VictimAuth = gMatchUtil.GetPlayerAuthId(Victim->edict());

					// If is not null
					if (VictimAuth)
					{
						// If is killed by HE Grenade
						if (Victim->m_bKilledByGrenade)
						{
							// Weapon HE Grenade
							ItemIndex = WEAPON_HEGRENADE;
						}

						// Victim hit group
						LastHitGroup = Victim->m_LastHitGroup;

						// Set deaths
						this->m_Player[VictimAuth].Stats[this->m_State].Deaths++;

						// Set weapon deaths
						this->m_Player[VictimAuth].Stats[this->m_State].Weapon[ItemIndex].Deaths++;

						// Set round deaths
						this->m_Player[VictimAuth].Round.Deaths++;

						// If victim is blinded by flash
						if (Victim->IsBlind())
						{
							// Set blind death
							this->m_Player[VictimAuth].Stats[this->m_State].BlindDeaths++;
						}

						// If has CSGameRules instance
						if (g_pGameRules)
						{
							// Set count variables
							auto NumAliveTR = 0, NumAliveCT = 0, NumDeadTR = 0, NumDeadCT = 0;

							// Intialize player counts
							CSGameRules()->InitializePlayerCounts(NumAliveTR, NumAliveCT, NumDeadTR, NumDeadCT);

							// If no one was dead
							if ((NumDeadTR == 0) && (NumDeadCT == 0))
							{
								// Set player as entry death of round
								this->m_Player[VictimAuth].Stats[this->m_State].EntryDeaths++;
							}
						}
					}
				}
			}

			// If entity is not null
			if (!FNullEnt(pOther))
			{
				// Cast to CBasePlayer
				auto Killer = static_cast<CBasePlayer*>(pOther);

				// If is not null
				if (Killer)
				{
					// Get Steam ID
					auto KillerAuth = gMatchUtil.GetPlayerAuthId(Killer->edict());

					// If is not null
					if (KillerAuth)
					{
						// If weapon is not set yet
						if (ItemIndex == WEAPON_NONE)
						{
							// If killer has active item
							if (Killer->m_pActiveItem)
							{
								// Set item index
								ItemIndex = Killer->m_pActiveItem->m_iId;
							}
						}

						// Set frags
						this->m_Player[KillerAuth].Stats[this->m_State].Frags++;

						// Set weapon frags
						this->m_Player[KillerAuth].Stats[this->m_State].Weapon[ItemIndex].Frags++;

						// Set round frags
						this->m_Player[KillerAuth].Round.Frags++;

						// Check kill time to set double kill
						if ((gpGlobals->time - this->m_Player[KillerAuth].Round.KillTime) < 0.25f)
						{
							// Increment double kills
							this->m_Player[KillerAuth].Stats[this->m_State].DoubleKill++;
						}

						// Set kill time
						this->m_Player[KillerAuth].Round.KillTime = gpGlobals->time;

						// If victim last hit group is head (1)
						if (LastHitGroup == 1)
						{
							// Set headshots
							this->m_Player[KillerAuth].Stats[this->m_State].Headshots++;

							// Set headshots
							this->m_Player[KillerAuth].Stats[this->m_State].Weapon[ItemIndex].Headshots++;

							// Set headshots
							this->m_Player[KillerAuth].Round.Headshots++;
						}

						// If killer is blinded by flash
						if (Killer->IsBlind())
						{
							// Set blind frags
							this->m_Player[KillerAuth].Stats[this->m_State].BlindFrags++;
						}

						// If has CSGameRules instance
						if (g_pGameRules)
						{
							// Set count variables
							auto NumAliveTR = 0, NumAliveCT = 0, NumDeadTR = 0, NumDeadCT = 0;

							// Intialize player counts
							CSGameRules()->InitializePlayerCounts(NumAliveTR, NumAliveCT, NumDeadTR, NumDeadCT);

							// If no one was dead
							if ((NumDeadTR == 0) && (NumDeadCT == 0))
							{
								// Set player as entry death of round
								this->m_Player[KillerAuth].Stats[this->m_State].EntryFrags++;
							}
						}

						// If entity is not null
						if (!FNullEnt(pEntity))
						{
							// Cast to CBasePlayer
							auto TempVictim = static_cast<CBasePlayer*>(pEntity);

							// If is not null
							if (TempVictim)
							{
								// If is not killed by AWP
								if (ItemIndex != WEAPON_AWP)
								{
									// If victim has full health
									if (TempVictim->m_iLastClientHealth >= 100)
									{
										// If killer damage has more than full heath
										if (Killer->m_lastDamageAmount >= 100)
										{
											// Set one shot
											this->m_Player[KillerAuth].Stats[this->m_State].OneShot++;
										}
									}
								}

								// If victim is not on ground
								if (!(TempVictim->edict()->v.flags & FL_ONGROUND))
								{
									// If plaer was falling
									if (TempVictim->m_flFallVelocity > 0.0f)
									{
										// Set fying frags
										this->m_Player[KillerAuth].Stats[this->m_State].FlyFrags++;
									}
								}
							}
						}
					}
				}
			}

			break;
		}
		case EVENT_PLAYER_LANDED_FROM_HEIGHT:
		{
			// If entity is not null
			if (!FNullEnt(pEntity))
			{
				// Cast to CBasePlayer
				auto Player = static_cast<CBasePlayer*>(pEntity);

				// If is not null
				if (Player)
				{
					// Get Steam ID
					auto Auth = gMatchUtil.GetPlayerAuthId(Player->edict());

					// If is not null
					if (Auth)
					{
						// Player landed from height count
						this->m_Player[Auth].Stats[this->m_State].LandedFromHeight++;
					}
				}
			}
			break;
		}
		case EVENT_PLAYER_TOOK_DAMAGE:
		{
			break;
		}
		case EVENT_PLAYER_CHANGED_TEAM:
		{
			// If entity is not null
			if (!FNullEnt(pEntity))
			{
				// Cast to CBasePlayer
				auto Player = static_cast<CBasePlayer*>(pEntity);

				// If is not null
				if (Player)
				{
					// Get Steam ID
					auto Auth = gMatchUtil.GetPlayerAuthId(Player->edict());

					// If is not null
					if (Auth)
					{
						// Set name
						this->m_Player[Auth].Name = STRING(Player->edict()->v.netname);

						// Set team
						this->m_Player[Auth].Team = static_cast<int>(Player->m_iTeam);
					}
				}
			}
			break;
		}
	}
}

// On Match event
void CMatchStats::MatchEvent(GameEventType GameEvent, CBaseEntity* pEntity, CBaseEntity* pOther)
{
	// Switch event
	switch (GameEvent)
	{
		case EVENT_ROUND_START:
		{
			break;
		}
	}
}

// On Round event
void CMatchStats::RoundEvent(GameEventType GameEvent, CBaseEntity* pEntity, CBaseEntity* pOther)
{
	// If is some of these events
	if ((GameEvent == EVENT_PLAYER_DIED) || (GameEvent == EVENT_BOMB_PLANTED) || (GameEvent == EVENT_BOMB_PICKED_UP) || (GameEvent == EVENT_BOMB_DEFUSED) || (GameEvent == EVENT_BOMB_EXPLODED) || (GameEvent == EVENT_TERRORISTS_WIN) || (GameEvent == EVENT_CTS_WIN))
	{
		// Round event data struct
		P_ROUND_EVENT Event = { };

		// Set current round count
		Event.Round = (gMatchBot.GetRound() + 1);

		// If has CSGameRules class
		if (g_pGameRules)
		{
			// Set round remaining time
			Event.Time = CSGameRules()->GetRoundRemainingTimeReal();
		}

		// Set what event is
		Event.GameEvent = GameEvent;

		// Round scenario event
		Event.ScenarioEvent = ROUND_NONE;

		// Switch events
		switch (GameEvent)
		{
			case EVENT_PLAYER_DIED:
			{
				// If entity is not null
				if (!FNullEnt(pEntity))
				{
					// Cast to CBasePlayer
					auto Victim = static_cast<CBasePlayer*>(pEntity);

					// If is not null
					if (Victim)
					{
						// Set Steam ID
						Event.Victim = gMatchUtil.GetPlayerAuthId(Victim->edict());

						// Set Origin
						Event.VictimOrigin = Victim->edict()->v.origin;

						// Set loser team
						Event.Loser = Victim->m_iTeam;

						// Is headshot
						Event.IsHeadShot = Victim->m_bHeadshotKilled;

						// If is killed by bomb
						if (Victim->m_bKilledByBomb)
						{
							// Set item
							Event.ItemIndex = WEAPON_C4;
						}

						// If is killed by HE
						if (Victim->m_bKilledByGrenade)
						{
							// Set item
							Event.ItemIndex = WEAPON_HEGRENADE;
						}
					}
				}

				// If entity is not null
				if (!FNullEnt(pOther))
				{
					// Cast to CBasePlayer
					auto Killer = static_cast<CBasePlayer*>(pOther);

					// If is not null
					if (Killer)
					{
						// Set Steam ID
						Event.Killer = gMatchUtil.GetPlayerAuthId(Killer->edict());

						// Set origin
						Event.KillerOrigin = Killer->edict()->v.origin;

						// Set winner
						Event.Winner = Killer->m_iTeam;

						// If has active item
						if (Killer->m_pActiveItem)
						{
							// If is not set by last victim
							if (Event.ItemIndex == WEAPON_NONE)
							{
								// Set item index
								Event.ItemIndex = Killer->m_pActiveItem->m_iId;
							}
						}
					}
				}

				break;
			}
			case EVENT_BOMB_PLANTED:
			{
				if (!FNullEnt(pEntity))
				{
					auto Player = UTIL_PlayerByIndexSafe(pEntity->entindex());

					if (Player)
					{
						auto Auth = gMatchUtil.GetPlayerAuthId(pEntity->edict());

						if (Auth)
						{
							Event.Killer = Auth;
							
							this->m_Player[Auth].Round.BombPlanter = pEntity->entindex();
						}

						Event.KillerOrigin = Player->edict()->v.origin;

						Event.Victim = "";

						Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };
					}

					Event.Winner = TERRORIST;

					Event.Loser = CT;

					Event.IsHeadShot = false;

					Event.ItemIndex = WEAPON_C4;
				}

				break;
			}
			case EVENT_BOMB_PICKED_UP:
			{
				// If is not null
				if (!FNullEnt(pEntity))
				{
					// Get CBasePlayer
					auto Player = static_cast<CBasePlayer*>(pEntity);

					// If is not null
					if (Player)
					{
						Event.Killer = gMatchUtil.GetPlayerAuthId(pEntity->edict());

						Event.KillerOrigin = Player->edict()->v.origin;

						Event.Victim = "";

						Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };
					}

					Event.Winner = TERRORIST;

					Event.Loser = CT;

					Event.IsHeadShot = true;

					Event.ItemIndex = WEAPON_C4;
				}

				break;
			}
			case EVENT_BOMB_DEFUSED:
			{
				// If is not null
				if (!FNullEnt(pEntity))
				{
					// Get CBasePlayer
					auto Player = static_cast<CBasePlayer*>(pEntity);

					// If is not null
					if (Player)
					{
						// Set killer
						Event.Killer = gMatchUtil.GetPlayerAuthId(pEntity->edict());

						// Set origin
						Event.KillerOrigin = Player->edict()->v.origin;

						// Set victim
						Event.Victim = "";

						// Set origin
						Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };

						// Is headshot (Defuser)
						Event.IsHeadShot = Player->m_bHasDefuser;
					}

					// Scenario event
					Event.ScenarioEvent = ROUND_BOMB_DEFUSED;

					// Set event winner
					Event.Winner = CT;

					// Set event loser
					Event.Loser = TERRORIST;

					// Set item index
					Event.ItemIndex = WEAPON_C4;
				}
				break;
			}
			case EVENT_BOMB_EXPLODED:
			{
				// Loop player stats
				for (auto& Player : this->m_Player)
				{
					// If entity index is valid
					if ((Player.second.Round.BombPlanter > 0) && (Player.second.Round.BombPlanter <= gpGlobals->maxClients))
					{
						// Get CBasePlayer data
						auto Planter = UTIL_PlayerByIndexSafe(Player.second.Round.BombPlanter);
						
						// If is not null
						if (Planter)
						{
							// Set Steam ID
							Event.Killer = gMatchUtil.GetPlayerAuthId(Planter->edict());

							// Set origin
							Event.KillerOrigin = Planter->m_vLastOrigin;
						}
					}
				}

				Event.Victim = "";

				Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };

				Event.IsHeadShot = false;

				Event.ScenarioEvent = ROUND_TARGET_BOMB;

				Event.Winner = TERRORIST;

				Event.Loser = CT;

				Event.ItemIndex = WEAPON_C4;
				break;
			}
			case EVENT_TERRORISTS_WIN:
			{
				Event.ScenarioEvent = ROUND_TERRORISTS_WIN;

				Event.Winner = TERRORIST;

				Event.Loser = CT;

				Event.Killer = "";

				Event.Victim = "";

				Event.KillerOrigin = { 0.0f, 0.0f, 0.0f };

				Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };

				Event.IsHeadShot = false;

				Event.ItemIndex = WEAPON_NONE;
				break;
			}
			case EVENT_CTS_WIN:
			{
				// Set screnario Event
				Event.ScenarioEvent = ROUND_CTS_WIN;

				Event.Winner = CT;

				Event.Loser = TERRORIST;

				Event.Killer = "";

				Event.Victim = "";

				Event.KillerOrigin = { 0.0f, 0.0f, 0.0f };

				Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };

				Event.IsHeadShot = false;

				Event.ItemIndex = WEAPON_NONE;
				break;
			}
		}

		// Push that match event
		this->m_Event.push_back(Event);
	}
}
