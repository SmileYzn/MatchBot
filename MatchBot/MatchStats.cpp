#include "precompiled.h"

CMatchStats gMatchStats;

// Change States
void CMatchStats::SetState(int State)
{
	// New Match State
	this->m_State = State;

	// Store star time if is First Half
	if(State == STATE_FIRST_HALF)
	{
		this->m_Data.StartTime = time(0);
	}
	// Store scores in each halftime
	else if(State == STATE_HALFTIME)
	{
		// Get Players
		auto Players = gMatchUtil.GetPlayers(true, true);

		// Loop Players
		for(auto & Player : Players)
		{
			// Get player entity index
			auto EntityIndex = Player->entindex();

			// Store Frags
			this->m_Score[EntityIndex][0] = (int)Player->edict()->v.frags;

			// Store deaths
			this->m_Score[EntityIndex][1] = (int)Player->m_iDeaths;
		}
	}
	// Store end time if is end
	else if(State == STATE_END)
	{
		this->m_Data.EndTime = time(0);
	}
}

// Get Player Score
int* CMatchStats::GetScore(int EntityIndex)
{
	// Return Scores
	return this->m_Score[EntityIndex];
}

// On Player Connect
bool CMatchStats::PlayerConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	auto Auth = GET_USER_AUTH(pEntity);

	if (Auth)
	{
		// Set Connection time
		this->m_Player[Auth].ConnectTime = time(0);

		// Reset Frags
		this->m_Score[ENTINDEX(pEntity)][0] = 0;

		// Reset Deaths
		this->m_Score[ENTINDEX(pEntity)][1] = 0;
	}

	return true;
}

// When player try to join in a team, we do things
bool CMatchStats::PlayerJoinTeam(CBasePlayer* Player, int Slot)
{
	auto Auth = GET_USER_AUTH(Player->edict());

	if (Auth)
	{
		// Update team when player change
		this->m_Player[Auth].Team = Player->m_iTeam;
	}

	return false;
}

// While player get into game (Enter in any TERRORIST or CT Team)
void CMatchStats::PlayerGetIntoGame(CBasePlayer* Player)
{
	auto Auth = GET_USER_AUTH(Player->edict());

	if (Auth)
	{
		// Get into game time
		this->m_Player[Auth].GetIntoGameTime = time(nullptr);

		// Update new team
		this->m_Player[Auth].Team = Player->m_iTeam;
	}
}

// When player disconnect from game
void CMatchStats::PlayerDisconnect(edict_t* pEdict)
{
	auto Auth = GET_USER_AUTH(pEdict);

	if (Auth)
	{
		// Get disconnection time
		this->m_Player[Auth].DisconnectedTime = time(nullptr);
	}
}

// Round Restart
void CMatchStats::RoundRestart(bool PreRestart)
{
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
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
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		// Clear Round Damage
		memset(this->m_RoundDmg, 0, sizeof(this->m_RoundDmg));

		// Clear Round Hits
		memset(this->m_RoundHit, 0, sizeof(this->m_RoundHit));
	}
}

// Round End
void CMatchStats::RoundEnd(int winStatus, ScenarioEventEndRound eventScenario, float tmDelay)
{
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		if (winStatus == WINSTATUS_TERRORISTS || winStatus == WINSTATUS_CTS)
		{
			auto Winner = winStatus == WINSTATUS_TERRORISTS ? TERRORIST : CT;

			auto Players = gMatchUtil.GetPlayers(true, true);

			for (auto Player : Players)
			{
				auto Auth = GET_USER_AUTH(Player->edict());

				if (Auth)
				{
					this->m_Player[Auth].Stats[this->m_State].RoundsPlay++;

					if (Player->m_iTeam == Winner)
					{
						this->m_Player[Auth].Stats[this->m_State].RoundsWin++;
					}
					else
					{
						this->m_Player[Auth].Stats[this->m_State].RoundsLose++;
					}
				}
			}
		}
	}
}

void CMatchStats::PlayerDamage(CBasePlayer* Victim, entvars_t* pevInflictor, entvars_t* pevAttacker, float& flDamage, int bitsDamageType)
{
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		if (!Victim->m_bKilledByBomb)
		{
			auto Attacker = UTIL_PlayerByIndexSafe(ENTINDEX(pevAttacker));

			if (Attacker)
			{
				if (Victim->entindex() != Attacker->entindex())
				{
					if (Victim->IsPlayer() && Attacker->IsPlayer())
					{
						if (CSGameRules()->FPlayerCanTakeDamage(Victim, Attacker))
						{
							// Victim Auth
							auto VictimAuth = GET_USER_AUTH(Victim->edict());

							// Attacker Auth
							auto AttackerAuth = GET_USER_AUTH(Attacker->edict());

							// Damage Taken
							auto DamageTaken = (int)(Victim->m_iLastClientHealth - clamp(Victim->edict()->v.health, 0.0f, Victim->edict()->v.health));

							// Item Index
							//auto ItemIndex = (bitsDamageType & DMG_EXPLOSION) ? WEAPON_HEGRENADE : ((Attacker->m_pActiveItem) ? Attacker->m_pActiveItem->m_iId : WEAPON_NONE);

							// Attacker Damage
							this->m_Player[AttackerAuth].Stats[this->m_State].Damage += DamageTaken;

							// Attacker Hits
							this->m_Player[AttackerAuth].Stats[this->m_State].Hits++;

							// Attacker HitBox Hits
							this->m_Player[AttackerAuth].Stats[this->m_State].HitBoxAttack[Victim->m_LastHitGroup][0]++;

							// Attacker HitBox Damage
							this->m_Player[AttackerAuth].Stats[this->m_State].HitBoxAttack[Victim->m_LastHitGroup][1] += DamageTaken;

							// Victim Damage Received
							this->m_Player[VictimAuth].Stats[this->m_State].DamageReceived += DamageTaken;

							// Victim Hits Received
							this->m_Player[VictimAuth].Stats[this->m_State].HitsReceived++;

							// Victim HitBox Hits
							this->m_Player[VictimAuth].Stats[this->m_State].HitBoxAttack[Victim->m_LastHitGroup][0]++;

							// Victim HitBox Damage
							this->m_Player[VictimAuth].Stats[this->m_State].HitBoxAttack[Victim->m_LastHitGroup][1] += DamageTaken;

							// Attacker Round Damage
							this->m_RoundDmg[Attacker->entindex()][Victim->entindex()] += DamageTaken;

							// Attacker Round Hits
							this->m_RoundHit[Attacker->entindex()][Victim->entindex()]++;
						}
					}
				}
			}
		}
	}
}

void CMatchStats::PlayerKilled(CBasePlayer* Victim, entvars_t* pevKiller, entvars_t* pevInflictor)
{
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		if (!Victim->m_bKilledByBomb)
		{
			auto Attacker = UTIL_PlayerByIndexSafe(ENTINDEX(pevKiller));

			if (Attacker)
			{
				if (Victim->entindex() != Attacker->entindex())
				{
					if (Victim->IsPlayer() && Attacker->IsPlayer())
					{
						// Victim Auth
						auto VictimAuth = GET_USER_AUTH(Victim->edict());

						// Attacker Auth
						auto AttackerAuth = GET_USER_AUTH(Attacker->edict());

						// Item Index
						//auto ItemIndex = (Victim->m_bKilledByGrenade) ? WEAPON_HEGRENADE : ((Attacker->m_pActiveItem) ? Attacker->m_pActiveItem->m_iId : WEAPON_NONE);

						// Frags
						this->m_Player[AttackerAuth].Stats[this->m_State].Frags++;

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

						// Headshots
						if (Victim->m_LastHitGroup == 1)
						{
							this->m_Player[AttackerAuth].Stats[this->m_State].Headshots++;
						}
					}
				}
			}
		}
	}
}

// Set Animation
void CMatchStats::PlayerSetAnimation(CBasePlayer* Player, PLAYER_ANIM playerAnim)
{
	if(this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		if ((playerAnim == PLAYER_ATTACK1) || (playerAnim == PLAYER_ATTACK2))
		{
			if (Player->m_pActiveItem)
			{
				if (Player->m_pActiveItem->m_iId)
				{
					if (Player->m_pActiveItem->IsWeapon())
					{
						if (Player->m_pActiveItem->CanDrop())
						{
							auto Auth = GET_USER_AUTH(Player->edict());

							if (Auth)
							{
								// Shots
								this->m_Player[Auth].Stats[this->m_State].Shots++;

								// Weapon Shots
							}
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
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		if (type != RT_NONE)
		{
			auto Auth = GET_USER_AUTH(Player->edict());

			if (Auth)
			{
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
						this->m_Player[Auth].Stats[this->m_State].BombExploded++;
					}
				}
			}
		}
	}
}

// Show Enemy HP
bool CMatchStats::ShowHP(CBasePlayer* Player)
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
						if (Target->m_iTeam != Player->m_iTeam)
						{
							if (Target->IsAlive())
							{
								StatsCount++;

								gMatchUtil.SayText
								(
									Player->edict(),
									Target->entindex(),
									_T("\3%s\1 with %d HP (%d AP)"),
									STRING(Target->edict()->v.netname),
									(int)Target->edict()->v.health,
									(int)Target->edict()->v.armorvalue
								);
							}
						}
					}

					if (!StatsCount)
					{
						gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("No one is alive."));
					}

					return true;
				}
			}
		}
	}

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));

	return false;
}

// Show Damage
bool CMatchStats::ShowDamage(CBasePlayer* Player)
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

							gMatchUtil.SayText
							(
								Player->edict(),
								Target->entindex(),
								_T("Hit \3%s\1 %d time(s) (Damage %d)"),
								STRING(Target->edict()->v.netname),
								this->m_RoundHit[Player->entindex()][Target->entindex()],
								this->m_RoundDmg[Player->entindex()][Target->entindex()]
							);
						}
					}

					if (!StatsCount)
					{
						gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You haven't hit anyone this round."));
					}

					return true;
				}
			}
		}
	}

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));

	return false;
}

// Show Received Damage
bool CMatchStats::ShowReceivedDamage(CBasePlayer* Player)
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
								_T("Hit by \3%s\1 %d time(s) (Damage %d)"),
								STRING(Target->edict()->v.netname),
								this->m_RoundHit[Target->entindex()][Player->entindex()],
								this->m_RoundDmg[Target->entindex()][Player->entindex()]
							);
						}
					}

					if (!StatsCount)
					{
						gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You weren't hit this round."));
					}

					return true;
				}
			}
		}
	}

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));

	return false;
}

// Show Round Summary
bool CMatchStats::ShowSummary(CBasePlayer* Player)
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

							gMatchUtil.SayText
							(
								Player->edict(),
								Target->entindex(),
								_T("(%d dmg / %d hits) to (%d dmg / %d hits) from \3%s\1 (%d HP)"),
								this->m_RoundDmg[Player->entindex()][Target->entindex()],
								this->m_RoundHit[Player->entindex()][Target->entindex()],
								this->m_RoundDmg[Target->entindex()][Player->entindex()],
								this->m_RoundHit[Target->entindex()][Player->entindex()],
								STRING(Target->edict()->v.netname),
								Target->IsAlive() ? (int)Target->edict()->v.health : 0
							);
						}
					}

					if (!StatsCount)
					{
						gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("No stats in this round."));
					}

					return true;
				}
			}
		}
	}

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));

	return false;
}
