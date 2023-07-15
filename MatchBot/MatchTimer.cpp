#include "precompiled.h"

CMatchTimer gMatchTimer;

void CMatchTimer::Init(int PlayersMin,int Delay)
{
	this->m_Delay = Delay;

	this->SetTime();

	gMatchTask.Create(TASK_READY_SYSTEM, 0.5f, true, (void*)this->Task, PlayersMin);

	gMatchUtil.SayText(nullptr, PRINT_TEAM_RED, _T("Match will start when all players join in game."));
}

void CMatchTimer::Stop(bool ChangeState)
{
	gMatchTask.Remove(TASK_READY_SYSTEM);

	if (ChangeState)
	{
		gMatchUtil.SayText(nullptr, PRINT_TEAM_RED, _T("All players are in teams and are ready!"));

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

void CMatchTimer::SetTime()
{
	this->m_CurrentTime = time(nullptr);
}

time_t CMatchTimer::GetTime()
{
	return this->m_CurrentTime;
}

int CMatchTimer::GetDelay()
{
	return this->m_Delay;
}

void CMatchTimer::Task(int PlayersMin)
{
	auto NeededCount = (PlayersMin - gMatchUtil.GetPlayers(true, true).size());

	if (NeededCount > 0)
	{
		gMatchTimer.SetTime();

		if (NeededCount > 1)
		{
			gMatchUtil.HudMessage(nullptr, gMatchUtil.HudParam(0, 255, 0, -1.0, 0.2, 0, 0.53, 0.53, 0.0, 0.0, 1), _T("WARMUP^n%d PLAYERS LEFT"), NeededCount);
		}
		else
		{
			gMatchUtil.HudMessage(nullptr, gMatchUtil.HudParam(0, 255, 0, -1.0, 0.2, 0, 0.53, 0.53, 0.0, 0.0, 1), _T("WARMUP^n%d PLAYER LEFT"), NeededCount);
		}
	}
	else
	{
		time_t RemainTime = (time_t)((time_t)gMatchTimer.GetDelay() - (time(nullptr) - gMatchTimer.GetTime()));

		if (RemainTime > 0)
		{
			struct tm* tm_info = localtime(&RemainTime);

			if (tm_info)
			{
				char Time[16];

				strftime(Time, sizeof(Time), "%M:%S", tm_info);

				gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(0, 255, 0, -1.0, 0.2, 0, 0.53, 0.53, 0.0, 0.0, 1), "%s^n%s", _T("STARTING MATCH"), Time);
			}
		}
		else
		{
			gMatchTimer.Stop(true);
		}
	}
}
