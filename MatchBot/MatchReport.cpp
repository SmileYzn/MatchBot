#include "precompiled.h"

CMatchReport gMatchReport;

void CMatchReport::ServerActivate()
{
	this->m_Data.clear();
	this->m_Reason.clear();
}

bool CMatchReport::Report(CBasePlayer* Player)
{
	auto pCmdArgs = g_engfuncs.pfnCmd_Args();

	if (pCmdArgs)
	{
		if (pCmdArgs[0u] != '\0')
		{
			auto State = gMatchBot.GetState();

			if (State != STATE_DEAD && State != STATE_START || State != STATE_END)
			{
				if ((Player->m_iTeam == TERRORIST) || (Player->m_iTeam == CT))
				{
					std::string Args = pCmdArgs;

					if (!Args.empty() && Args.length() > MINIMUM_REPORT_REASON_LENGTH)
					{
						Args.erase(std::remove(Args.begin(), Args.end(), '\"'), Args.end());

						this->Menu(Player, Args);

						return true;
					}
					else
					{
						gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("The report reason must have %d or more characters."), MINIMUM_REPORT_REASON_LENGTH);

						return false;
					}
				}
			}

			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));

			return false;
		}
	}

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Usage: .report \3<Reason of Report>\1"));

	return false;
}

void CMatchReport::Menu(CBasePlayer* Player, std::string Report)
{
	auto Players = gMatchUtil.GetPlayers(false, true);

	if (Players.size() > 1)
	{
		auto EntityIndex = Player->entindex();

		gMatchMenu[EntityIndex].Create(_T("Select an player to report with given reason"), true, (void*)this->MenuHandle);

		for (auto const& Target : Players)
		{
			if (!gMatchAdmin.Access(Target->entindex(), ADMIN_IMMUNITY) && (Player->entindex() != Target->entindex()))
			{
				gMatchMenu[EntityIndex].AddItem(Player->entindex(), STRING(Target->edict()->v.netname), false, Target->m_iTeam);
			}
		}

		this->m_Reason[EntityIndex] = Report;

		gMatchMenu[EntityIndex].Show(EntityIndex);

		gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Select an player to report with reason: %s."), Report.c_str());

		return;
	}

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("The report system can't detect players on server."), MINIMUM_REPORT_REASON_LENGTH);
}

void CMatchReport::MenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		auto Target = UTIL_PlayerByIndexSafe(Item.Info);

		if (Target)
		{
			/**/
		}
	}
}