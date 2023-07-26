#include "precompiled.h"

CMatchReport gMatchReport;

void CMatchReport::ServerActivate()
{
	this->m_Data.clear();

	this->m_Reason.clear();
}

std::vector<P_REPORT_DATA> CMatchReport::GetData()
{
	return this->m_Data;
}

bool CMatchReport::Menu(CBasePlayer* Player)
{
	if (Player->m_iTeam == TERRORIST || Player->m_iTeam == CT)
	{
		if (!((BIT(STATE_DEAD) | BIT(STATE_START) | BIT(STATE_END)) & BIT(gMatchBot.GetState())))
		{
			auto pCmdArgs = g_engfuncs.pfnCmd_Args();

			if (pCmdArgs)
			{
				if (pCmdArgs[0u] != '\0')
				{
					auto EntityIndex = Player->entindex();

					this->m_Reason[EntityIndex] = pCmdArgs;

					if (this->m_Reason[EntityIndex].length() >= REPORT_REASON_MININUM_LENGTH)
					{
						this->m_Reason[EntityIndex].erase(std::remove(this->m_Reason[EntityIndex].begin(), this->m_Reason[EntityIndex].end(), '\n'), this->m_Reason[EntityIndex].end());
						this->m_Reason[EntityIndex].erase(std::remove(this->m_Reason[EntityIndex].begin(), this->m_Reason[EntityIndex].end(), '\"'), this->m_Reason[EntityIndex].end());

						auto Players = gMatchUtil.GetPlayers(false, true);

						if (Players.size() > 1)
						{
							gMatchMenu[EntityIndex].Create(_T("Select player to report:"), true, (void*)this->MenuHandle);

							for (auto const& Target : Players)
							{
								if (EntityIndex != Target->entindex())
								{
									if (!gMatchAdmin.Access(Target->entindex(), ADMIN_IMMUNITY))
									{
										gMatchMenu[EntityIndex].AddItem(Target->entindex(), gMatchUtil.FormatString("%s \\R\\y%s", STRING(Target->edict()->v.netname), gMatchBot.GetTeam(Target->m_iTeam, true)), false);
									}
								}
							}

							gMatchMenu[EntityIndex].Show(Player->entindex());

							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Select player to rerport with reason: ^3%s."), this->m_Reason[EntityIndex].c_str());

							return true;
						}

						gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Server can't detect players connected."));

						return false;
					}

					gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("The reason must be at least %d characters long."), REPORT_REASON_MININUM_LENGTH);

					return false;
				}
			}

			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Usage: %sreport ^3<Description of report>^1"), gMatchBot.m_PlayerPrefix->string);

			return false;
		}
	}

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));

	return false;
}

void CMatchReport::MenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		auto Target = UTIL_PlayerByIndexSafe(Item.Info);

		if (Target)
		{
			gMatchReport.Insert(Player, Target);
		}
	}
}

void CMatchReport::Insert(CBasePlayer* Player, CBasePlayer* Target)
{
	P_REPORT_DATA Report;

	Report.Player = GET_USER_AUTH(Player->edict());
	Report.PlayerTeam = Player->m_iTeam;

	Report.Target = GET_USER_AUTH(Target->edict());;
	Report.TargetTeam = Target->m_iTeam;

	Report.Reason = this->m_Reason[Player->entindex()];

	Report.MatchState = gMatchBot.GetState();

	Report.Time = time(0);

	this->m_Data.push_back(Report);
	
	char Time[32] = { 0 };

	strftime(Time, sizeof(Time), "%D %R", localtime(&Report.Time));

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Report about ^3%s^1 sent at %s."), STRING(Target->edict()->v.netname), Time);
	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Administrators will review report and take an action if needed."));
}
