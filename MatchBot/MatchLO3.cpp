#include "precompiled.h"

CMatchLO3 gMatchLO3;

void CMatchLO3::Start()
{
	this->m_Restart = 3;

	if (g_pGameRules)
	{
		CSGameRules()->m_bGameStarted = true;
	}

	gMatchBot.RecordDemo(nullptr);

	g_engfuncs.pfnCvar_DirectSet(gMatchBot.m_SvRestart, std::to_string(this->m_Restart).c_str());

	gMatchUtil.SayText(nullptr, PRINT_TEAM_RED, _T("Live in three restarts! ^3Get Ready!!"));
} 

void CMatchLO3::RoundRestart()
{
	if(this->m_Restart > 0)
	{
		this->m_Restart--;

		g_engfuncs.pfnCvar_DirectSet(gMatchBot.m_SvRestart, std::to_string(this->m_Restart).c_str());

		if (!this->m_Restart)
		{
			gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(0, 255, 0, -1.0, 0.2, 1, 13.0, 13.0, 0.0f, 0.0f, 1), _T("--- MATCH IS LIVE ---"));

			gMatchUtil.SayText(nullptr, PRINT_TEAM_RED, _T("^4Match Is Live! Good Luck & Have Fun!!"));
		}
	}
}