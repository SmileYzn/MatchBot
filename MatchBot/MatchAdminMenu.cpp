#include "precompiled.h"

CMatchAdminMenu gMatchAdminMenu;

// Main Adminnistrator menu
void CMatchAdminMenu::MainMenu(int EntityIndex)
{
	// Get Admin Flags
	auto Flags = gMatchAdmin.GetFlags(EntityIndex);

	// If do not have an access to main menu
	if (!(Flags & ADMIN_MENU))
	{
		gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	// Menu
	gMatchMenu[EntityIndex].Create(_T("Match BOT Menu"), true, (void*)this->MainMenuHandle);

	// Kick Option
	gMatchMenu[EntityIndex].AddItem(0, _T("Kick Player"), !(Flags & ADMIN_KICK));

	// Ban Player
	gMatchMenu[EntityIndex].AddItem(1, _T("Ban Player"), !(Flags & ADMIN_BAN));

	// Kill Player
	gMatchMenu[EntityIndex].AddItem(2, _T("Slay Player"), !(Flags & ADMIN_SLAY));

	// Change Player Team
	gMatchMenu[EntityIndex].AddItem(3, _T("Team Player"), !(Flags & ADMIN_LEVEL_A));

	// Change map
	gMatchMenu[EntityIndex].AddItem(4, _T("Change Map"), !(Flags & ADMIN_MAP));

	// Control Match BOT
	gMatchMenu[EntityIndex].AddItem(5, _T("Match Control"), !(Flags & ADMIN_MENU));

	// Send global chat message
	gMatchMenu[EntityIndex].AddItem(6, _T("Send Message"), !(Flags & ADMIN_CHAT));

	// Send server rcon command
	gMatchMenu[EntityIndex].AddItem(7, _T("Send Command"), !(Flags & ADMIN_RCON));

	// Swap teams instantly
	gMatchMenu[EntityIndex].AddItem(8, _T("Swap Teams"), !(Flags & ADMIN_CVAR));

	// Player List
	gMatchMenu[EntityIndex].AddItem(9, _T("Player List"), !(Flags & ADMIN_MENU));

	// Console Variables Menu
	gMatchMenu[EntityIndex].AddItem(10, _T("Cvars Menu"), !(Flags & ADMIN_CVAR));

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
		if (Item.Disabled)
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
			
				g_engfuncs.pfnClientCommand(Player->edict(), pCmdFormat);
				break;
			}
			case 7: // Send rcon command
			{
			    char pCmdFormat[] = "messagemode !cmd\n";
			
				g_engfuncs.pfnClientCommand(Player->edict(), pCmdFormat);
				break;
			}
			case 8: // Swap Teams
			{
				gMatchAdminMenu.SwapTeams(Player->entindex());
				break;
			}
			case 9: // Player List
			{
				gMatchPlayer.PlayerMenu(Player);
				break;
			}
			case 10: // Cvars Menu
			{
				gMatchCvarMenu.Menu(Player);
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
			gMatchUtil.ServerCommand("kick #%d", g_engfuncs.pfnGetPlayerUserId(Target->edict()));

			LOG_MESSAGE
			(
				PLID,
				"\"%s<%i><%s><%s>\" kick \"%s<%i><%s><%s>\"",
				STRING(Player->edict()->v.netname),
				g_engfuncs.pfnGetPlayerUserId(Player->edict()),
				g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
				gMatchBot.GetTeam(Player->m_iTeam, true),
				STRING(Target->edict()->v.netname),
				g_engfuncs.pfnGetPlayerUserId(Target->edict()),
				g_engfuncs.pfnGetPlayerAuthId(Target->edict()),
				gMatchBot.GetTeam(Target->m_iTeam, true)
			);
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
				gMatchMenu[EntityIndex].Create(gMatchUtil.FormatString(_T("Choose a time to ban: ^w%s^y"), STRING(Target->edict()->v.netname)), true, (void*)gMatchAdminMenu.BanMenuHandle);

				std::vector<time_t> BanTimes = { 0, 5, 10, 15, 30, 60, 120, 240, 480, 960, 1440, 10080, 43200 };

				for (auto const& Time : BanTimes)
				{
					gMatchMenu[EntityIndex].AddItem(Target->entindex(), CTimeFormat::GetTimeLength(Time, CTimeFormat::TIMEUNIT_MINUTES), false, Time);
				}

				gMatchMenu[EntityIndex].Show(EntityIndex);
			}
			else
			{
				gMatchAdminMenu.BanMenu(EntityIndex);

				gMatchUtil.ServerCommand(_T("banid %d #%d kick"), Item.Extra, g_engfuncs.pfnGetPlayerUserId(Target->edict()));

				gMatchUtil.ServerCommand("wait;wait;writeid;writeip");

				LOG_MESSAGE
				(
					PLID,
					"\"%s<%i><%s><%s>\" banned \"%s<%i><%s><%s>\" for %d min(s)",
					STRING(Player->edict()->v.netname),
					g_engfuncs.pfnGetPlayerUserId(Player->edict()),
					g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
					gMatchBot.GetTeam(Player->m_iTeam, true),
					STRING(Target->edict()->v.netname),
					g_engfuncs.pfnGetPlayerUserId(Target->edict()),
					g_engfuncs.pfnGetPlayerAuthId(Target->edict()),
					gMatchBot.GetTeam(Target->m_iTeam, true),
					Item.Extra
				);
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

			LOG_MESSAGE
			(
				PLID,
				"\"%s<%i><%s><%s>\" slays \"%s<%i><%s><%s>\"",
				STRING(Player->edict()->v.netname),
				g_engfuncs.pfnGetPlayerUserId(Player->edict()),
				g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
				gMatchBot.GetTeam(Player->m_iTeam, true),
				STRING(Target->edict()->v.netname),
				g_engfuncs.pfnGetPlayerUserId(Target->edict()),
				g_engfuncs.pfnGetPlayerAuthId(Target->edict()),
				gMatchBot.GetTeam(Target->m_iTeam, true)
			);
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
				gMatchMenu[EntityIndex].Create(gMatchUtil.FormatString(_T("Set ^w%s^y Team"), STRING(Target->edict()->v.netname)), true, (void*)gMatchAdminMenu.TeamMenuHandle);

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

				LOG_MESSAGE
				(
					PLID,
					"\"%s<%i><%s><%s>\" set team \"%s<%i><%s><%s>\"",
					STRING(Player->edict()->v.netname),
					g_engfuncs.pfnGetPlayerUserId(Player->edict()),
					g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
					gMatchBot.GetTeam(Player->m_iTeam, true),
					STRING(Target->edict()->v.netname),
					g_engfuncs.pfnGetPlayerUserId(Target->edict()),
					g_engfuncs.pfnGetPlayerAuthId(Target->edict()),
					gMatchBot.GetTeam(Target->m_iTeam, true)
				);
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

		for (auto const& Map : MapList)
		{
			gMatchMenu[EntityIndex].AddItem(Map.first, Map.second, !Map.second.compare(STRING(gpGlobals->mapname)), Map.first);
		}

		gMatchMenu[EntityIndex].Show(EntityIndex);
	}
}

void CMatchAdminMenu::MapMenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		g_engfuncs.pfnCvar_DirectSet(gMatchBot.m_VoteMap, "0");

		gMatchChangeMap.ChangeMap(Item.Text, 5.0f, true);

		gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 changed map to: ^4%s^1..."), STRING(Player->edict()->v.netname), Item.Text.c_str());

		LOG_MESSAGE
		(
			PLID,
			"\"%s<%i><%s><%s>\" changed map to %s",
			STRING(Player->edict()->v.netname),
			g_engfuncs.pfnGetPlayerUserId(Player->edict()),
			g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
			gMatchBot.GetTeam(Player->m_iTeam, true),
			Item.Text.c_str()
		);
	}
}

void CMatchAdminMenu::ControlMenu(int EntityIndex)
{
	auto Flags = gMatchAdmin.GetFlags(EntityIndex);

	if (!(Flags & ADMIN_MENU))
	{
		gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	auto State = gMatchBot.GetState();

	gMatchMenu[EntityIndex].Create(_T("Match Control"), true, (void*)this->ControlMenuHandle);

	gMatchMenu[EntityIndex].AddItem(0, _T("Run Vote Map"), (State == STATE_DEAD || State == STATE_START || State == STATE_END || !(Flags & ADMIN_VOTE)));

	gMatchMenu[EntityIndex].AddItem(1, _T("Run Vote Teams"), ((State != STATE_WARMUP) || !(Flags & ADMIN_VOTE)));

	gMatchMenu[EntityIndex].AddItem(2, (State == STATE_HALFTIME) ? _T("Continue Match") : _T("Start Match"), ((State != STATE_WARMUP && State != STATE_HALFTIME) || !(Flags & ADMIN_LEVEL_B)));

	gMatchMenu[EntityIndex].AddItem(3, _T("Stop Match"), ((State == STATE_DEAD || State == STATE_WARMUP || State == STATE_START || State == STATE_END) || !(Flags & ADMIN_LEVEL_B)));

	gMatchMenu[EntityIndex].AddItem(4, _T("Restart Match"), ((State != STATE_FIRST_HALF && State != STATE_SECOND_HALF && State != STATE_OVERTIME) || !(Flags & ADMIN_LEVEL_B)));

	gMatchMenu[EntityIndex].AddItem(5, _T("Pause Match"), ((State == STATE_DEAD || State == STATE_WARMUP || State == STATE_START || State == STATE_HALFTIME || State == STATE_END) || !(Flags & ADMIN_LEVEL_B)));

	gMatchMenu[EntityIndex].AddItem(6, _T("Toggle Match BOT"), ((State != STATE_DEAD && State != STATE_WARMUP) || !(Flags & ADMIN_LEVEL_B)));

	gMatchMenu[EntityIndex].Show(EntityIndex);
}

void CMatchAdminMenu::ControlMenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		if (Item.Disabled)
		{
			gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
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

bool CMatchAdminMenu::Message(CBasePlayer* Player)
{
	auto pCmdArgs = g_engfuncs.pfnCmd_Args();

	if (pCmdArgs)
	{
		if (pCmdArgs[0u] != '\0')
		{
			if (!gMatchAdmin.Access(Player->entindex(), ADMIN_CHAT))
			{
				gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
				return false;
			}

			std::string Args = pCmdArgs;

			if (!Args.empty() && Args.length() > 2)
			{
				Args.erase(std::remove(Args.begin(), Args.end(), '\"'), Args.end());

				gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3(%s)^1: %s"), STRING(Player->edict()->v.netname), Args.c_str());

				LOG_MESSAGE
				(
					PLID,
					"\"%s<%i><%s><%s>\" message: %s",
					STRING(Player->edict()->v.netname),
					g_engfuncs.pfnGetPlayerUserId(Player->edict()),
					g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
					gMatchBot.GetTeam(Player->m_iTeam, true),
					Args.c_str()
				);

				return true;
			}
		}
	}

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Usage: !msg ^3<Text Message>^1"));

	return false;
}

bool CMatchAdminMenu::Rcon(CBasePlayer* Player)
{
	auto pCmdArgs = g_engfuncs.pfnCmd_Args();

	if (pCmdArgs)
	{
		if (pCmdArgs[0u] != '\0')
		{
			if (!gMatchAdmin.Access(Player->entindex(), ADMIN_RCON))
			{
				gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
				return false;
			}

			std::string Args = pCmdArgs;

			if (Args.find("rcon_password") != std::string::npos || Args.find("sv_password") != std::string::npos)
			{
				gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("These commands are protected here, only ^3Server^1 can use."));
				return false;
			}

			if (!Args.empty() && Args.length() > 2)
			{
				Args.erase(std::remove(Args.begin(), Args.end(), '\"'), Args.end());

				gMatchUtil.ServerCommand(Args.c_str());

				gMatchUtil.SayText(Player->edict(), Player->entindex(), _T("^3Executed^1: %s"), Args.c_str());

				LOG_MESSAGE
				(
					PLID,
					"\"%s<%i><%s><%s>\" server command: %s",
					STRING(Player->edict()->v.netname),
					g_engfuncs.pfnGetPlayerUserId(Player->edict()),
					g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
					gMatchBot.GetTeam(Player->m_iTeam, true),
					Args.c_str()
				);

				return true;
			}
		}
	}

	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Usage: !cmd ^3<Server Command>^1"));

	return false;
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
			gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 changed team sides manually."), STRING(Player->edict()->v.netname));

			LOG_MESSAGE
			(
				PLID,
				"\"%s<%i><%s><%s>\" changed team sides",
				STRING(Player->edict()->v.netname),
				g_engfuncs.pfnGetPlayerUserId(Player->edict()),
				g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
				gMatchBot.GetTeam(Player->m_iTeam, true)
			);
		}
		else
		{
			// Send message
			gMatchUtil.SayText(Player->edict(), Player->entindex(), _T("Can only swap teams during ^4%s^1 period."), gMatchBot.GetState(STATE_WARMUP));
		}
	}
}
