#include "precompiled.h"

CMatchCaptain gMatchCaptain;

void CMatchCaptain::Init(int PlayersMin)
{
	auto Players = gMatchUtil.GetPlayers(false, true);

	if ((int)Players.size() > (PlayersMin / 2))
	{
		this->m_Running = true;

		this->m_PlayersMin = PlayersMin;

		g_engfuncs.pfnCVarSetString("mp_auto_join_team", "1");
		g_engfuncs.pfnCVarSetString("humans_join_team", "SPEC");
		g_engfuncs.pfnCVarSetString("bot_join_team", "SPEC");
		g_engfuncs.pfnCVarSetString("allow_spectators", "1");

		for (auto & Player : Players)
		{
			if (Player->m_iTeam != UNASSIGNED)
			{
				Player->edict()->v.deadflag = DEAD_DEAD;

				Player->CSPlayer()->JoinTeam(SPECTATOR);

				Player->ClearConditions(BIT_CONDITION_LEADER);

				Player->ClearConditions(BIT_CONDITION_INMENU);
			}
		}

		auto TeamA = (TeamName)RANDOM_LONG(1, 2);

		auto TeamB = (TeamA == CT) ? TERRORIST : CT;

		this->SetCaptain(Players[0], TeamA);

		this->SetCaptain(Players[1], TeamB);

		this->Menu(Players[RANDOM_LONG(0, 1)]);

		gMatchTask.Create(TASK_VOTE_LIST, 0.5f, true, (void*)this->PlayerList, this->m_PlayersMin);
	}
	else
	{
		gMatchUtil.SayText(nullptr, PRINT_TEAM_RED, _T("The choice of the teams failed: ^3Not enough players.^1"));

		this->Stop(STATE_FIRST_HALF);
	}
}

void CMatchCaptain::Stop(int NextState)
{
	this->m_Running = false;

	g_engfuncs.pfnCVarSetString("mp_auto_join_team", "0");
	g_engfuncs.pfnCVarSetString("humans_join_team", "any");
	g_engfuncs.pfnCVarSetString("bot_join_team", "any");

	if (g_pGameRules)
	{
		CSGameRules()->BalanceTeams();
	}

	gMatchTask.Remove(TASK_VOTE_LIST);

	gMatchTask.Create(TASK_CHANGE_STATE, 3.0f, false, (void*)gMatchBot.NextState, NextState);
}

void CMatchCaptain::PlayerDisconnect(edict_t* pEdict)
{
	if (this->m_Running)
	{
		auto Players = gMatchUtil.GetPlayers(false, true);

		if (Players.size() > (size_t)(this->m_PlayersMin / 2))
		{
			auto Captain = UTIL_PlayerByIndexSafe(ENTINDEX(pEdict));

			if (Captain)
			{
				gMatchTask.Remove(Captain->entindex());

				if (Captain->HasConditions(BIT_CONDITION_LEADER))
				{
					for (auto const& Player : Players)
					{
						if (Player->m_iTeam == SPECTATOR)
						{
							this->SetCaptain(Player, Captain->m_iTeam);

							if (Captain->HasConditions(BIT_CONDITION_INMENU))
							{
								this->Menu(Player);
							}

							return;
						}
					}
				}
			}
		}
		else
		{
			gMatchUtil.SayText(nullptr, PRINT_TEAM_RED, _T("The choice of the teams failed: ^3Not enough players.^1"));

			this->Stop(STATE_WARMUP);
		}
	}
}

void CMatchCaptain::SetCaptain(CBasePlayer* Player, TeamName Team)
{
	Player->CSPlayer()->JoinTeam(Team);

	if (!Player->IsAlive())
	{
		Player->RoundRespawn();
	}

	Player->SetConditions(BIT_CONDITION_LEADER);

	Player->ClearConditions(BIT_CONDITION_INMENU);

	gMatchUtil.SayText(nullptr, (Team == TERRORIST ? PRINT_TEAM_RED : PRINT_TEAM_BLUE), _T("The ^3%s^1 captain is: %s"), gMatchBot.GetTeam(Team, false), STRING(Player->edict()->v.netname));
}

void CMatchCaptain::GetPlayer(CBasePlayer* Captain, CBasePlayer* Target)
{
	gMatchTask.Remove(Captain->entindex());

	if (Captain)
	{
		if (Target != nullptr)
		{
			if (Target->m_iTeam == SPECTATOR)
			{
				Target->CSPlayer()->JoinTeam(Captain->m_iTeam);

				if (!Target->IsAlive())
				{
					Target->RoundRespawn();
				}

				Captain->ClearConditions(BIT_CONDITION_INMENU);

				this->PlayerList(this->m_PlayersMin);

				gMatchUtil.SayText(nullptr, (Captain->m_iTeam == TERRORIST ? PRINT_TEAM_RED : PRINT_TEAM_BLUE), _T("^3%s^1 choosed ^3%s^1"), STRING(Captain->edict()->v.netname), STRING(Target->edict()->v.netname));

				this->NextMenu(Captain);
			}
			else
			{
				gMatchUtil.SayText(nullptr, (Target->m_iTeam == TERRORIST ? PRINT_TEAM_RED : PRINT_TEAM_BLUE), _T("^3%s^1 already on the team ^3%s^1"), STRING(Target->edict()->v.netname), gMatchBot.GetTeam(Target->m_iTeam, false));

				this->Menu(Captain);
			}
		}
		else
		{
			auto Players = gMatchUtil.GetPlayers(false, true);

			for (auto const& Player : Players)
			{
				if (Player->m_iTeam == SPECTATOR)
				{
					this->GetPlayer(Captain, Player);
					break;
				}
			}
		}
	}
}

void CMatchCaptain::NextMenu(CBasePlayer* Captain)
{
	auto Spectators = gMatchUtil.GetCount(SPECTATOR);

	if (Spectators)
	{
		auto Players = gMatchUtil.GetPlayers(true, true);

		for (auto const& Player : Players)
		{
			if (Player->HasConditions(BIT_CONDITION_LEADER))
			{
				if (Player->entindex() != Captain->entindex())
				{
					if (gMatchUtil.GetCount(Player->m_iTeam) < (this->m_PlayersMin / 2))
					{
						this->Menu(Player);
						return;
					}
				}
			}
		}

		if (gMatchUtil.GetCount(Captain->m_iTeam) < (this->m_PlayersMin / 2))
		{
			this->Menu(Captain);
			return;
		}
	}

	this->Stop(STATE_FIRST_HALF);
}

void CMatchCaptain::Menu(CBasePlayer* Captain)
{
	if (gMatchUtil.GetCount(SPECTATOR) == 1 || Captain->IsBot())
	{
		gMatchTask.Create(Captain->entindex(), 1.0f, false, (void*)this->GetRandomPlayer, Captain->entindex());
	}
	else
	{
		auto EntityIndex = Captain->entindex();

		gMatchMenu[EntityIndex].Create(gMatchBot.GetTeam(SPECTATOR, false), false, (void*)this->MenuHandle);

		auto Players = gMatchUtil.GetPlayers(false, true);

		for (auto const& Player : Players)
		{
			if (Player->m_iTeam == SPECTATOR)
			{
				gMatchMenu[EntityIndex].AddItem(Player->entindex(), STRING(Player->edict()->v.netname));
			}
		}

		gMatchMenu[EntityIndex].Show(EntityIndex);

		Captain->SetConditions(BIT_CONDITION_INMENU);

		gMatchTask.Create(Captain->entindex(), 10.0f, false, (void*)this->GetRandomPlayer, Captain->entindex());
	}
}

void CMatchCaptain::MenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Captain = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Captain)
	{
		auto Player = UTIL_PlayerByIndexSafe(Item.Info);

		if (Player)
		{
			gMatchCaptain.GetPlayer(Captain, Player);
		}
	}
}

void CMatchCaptain::GetRandomPlayer(int EntityIndex)
{
	auto Captain = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Captain)
	{
		gMatchMenu[EntityIndex].Hide(EntityIndex);

		gMatchCaptain.GetPlayer(Captain, nullptr);
	}
}

void CMatchCaptain::PlayerList(int PlayersMin)
{
	std::string PlayerList[SPECTATOR + 1];

	int Count[SPECTATOR + 1] = { 0 };

	for (int i = 1; i <= gpGlobals->maxClients; ++i)
	{
		auto Player = UTIL_PlayerByIndexSafe(i);

		if (Player)
		{
			if (!Player->IsDormant())
			{
				if (Player->m_iTeam != UNASSIGNED)
				{
					Count[Player->m_iTeam]++;

					PlayerList[Player->m_iTeam] += STRING(Player->edict()->v.netname);

					if (Player->HasConditions(BIT_CONDITION_LEADER))
					{
						if (Player->HasConditions(BIT_CONDITION_INMENU))
						{
							PlayerList[Player->m_iTeam] += _T(" (C) *");
						}
						else
						{
							PlayerList[Player->m_iTeam] += _T(" (C)");
						}
					}

					PlayerList[Player->m_iTeam] += "\n";
				}
			}
		}
	}

	for (int i = 0; i < 5 - Count[CT]; i++)
	{
		PlayerList[CT] += "\n";
	}

	gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(0, 255, 0, 0.75, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 1), "%s (%d / %d):", gMatchBot.GetTeam(TERRORIST, false), Count[TERRORIST], (PlayersMin / 2));

	gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(255, 255, 255, 0.75, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 2), "\n%s", PlayerList[TERRORIST].c_str());

	if (Count[SPECTATOR])
	{
		gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(0, 255, 0, 0.75, 0.28, 0, 0.0, 0.8, 0.0, 0.0, 3), "%s (%d / %d):\n\n\n\n\n\n%s (%d):", gMatchBot.GetTeam(CT, false), Count[CT], (PlayersMin / 2), gMatchBot.GetTeam(SPECTATOR, false), Count[SPECTATOR]);

		gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(255, 255, 255, 0.75, 0.28, 0, 0.0, 0.8, 0.0, 0.0, 4), "\n%s\n%s", PlayerList[CT].c_str(), PlayerList[SPECTATOR].c_str());
	}
	else
	{
		gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(0, 255, 0, 0.75, 0.28, 0, 0.0, 0.8, 0.0, 0.0, 3), "%s (%d / %d):", gMatchBot.GetTeam(CT, false), Count[CT], (PlayersMin / 2));

		gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(255, 255, 255, 0.75, 0.28, 0, 0.0, 0.8, 0.0, 0.0, 4), "\n%s", PlayerList[CT].c_str());
	}
}
