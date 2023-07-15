#include "precompiled.h"

CMatchReady gMatchReady;

void CMatchReady::Init(int PlayersMin)
{
	this->m_Running = true;

	std::fill(this->m_Ready.begin(), this->m_Ready.end(), false);

	gMatchUtil.SayText(nullptr, PRINT_TEAM_RED, _T("Say ^4.ready^1 to continue."));

	gMatchTask.Create(TASK_READY_SYSTEM, 0.5f, true, (void*)this->Task, PlayersMin);
}

void CMatchReady::Stop(bool ChangeState)
{
	if (this->m_Running)
	{
		this->m_Running = false;

		std::fill(this->m_Ready.begin(), this->m_Ready.end(), false);

		gMatchTask.Remove(TASK_READY_SYSTEM);

		if (ChangeState)
		{
			auto State = STATE_START;

			if (gMatchBot.GetState() == STATE_HALFTIME)
			{
				State = STATE_SECOND_HALF;

				if (gMatchBot.GetRound() >= (int)(gMatchBot.m_PlayRounds->value))
				{
					State = STATE_OVERTIME;
				}
			}

			gMatchTask.Create(TASK_CHANGE_STATE, 1.0f, false, (void*)gMatchBot.NextState, State);
		}
	}
}

void CMatchReady::Task(int PlayersMin)
{
	std::string PlayerList[2];

	std::array<int, 2> PlayerCount = { 0 };

	for (int i = 1; i <= gpGlobals->maxClients; ++i)
	{
		auto Player = UTIL_PlayerByIndexSafe(i);

		if (Player)
		{
			if (!Player->IsDormant())
			{
				if (Player->m_iTeam == TERRORIST || Player->m_iTeam == CT)
				{
					int ReadyState = (Player->IsBot() || gMatchReady.GetReady(Player->entindex())) ? 1 : 0;

					PlayerCount[ReadyState]++;

					PlayerList[ReadyState].append(STRING(Player->edict()->v.netname));

					PlayerList[ReadyState].append("\n");
				}
			}
		}
	}

	if (PlayerCount[1] >= PlayersMin)
	{
		gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("All players are ready!"));

		gMatchReady.Stop(true);
	}
	else
	{
		gMatchUtil.HudMessage(nullptr, gMatchUtil.HudParam(0, 255, 0, 0.23, 0.02, 0, 0.0, 0.53, 0.0, 0.0, 1), _T("Not Ready (%d of %d):"), PlayerCount[0], PlayersMin);

		gMatchUtil.HudMessage(nullptr, gMatchUtil.HudParam(0, 255, 0, 0.58, 0.02, 0, 0.0, 0.53, 0.0, 0.0, 2), _T("Ready (%d of %d):"), PlayerCount[1], PlayersMin);

		gMatchUtil.HudMessage(nullptr, gMatchUtil.HudParam(255, 255, 225, 0.23, 0.02, 0, 0.0, 0.53, 0.0, 0.0, 4), "\n%s", PlayerList[0].c_str());

		gMatchUtil.HudMessage(nullptr, gMatchUtil.HudParam(255, 255, 225, 0.58, 0.02, 0, 0.0, 0.53, 0.0, 0.0, 3), "\n%s", PlayerList[1].c_str());
	}
}

void CMatchReady::PlayerGetIntoGame(CBasePlayer* Player)
{
	this->SetReady(Player->entindex(), false);

	if (this->m_Running)
	{
		if (!this->GetReady(Player->entindex()))
		{
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_RED, _T("Say ^4.ready^1 to continue."));
		}
	}
}

void CMatchReady::SetReady(int EntityIndex, bool Ready)
{
	this->m_Ready[EntityIndex] = Ready;
}

bool CMatchReady::GetReady(int EntityIndex)
{
	return this->m_Ready[EntityIndex];
}

bool CMatchReady::Ready(CBasePlayer* Player)
{
	if (this->m_Running)
	{
		if (!this->GetReady(Player->entindex()))
		{
			if (Player->m_iTeam == TERRORIST || Player->m_iTeam == CT)
			{
				this->SetReady(Player->entindex(), true);

				gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 is ready."), STRING(Player->edict()->v.netname));

				return true;
			}
		}
	}

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));

	return false;
}

bool CMatchReady::NotReady(CBasePlayer* Player)
{
	if (this->m_Running)
	{
		if (this->GetReady(Player->entindex()))
		{
			if (Player->m_iTeam == TERRORIST || Player->m_iTeam == CT)
			{
				this->SetReady(Player->entindex(), false);

				gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 is not ready."), STRING(Player->edict()->v.netname));

				return true;
			}
		}
	}

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));

	return false;
}
