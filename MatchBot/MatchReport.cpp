#include "precompiled.h"

CMatchReport gMatchReport;

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
										gMatchMenu[EntityIndex].AddItem(Target->entindex(), STRING(Target->edict()->v.netname), false);
									}
								}
							}

							gMatchMenu[EntityIndex].Show(Player->entindex());

							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Select player to rerport with reason: \3%s."), this->m_Reason[EntityIndex].c_str());

							return true;
						}

						gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Server can't detect players connected."));

						return false;
					}
				}
			}

			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Usage: .report \3<Description of report>\1"));

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
	// Denúncia sobre %s enviada. ID da denúncia: %d.
	P_REPORT_DATA Report;

	Report.Player = GET_USER_AUTH(Player->edict());
	Report.PlayerTeam = Player->m_iTeam;

	Report.Target = GET_USER_AUTH(Target->edict());;
	Report.TargetTeam = Target->m_iTeam;

	Report.Time = time(0);

	Report.Reason = this->m_Reason[Player->entindex()];

	this->m_Data.push_back(Report);

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Report about \3%s\1 send. ID of report is \3#%d"), STRING(Target->edict()->v.netname), this->m_Data.size());
}
