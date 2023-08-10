#include "precompiled.h"

CMatchStats gMatchStats;

void CMatchStats::ServerActivate()
{
	// Match stats commands
	this->m_StatsCommandFlags = CMD_ALL;
}

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
}

// Round Start
void CMatchStats::RoundStart()
{
	// If match is live
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		// Clear Round Damage
		this->m_RoundDmg.fill({});

		// Clear Round Hits
		this->m_RoundHit.fill({});
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
				// Attacker entity index
				auto AttackerIndex = Attacker->entindex();

				// Victim entity index
				auto VictimIndex = Victim->entindex();

				// If attacker is not victim
				if (AttackerIndex != VictimIndex)
				{
					// If victim can receive damage form attacker
					if (CSGameRules()->FPlayerCanTakeDamage(Victim, Attacker))
					{
						// Attacker Round Damage
						this->m_RoundDmg[AttackerIndex][VictimIndex] += (int)(Victim->m_iLastClientHealth - clamp(Victim->edict()->v.health, 0.0f, Victim->edict()->v.health));

						// Attacker Round Hits
						this->m_RoundHit[AttackerIndex][VictimIndex]++;
					}
				}
			}
		}
		else
		{
			// Get attacker
			auto Attacker = UTIL_PlayerByIndexSafe(ENTINDEX(pevAttacker));

			// Make C4 Death Message
			gMatchUtil.MakeDeathMessage((Attacker != nullptr) ? ((Attacker->entindex() != Victim->entindex()) ? Attacker->edict() : nullptr) : nullptr, Victim->edict(), false, "c4");
		}
	}
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
