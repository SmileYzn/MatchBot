#include "precompiled.h"

CMatchLO3 gMatchLO3;

void CMatchLO3::Run()
{
	this->Restart(3);

	gMatchUtil.SayText(nullptr, PRINT_TEAM_RED, _T("Live in three restarts! \3Get Ready!!"));
}

void CMatchLO3::Restart(int Time)
{
	if (Time)
	{
		gMatchTask.Create(TASK_TIMER_LO3, (float)(Time > 1 ? Time : 2), false, (void*)gMatchLO3.Restart, (Time - 1));

		g_engfuncs.pfnCVarSetFloat("sv_restart", (float)Time);
	}
	else
	{
		if (g_pGameRules)
		{
			CSGameRules()->m_bGameStarted = true;
		}

		gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(0, 255, 0, -1.0, 0.2, 1, 12.0, 12.0, 0.0f, 0.0f, 1), _T("--- MATCH IS LIVE ---"));

		gMatchUtil.SayText(nullptr, PRINT_TEAM_RED, _T("\4Match Is Live! Good Luck & Have Fun!!"));
	}
}
