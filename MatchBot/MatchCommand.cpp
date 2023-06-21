#include "precompiled.h"

CMatchCommand gMatchCommand;

// On Client command
bool CMatchCommand::ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1)
{
	// IF is not null
	if (!pcmd)
	{
		return false;
	}

	// If string is not empty
	if (pcmd[0u] == '\0')
	{
		return false;
	}

	// If is menuselect command
	if (!Q_strcmp(pcmd,"menuselect"))
	{
		// If has arguments
		if (parg1)
		{
			// If native CS menu is not being displayed
			if (Player->m_iMenu == Menu_OFF)
			{
				// Handle menu
				if (gMatchMenu[Player->entindex()].Handle(Player->entindex(), atoi(parg1)))
				{
					// Return result
					return true;
				}
			}
		}
	}
	// If player used say or say_team command
	else if (!Q_strcmp(pcmd,"say") || !Q_strcmp(pcmd,"say_team"))
	{
		// If has any argument
		if (parg1)
		{
			// Compare if is an admin or player prefix
			if (parg1[0u] == '!' || parg1[0u] == '.')
			{
				// Get arguments
				auto pCmdArgs = g_engfuncs.pfnCmd_Args();

				// If is not null
				if (pCmdArgs)
				{
					// If string is not empty
					if (pCmdArgs[0u] != '\0')
					{
						// Set command format
						char pCmdFormat[] = "%s\n";

						// Execute interal client command with arguments
						g_engfuncs.pfnClientCommand(Player->edict(), pCmdFormat, pCmdArgs);

						// Return results
						return true;
					}
				}
			}
		}
	}
	else
	{
		// Player Commands
		if (!Q_stricmp(pcmd,".status"))
		{
			gMatchBot.Status(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,".score"))
		{
			gMatchBot.Scores(Player, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,".ready"))
		{
			gMatchReady.Ready(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,".notready"))
		{
			gMatchReady.NotReady(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,".hp"))
		{
			gMatchStats.ShowHP(Player, true, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,".dmg"))
		{
			gMatchStats.ShowDamage(Player, true, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,".rdmg"))
		{
			gMatchStats.ShowReceivedDamage(Player, true, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,".sum"))
		{
			gMatchStats.ShowSummary(Player, true, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,".help"))
		{
			gMatchBot.Help(Player, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,".vote"))
		{
			gMatchVoteMenu.Menu(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,".votekick"))
		{
			gMatchVoteMenu.VoteKick(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,".votemap"))
		{
			gMatchVoteMenu.VoteMap(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,".votepause"))
		{
			gMatchVoteMenu.VotePause(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd, ".voterestart"))
		{
			gMatchVoteMenu.VoteRestart(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd, ".votestop"))
		{
			gMatchVoteMenu.VoteStop(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd, ".report"))
		{
			gMatchReport.Report(Player);
			return true;
		}
		// Admin Commands
		else if (!Q_stricmp(pcmd,"!menu") || !Q_stricmp(pcmd,"mb_menu"))
		{
			gMatchAdminMenu.MainMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"!kick") || !Q_stricmp(pcmd,"mb_kick_menu"))
		{
			gMatchAdminMenu.KickMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"!ban") || !Q_stricmp(pcmd,"mb_ban_menu"))
		{
			gMatchAdminMenu.BanMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"!kill") || !Q_stricmp(pcmd,"mb_kill_menu"))
		{
			gMatchAdminMenu.SlayMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"!team") || !Q_stricmp(pcmd,"mb_team_menu"))
		{
			gMatchAdminMenu.TeamMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"!map") || !Q_stricmp(pcmd,"mb_mapmenu"))
		{
			gMatchAdminMenu.MapMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"!control") || !Q_stricmp(pcmd,"mb_control_menu"))
		{
			gMatchAdminMenu.ControlMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"!msg") || !Q_stricmp(pcmd,"mb_message"))
		{
			gMatchAdminMenu.Message(Player);

			return true;
		}
		else if (!Q_stricmp(pcmd,"!cmd") || !Q_stricmp(pcmd,"mb_command"))
		{
			gMatchAdminMenu.Rcon(Player);

			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Usage: !cmd \3<Server Command>\1"));
			
			return true;
		}
		else if (!Q_stricmp(pcmd,"!swap") || !Q_stricmp(pcmd,"mb_swap_teams"))
		{
			gMatchAdminMenu.SwapTeams(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"!votemap") || !Q_stricmp(pcmd,"mb_start_vote_map"))
		{
			gMatchBot.StartVoteMap(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"!voteteam") || !Q_stricmp(pcmd,"mb_start_vote_team"))
		{
			gMatchBot.StartVoteTeam(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"!start") || !Q_stricmp(pcmd,"mb_start_match"))
		{
			gMatchBot.StartMatch(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"!stop") || !Q_stricmp(pcmd,"mb_stop_match"))
		{
			gMatchBot.StopMatch(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"!restart") || !Q_stricmp(pcmd,"mb_restart_match"))
		{
			gMatchBot.RestartMatch(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"!pause") || !Q_stricmp(pcmd,"mb_pause_match"))
		{
			gMatchPause.Init(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"!help") || !Q_stricmp(pcmd,"mb_help"))
		{
			gMatchBot.Help(Player, true);
			return true;
		}
	}

	// Return result
	return false;
}

