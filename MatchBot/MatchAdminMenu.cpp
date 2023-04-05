#include "precompiled.h"

CMatchAdminMenu gMatchAdminMenu;

// Main Adminnistrator menu
void CMatchAdminMenu::MainMenu(int EntityIndex)
{
	// If do not have an access to menu
	if (!gMatchAdmin.Access(EntityIndex, ADMIN_MENU))
	{
		gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	// Menu
	gMatchMenu[EntityIndex].Create(_T("Match BOT Menu"), true, (void*)this->MainMenuHandle);

	// Kick Option
	gMatchMenu[EntityIndex].AddItem(0, _T("Kick Player"), !gMatchAdmin.Access(EntityIndex, ADMIN_KICK), ADMIN_KICK);

	// Ban Player
	gMatchMenu[EntityIndex].AddItem(1, _T("Ban Player"), !gMatchAdmin.Access(EntityIndex, ADMIN_BAN), ADMIN_BAN);

	// Kill Player
	gMatchMenu[EntityIndex].AddItem(2, _T("Slay Player"), !gMatchAdmin.Access(EntityIndex, ADMIN_SLAY), ADMIN_SLAY);

	// Change Player Team
	gMatchMenu[EntityIndex].AddItem(3, _T("Team Player"), !gMatchAdmin.Access(EntityIndex, ADMIN_LEVEL_A), ADMIN_LEVEL_A);

	// Change map
	gMatchMenu[EntityIndex].AddItem(4, _T("Change Map"), !gMatchAdmin.Access(EntityIndex, ADMIN_MAP), ADMIN_MAP);

	// Control Match BOT
	gMatchMenu[EntityIndex].AddItem(5, _T("Match Control"), !gMatchAdmin.Access(EntityIndex, ADMIN_MENU), ADMIN_MENU);

	// Send global chat message
	gMatchMenu[EntityIndex].AddItem(6, _T("Send Message"), !gMatchAdmin.Access(EntityIndex, ADMIN_CHAT), ADMIN_CHAT);

	// Send server rcon command
	gMatchMenu[EntityIndex].AddItem(7, _T("Send Command"), !gMatchAdmin.Access(EntityIndex, ADMIN_RCON), ADMIN_RCON);

	// Swap teams instantly
	gMatchMenu[EntityIndex].AddItem(8, _T("Swap Teams"), !gMatchAdmin.Access(EntityIndex, ADMIN_CVAR), ADMIN_CVAR);

	// Show menu 
	gMatchMenu[EntityIndex].Show(EntityIndex);
}

// Handle main menu
void CMatchAdminMenu::MainMenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	// Get Player
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	// IF player
	if (Player)
	{
		// If has access to that option
		if (!gMatchAdmin.Access(Player->entindex(), Item.Extra))
		{
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command.")); 
			return;
		}
		
		// Check what is option
		switch (Item.Info)
		{
			case 0: // Kick Player
			{
				gMatchAdminMenu.KickMenu(Player->entindex());
				break;
			}
			case 1: // Ban Player
			{
				gMatchAdminMenu.BanMenu(Player->entindex());
				break;
			}
			case 2: // Kill Player
			{
				gMatchAdminMenu.SlayMenu(Player->entindex());
				break;
			}
			case 3: // Team
			{
				gMatchAdminMenu.TeamMenu(Player->entindex());
				break;
			}
			case 4: // Change Map
			{
				gMatchAdminMenu.MapMenu(Player->entindex());
				break;
			}
			case 5: // Match BOT Control
			{
				gMatchAdminMenu.ControlMenu(Player->entindex());
				break;
			}
			case 6: // Send message
			{
			    char pCmdFormat[] = "messagemode !msg\n";
			
				CLIENT_COMMAND(Player->edict(), pCmdFormat);
				break;
			}
			case 7: // Send rcon command
			{
			    char pCmdFormat[] = "messagemode !cmd\n";
			
				CLIENT_COMMAND(Player->edict(), pCmdFormat);
				break;
			}
			case 8: // Swap Teams
			{
				gMatchAdminMenu.SwapTeams(Player->entindex());
				break;
			}
		}
	}
}

// Kick Menu
void CMatchAdminMenu::KickMenu(int EntityIndex)
{
	// If do not have access
	if (!gMatchAdmin.Access(EntityIndex, ADMIN_KICK))
	{
		gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	gMatchMenu[EntityIndex].Create(_T("Kick Player"), true, (void*)this->KickMenuHandle);

	auto Players = gMatchUtil.GetPlayers(false, true);

	for(auto const & Player : Players)
	{
		if (!gMatchAdmin.Access(Player->entindex(), ADMIN_IMMUNITY))
		{
			gMatchMenu[EntityIndex].AddItem(Player->entindex(), STRING(Player->edict()->v.netname));
		}
	}

	gMatchMenu[EntityIndex].Show(EntityIndex);
}

void CMatchAdminMenu::KickMenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		auto Target = UTIL_PlayerByIndexSafe(Item.Info);

		if (Target)
		{
			gMatchUtil.ServerCommand("kick #%d", GETPLAYERUSERID(Target->edict()));
		}
	}

	gMatchAdminMenu.KickMenu(EntityIndex);
}

void CMatchAdminMenu::BanMenu(int EntityIndex)
{
	// If do not have access
	if (!gMatchAdmin.Access(EntityIndex, ADMIN_BAN))
	{
		gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	// Create menu
	gMatchMenu[EntityIndex].Create(_T("Ban Player"), true, (void*)this->BanMenuHandle);

	// Get players
	auto Players = gMatchUtil.GetPlayers(false, true);

	// Loop Players
	for (auto const& Player : Players)
	{
		// If is not admin immunity
		if (!gMatchAdmin.Access(Player->entindex(), ADMIN_IMMUNITY))
		{
			// Add Player to menu
			gMatchMenu[EntityIndex].AddItem(Player->entindex(), STRING(Player->edict()->v.netname), false, -1);
		}
	}

	// Show menu
	gMatchMenu[EntityIndex].Show(EntityIndex);
}

void CMatchAdminMenu::BanMenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		auto Target = UTIL_PlayerByIndexSafe(Item.Info);

		if (Target)
		{
			if (Item.Extra < 0)
			{
				gMatchMenu[EntityIndex].Create(gMatchUtil.FormatString(_T("Choose a time to ban: \\w%s\\y"), STRING(Target->edict()->v.netname)), true, (void*)gMatchAdminMenu.BanMenuHandle);

				std::vector<time_t> BanTimes = { 0, 5, 10, 15, 30, 60, 120, 240, 480, 960, 1440, 10080, 43200 };

				for (auto const& Time : BanTimes)
				{
					gMatchMenu[EntityIndex].AddItem(Target->entindex(), CTimeFormat::GetTimeLength(Time, CTimeFormat::TIMEUNIT_MINUTES), false, Time);
				}

				gMatchMenu[EntityIndex].Show(EntityIndex);
			}
			else
			{
				gMatchUtil.ServerCommand("banid %d #%d kick;wait;writeid", Item.Extra, GETPLAYERUSERID(Target->edict()));

				gMatchAdminMenu.BanMenu(EntityIndex);
			}
		}
	}
}

void CMatchAdminMenu::SlayMenu(int EntityIndex)
{
	// If do not have access
	if (!gMatchAdmin.Access(EntityIndex, ADMIN_SLAY))
	{
		gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	gMatchMenu[EntityIndex].Create(_T("Slay Player"), true, (void*)this->SlayMenuHandle);

	auto Players = gMatchUtil.GetPlayers(false, true);

	for (auto const& Player : Players)
	{
		if (!gMatchAdmin.Access(Player->entindex(), ADMIN_IMMUNITY))
		{
			if (Player->IsAlive())
			{
				gMatchMenu[EntityIndex].AddItem(Player->entindex(), STRING(Player->edict()->v.netname));
			}
		}
	}

	gMatchMenu[EntityIndex].Show(EntityIndex);
}

void CMatchAdminMenu::SlayMenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		auto Target = UTIL_PlayerByIndexSafe(Item.Info);

		if (Target)
		{
			MDLL_ClientKill(Target->edict());
		}
	}

	gMatchAdminMenu.SlayMenu(EntityIndex);
}

void CMatchAdminMenu::TeamMenu(int EntityIndex)
{
	// If do not have access
	if (!gMatchAdmin.Access(EntityIndex, ADMIN_LEVEL_A))
	{
		gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	gMatchMenu[EntityIndex].Create(_T("Change Team"), true, (void*)this->TeamMenuHandle);

	auto Players = gMatchUtil.GetPlayers(false, true);

	for (auto const& Player : Players)
	{
		if (!gMatchAdmin.Access(Player->entindex(), ADMIN_IMMUNITY))
		{
			if (Player->m_iTeam != UNASSIGNED)
			{
				gMatchMenu[EntityIndex].AddItem(Player->entindex(), gMatchUtil.FormatString("%s \\R\\y%s", STRING(Player->edict()->v.netname), gMatchBot.GetTeam(Player->m_iTeam, true)), false, (int)UNASSIGNED);
			}
		}
	}

	gMatchMenu[EntityIndex].Show(EntityIndex);
}

void CMatchAdminMenu::TeamMenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		auto Target = UTIL_PlayerByIndexSafe(Item.Info);

		if (Target)
		{
			if (Item.Extra == UNASSIGNED)
			{
				gMatchMenu[EntityIndex].Create(gMatchUtil.FormatString(_T("Set \\w%s\\y Team"), STRING(Target->edict()->v.netname)), true, (void*)gMatchAdminMenu.TeamMenuHandle);

				gMatchMenu[EntityIndex].AddItem(Target->entindex(), gMatchBot.GetTeam(TERRORIST, false), Target->m_iTeam == TERRORIST, TERRORIST);

				gMatchMenu[EntityIndex].AddItem(Target->entindex(), gMatchBot.GetTeam(CT, false), Target->m_iTeam == CT, CT);

				gMatchMenu[EntityIndex].AddItem(Target->entindex(), gMatchBot.GetTeam(SPECTATOR, false), Target->m_iTeam == SPECTATOR, SPECTATOR);

				gMatchMenu[EntityIndex].Show(EntityIndex);

				return;
			}
			else
			{
				if (Item.Extra == SPECTATOR)
				{
					if (Target->IsAlive())
					{
						Player->edict()->v.deadflag = DEAD_DEAD;
					}
				}

				Target->CSPlayer()->JoinTeam((TeamName)Item.Extra);
			}
		}
	}

	gMatchAdminMenu.TeamMenu(EntityIndex);
}

void CMatchAdminMenu::MapMenu(int EntityIndex)
{
	if (!gMatchAdmin.Access(EntityIndex, ADMIN_MAP))
	{
		gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	auto MapList = gMatchUtil.GetMapList(true);

	if (!MapList.empty())
	{
		gMatchMenu[EntityIndex].Create(_T("Change Map"), true, (void*)this->MapMenuHandle);

		for (auto const& row : MapList)
		{
			gMatchMenu[EntityIndex].AddItem(row.first, row.second, !row.second.compare(STRING(gpGlobals->mapname)), row.first);
		}

		gMatchMenu[EntityIndex].Show(EntityIndex);
	}
}

void CMatchAdminMenu::MapMenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		gMatchChangeMap.ChangeMap(Item.Text, 5.0f, true);

		gMatchUtil.SayText(nullptr, Player->entindex(), _T("\3%s\1 changed map to: \4%s\1..."), STRING(Player->edict()->v.netname), Item.Text.c_str());
	}
}

void CMatchAdminMenu::ControlMenu(int EntityIndex)
{
	if (!gMatchAdmin.Access(EntityIndex, ADMIN_MENU))
	{
		gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	auto State = gMatchBot.GetState();

	auto AccessVote = gMatchAdmin.Access(EntityIndex, ADMIN_VOTE);

	auto AccessControl = gMatchAdmin.Access(EntityIndex, ADMIN_LEVEL_B);

	gMatchMenu[EntityIndex].Create(_T("Match Control"), true, (void*)this->ControlMenuHandle);

	gMatchMenu[EntityIndex].AddItem(0, _T("Run Vote Map"), (State == STATE_DEAD || State == STATE_START || State == STATE_END || !AccessVote));

	gMatchMenu[EntityIndex].AddItem(1, _T("Run Vote Teams"), ((State != STATE_WARMUP) || !AccessVote));

	gMatchMenu[EntityIndex].AddItem(2, (State == STATE_HALFTIME) ? _T("Continue Match") : _T("Start Match"), ((State != STATE_WARMUP && State != STATE_HALFTIME) || !AccessControl));

	gMatchMenu[EntityIndex].AddItem(3, _T("Stop Match"), ((State == STATE_DEAD || State == STATE_WARMUP || State == STATE_START || State == STATE_END) || !AccessControl));

	gMatchMenu[EntityIndex].AddItem(4, _T("Restart Match"), ((State != STATE_FIRST_HALF && State != STATE_SECOND_HALF && State != STATE_OVERTIME) || !AccessControl));

	gMatchMenu[EntityIndex].AddItem(5, _T("Pause Match"), ((State == STATE_DEAD || State == STATE_WARMUP || State == STATE_START || State == STATE_HALFTIME || State == STATE_END) || !AccessControl));

	gMatchMenu[EntityIndex].AddItem(6, _T("Toggle Match BOT"), ((State != STATE_DEAD && State != STATE_WARMUP) || !AccessControl));

	gMatchMenu[EntityIndex].Show(EntityIndex);
}

void CMatchAdminMenu::ControlMenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		if (Item.Disabled)
		{
			gMatchAdminMenu.ControlMenu(EntityIndex);
			return;
		}

		switch (Item.Info)
		{
			case 0: // Vote Map
			{
				gMatchBot.StartVoteMap(Player);
				break;
			}
			case 1: // Vote Team
			{
				gMatchBot.StartVoteTeam(Player);
				break;
			}
			case 2: // Start / Continue Match
			{
				gMatchBot.StartMatch(Player);
				break;
			}
			case 3: // Stop Match
			{
				gMatchBot.StopMatch(Player);
				break;
			}
			case 4: // Restart Match
			{
				gMatchBot.RestartMatch(Player);
				break;
			}
			case 5: // Pause Match
			{
				gMatchPause.Init(Player);
				break;
			}
			case 6: // Enable / Disable Match BOT
			{
				if (gMatchBot.GetState() == STATE_WARMUP)
				{
					gMatchBot.Disable();
				}
				else
				{
					gMatchBot.Enable();
				}

				break;
			}
		}
	}
}

void CMatchAdminMenu::Message(int EntityIndex, std::string Args)
{
	auto Player = UTIL_PlayerByIndex(EntityIndex);

	if (Player)
	{
		if (!gMatchAdmin.Access(EntityIndex, ADMIN_CHAT))
		{
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
			return;
		}

		if (!Args.empty())
		{
			Args.erase(std::remove(Args.begin(), Args.end(), '\"'), Args.end());

			gMatchUtil.SayText(nullptr, Player->entindex(), _T("\3(%s)\1: %s"), STRING(Player->edict()->v.netname), Args.c_str());
		}
		else
		{
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Usage: !msg \3<Text Message>\1"));
		}
	}
}

void CMatchAdminMenu::Rcon(int EntityIndex, std::string Args)
{
	auto Player = UTIL_PlayerByIndex(EntityIndex);

	if (Player)
	{
		if (!gMatchAdmin.Access(EntityIndex, ADMIN_RCON))
		{
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
			return;
		}

		if (Args.find("rcon_password") != std::string::npos || Args.find("sv_password") != std::string::npos)
		{
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("These commands are protected here, only \3Server\1 can use."));
			return;
		}

		if (!Args.empty())
		{
			Args.erase(std::remove(Args.begin(), Args.end(), '\"'), Args.end());

			gMatchUtil.ServerCommand(Args.c_str());

			gMatchUtil.SayText(Player->edict(), Player->entindex(), _T("\3Executed\1: %s"), Args.c_str());
		}
		else
		{
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Usage: !cmd \3<Server Command>\1"));
		}
	}
}

void CMatchAdminMenu::SwapTeams(int EntityIndex)
{
	// Get Player
	auto Player = UTIL_PlayerByIndex(EntityIndex);

	// If found
	if (Player)
	{
		// If has access to swap teams
		if (!gMatchAdmin.Access(EntityIndex, ADMIN_CVAR))
		{
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
			return;
		}

		// If is in Warmup state
		if (gMatchBot.GetState() == STATE_WARMUP)
		{
			// Swap Teams and restart round
			if (g_pGameRules)
			{
				CSGameRules()->SwapAllPlayers();

				CSGameRules()->RestartRound();
			}

			// Send message
			gMatchUtil.SayText(nullptr, Player->entindex(), _T("\3%s\1 changed team sides manually."), STRING(Player->edict()->v.netname));
		}
		else
		{
			// Send message
			gMatchUtil.SayText(Player->edict(), Player->entindex(), _T("Can only swap teams during \4%s\1 period."), gMatchBot.GetState(STATE_WARMUP));
		}
	}
}
