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
			if (parg1[0u] == gMatchBot.m_AdminPrefix->string[0u] || parg1[0u] == gMatchBot.m_PlayerPrefix->string[0u])
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

		//Player commands
		if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "status")) {
        
			gMatchBot.Status(Player);
			return true;

    	}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "score"))
		{
			gMatchBot.Scores(Player, false);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "ready"))
		{
			gMatchReady.Ready(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "notready"))
		{
			gMatchReady.NotReady(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "hp"))
		{
			gMatchStats.ShowHP(Player, true, false);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "dmg"))
		{
			gMatchStats.ShowDamage(Player, true, false);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "rdmg"))
		{
			gMatchStats.ShowReceivedDamage(Player, true, false);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "sum"))
		{
			gMatchStats.ShowSummary(Player, true, false);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "help"))
		{
			gMatchBot.Help(Player, false);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "vote"))
		{
			gMatchVoteMenu.Menu(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "votekick"))
		{
			gMatchVoteMenu.VoteKick(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "votemap"))
		{
			gMatchVoteMenu.VoteMap(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "votepause"))
		{
			gMatchVoteMenu.VotePause(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "voterestart"))
		{
			gMatchVoteMenu.VoteRestart(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "votestop"))
		{
			gMatchVoteMenu.VoteStop(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_PlayerPrefix->string, "report"))
		{
			gMatchReport.Menu(Player);
			return true;
		}
		// Admin Commands
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "menu") || !Q_stricmp(pcmd,"mb_menu"))
		{
			gMatchAdminMenu.MainMenu(Player->entindex());
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "kick") || !Q_stricmp(pcmd,"mb_kick_menu"))
		{
			gMatchAdminMenu.KickMenu(Player->entindex());
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "ban") || !Q_stricmp(pcmd,"mb_ban_menu"))
		{
			gMatchAdminMenu.BanMenu(Player->entindex());
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "kick") || !Q_stricmp(pcmd,"mb_kill_menu"))
		{
			gMatchAdminMenu.SlayMenu(Player->entindex());
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "team") || !Q_stricmp(pcmd,"mb_team_menu"))
		{
			gMatchAdminMenu.TeamMenu(Player->entindex());
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "map") || !Q_stricmp(pcmd,"mb_mapmenu"))
		{
			gMatchAdminMenu.MapMenu(Player->entindex());
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "control") || !Q_stricmp(pcmd,"mb_control_menu"))
		{
			gMatchAdminMenu.ControlMenu(Player->entindex());
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "msg") || !Q_stricmp(pcmd,"mb_message"))
		{
			gMatchAdminMenu.Message(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "cmd") || !Q_stricmp(pcmd,"mb_command"))
		{
			gMatchAdminMenu.Rcon(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "swap") || !Q_stricmp(pcmd,"mb_swap_teams"))
		{
			gMatchAdminMenu.SwapTeams(Player->entindex());
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "votemap") || !Q_stricmp(pcmd,"mb_start_vote_map"))
		{
			gMatchBot.StartVoteMap(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "voteteam") || !Q_stricmp(pcmd,"mb_start_vote_team"))
		{
			gMatchBot.StartVoteTeam(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "start") || !Q_stricmp(pcmd,"mb_start_match"))
		{
			gMatchBot.StartMatch(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "stop") || !Q_stricmp(pcmd,"mb_stop_match"))
		{
			gMatchBot.StopMatch(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "restart") || !Q_stricmp(pcmd,"mb_restart_match"))
		{
			gMatchBot.RestartMatch(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "pause") || !Q_stricmp(pcmd,"mb_pause_match"))
		{
			gMatchPause.Init(Player);
			return true;
		}
		else if (CompareCommand(pcmd, gMatchBot.m_AdminPrefix->string, "help") || !Q_stricmp(pcmd,"mb_help"))
		{
			gMatchBot.Help(Player, true);
			return true;
		}
	}

	// Return result
	return false;
}

bool CMatchCommand::CompareCommand(const char* pcmd, const char* prefix, const char* mbCommand) {
    // Combina el prefijo y la parte variable en una cadena completa
    std::string fullCommand = std::string(prefix) + mbCommand;

    // Realiza la comparación con pcmd usando strcmp
    return (std::strcmp(pcmd, fullCommand.c_str()) == 0);
}