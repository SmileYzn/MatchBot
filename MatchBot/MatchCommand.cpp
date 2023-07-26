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
		// Player Commands
		if (!Q_stricmp(pcmd,"%sstatus",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchBot.Status(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%sscore",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchBot.Scores(Player, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%sready",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchReady.Ready(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%snotready",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchReady.NotReady(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%shp",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchStats.ShowHP(Player, true, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%sdmg",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchStats.ShowDamage(Player, true, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%srdmg",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchStats.ShowReceivedDamage(Player, true, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%ssum",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchStats.ShowSummary(Player, true, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%shelp",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchBot.Help(Player, false);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%svote",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchVoteMenu.Menu(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%svotekick",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchVoteMenu.VoteKick(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%svotemap",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchVoteMenu.VoteMap(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%svotepause",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchVoteMenu.VotePause(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%svoterestart",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchVoteMenu.VoteRestart(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%svotestop",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchVoteMenu.VoteStop(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%sreport",gMatchBot.m_PlayerPrefix->string))
		{
			gMatchReport.Menu(Player);
			return true;
		}
		// Admin Commands
		else if (!Q_stricmp(pcmd,"%smenu",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_menu"))
		{
			gMatchAdminMenu.MainMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"%skick",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_kick_menu"))
		{
			gMatchAdminMenu.KickMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"%sban",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_ban_menu"))
		{
			gMatchAdminMenu.BanMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"%skill",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_kill_menu"))
		{
			gMatchAdminMenu.SlayMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"%steam",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_team_menu"))
		{
			gMatchAdminMenu.TeamMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"%smap",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_mapmenu"))
		{
			gMatchAdminMenu.MapMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"%scontrol",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_control_menu"))
		{
			gMatchAdminMenu.ControlMenu(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"%smsg",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_message"))
		{
			gMatchAdminMenu.Message(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%scmd",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_command"))
		{
			gMatchAdminMenu.Rcon(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%sswap",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_swap_teams"))
		{
			gMatchAdminMenu.SwapTeams(Player->entindex());
			return true;
		}
		else if (!Q_stricmp(pcmd,"%svotemap",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_start_vote_map"))
		{
			gMatchBot.StartVoteMap(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%svoteteam",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_start_vote_team"))
		{
			gMatchBot.StartVoteTeam(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%sstart",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_start_match"))
		{
			gMatchBot.StartMatch(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%sstop",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_stop_match"))
		{
			gMatchBot.StopMatch(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%srestart",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_restart_match"))
		{
			gMatchBot.RestartMatch(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%spause",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_pause_match"))
		{
			gMatchPause.Init(Player);
			return true;
		}
		else if (!Q_stricmp(pcmd,"%shelp",gMatchBot.m_AdminPrefix->string) || !Q_stricmp(pcmd,"mb_help"))
		{
			gMatchBot.Help(Player, true);
			return true;
		}
	}

	// Return result
	return false;
}

