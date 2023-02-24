#include "precompiled.h"

CMatchCommand gMatchCommand;

// Match BOT Commands Hash
enum MatchCommandConst_ : std::uint32_t
{
	// Basic Stuff
	Command_Say = Hash::GetConst("say"),
	Command_SayTeam = Hash::GetConst("say_team"),
	Command_MenuSelect = Hash::GetConst("menuselect"),

	// Player Commands
	Command_Player_Status = Hash::GetConst(".status"),
	Command_Player_Score = Hash::GetConst(".score"),
	Command_Player_Ready = Hash::GetConst(".ready"),
	Command_Player_NotReady = Hash::GetConst(".notready"),
	Command_Player_HP = Hash::GetConst(".hp"),
	Command_Player_Dmg = Hash::GetConst(".dmg"),
	Command_Player_DmgReceived = Hash::GetConst(".rdmg"),
	Command_Player_Summary = Hash::GetConst(".sum"),
	Command_Player_Help = Hash::GetConst(".help"),
	Command_Player_Vote = Hash::GetConst(".vote"),
	Command_Player_VoteKick = Hash::GetConst(".votekick"),
	Command_Player_VoteMap = Hash::GetConst(".votemap"),
	Command_Player_VotePause = Hash::GetConst(".votepause"),
	Command_Player_VoteRestart = Hash::GetConst(".voterestart"),

	// Admin Commands
	Command_Admin_Menu = Hash::GetConst("!menu"),
	Command_Admin_Kick = Hash::GetConst("!kick"),
	Command_Admin_Ban = Hash::GetConst("!ban"),
	Command_Admin_Kill = Hash::GetConst("!kill"),
	Command_Admin_Team = Hash::GetConst("!team"),
	Command_Admin_Map = Hash::GetConst("!map"),
	Command_Admin_Control = Hash::GetConst("!control"),
	Command_Admin_Msg = Hash::GetConst("!msg"),
	Command_Admin_Cmd = Hash::GetConst("!cmd"),
	Command_Admin_Swap = Hash::GetConst("!swap"),
	Command_Admin_VoteMap = Hash::GetConst("!votemap"),
	Command_Admin_VoteTeam = Hash::GetConst("!voteteam"),
	Command_Admin_Start = Hash::GetConst("!start"),
	Command_Admin_Stop = Hash::GetConst("!stop"),
	Command_Admin_Restart = Hash::GetConst("!restart"),
	Command_Admin_Pause = Hash::GetConst("!pause"),
	Command_Admin_Help = Hash::GetConst("!help"),

	// Admin Console Commands
	Command_Console_Menu = Hash::GetConst("mb_menu"),
	Command_Console_Kick = Hash::GetConst("mb_kick_menu"),
	Command_Console_Ban = Hash::GetConst("mb_ban_menu"),
	Command_Console_Kill = Hash::GetConst("mb_kill_menu"),
	Command_Console_Team = Hash::GetConst("mb_team_menu"),
	Command_Console_Map = Hash::GetConst("mb_map_menu"),
	Command_Console_Control = Hash::GetConst("mb_control_menu"),
	Command_Console_Msg = Hash::GetConst("mb_message"),
	Command_Console_Cmd = Hash::GetConst("mb_command"),
	Command_Console_Swap = Hash::GetConst("mb_swap_teams"),
	Command_Console_VoteMap = Hash::GetConst("mb_vote_map_menu"),
	Command_Console_VoteTeam = Hash::GetConst("mb_vote_team_menu"),
	Command_Console_Start = Hash::GetConst("mb_start_match"),
	Command_Console_Stop = Hash::GetConst("mb_stop_match"),
	Command_Console_Restart = Hash::GetConst("mb_restart_match"),
	Command_Console_Pause = Hash::GetConst("mb_pause_match"),
	Command_Console_Help = Hash::GetConst("mb_help"),
};

bool CMatchCommand::ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1)
{
	if (!pcmd)
	{
		return false;
	}

	if (pcmd[0u] == '\0')
	{
		return false;
	}

	const auto CommandHash{ Hash::Get(pcmd) };

	if (CommandHash == Command_MenuSelect)
	{
		if (parg1)
		{
			if (Player->m_iMenu == Menu_OFF)
			{
				if (gMatchMenu[Player->entindex()].Handle(Player->entindex(), atoi(parg1)))
				{
					return true;
				}
			}
		}
	}
	else if (CommandHash == Command_Say || CommandHash == Command_SayTeam)
	{
		if (parg1)
		{
			const bool HasServerCommand{ parg1[0u] == '!' };
			const bool HasClientCommand{ parg1[0u] == '.' };

			if (HasServerCommand || HasClientCommand)
			{
				if (auto pCmdArgs{ CMD_ARGS() })
				{
                    char pCmdFormat[] = "%s\n";
				    
					CLIENT_COMMAND(Player->edict(), pCmdFormat, pCmdArgs);
					return true;
				}
			}
		}
	}
	else
	{
		switch(CommandHash)
		{
			// Player Commands
			case Command_Player_Status:
			{
				gMatchBot.Status(Player);
				return true;
			}
			case Command_Player_Score:
			{
				gMatchBot.Scores(Player, false);
				return true;
			}
			case Command_Player_Ready:
			{
				gMatchReady.Ready(Player);
				return true;
			}
			case Command_Player_NotReady:
			{
				gMatchReady.NotReady(Player);
				return true;
			}
			case Command_Player_HP:
			{
				gMatchStats.ShowHP(Player);
				return true;
			}
			case Command_Player_Dmg:
			{
				gMatchStats.ShowDamage(Player);
				return true;
			}
			case Command_Player_DmgReceived:
			{
				gMatchStats.ShowReceivedDamage(Player);
				return true;
			}
			case Command_Player_Summary:
			{
				gMatchStats.ShowSummary(Player);
				return true;
			}
			case Command_Player_Help:
			{
				gMatchBot.Help(Player, false);
				return true;
			}
			case Command_Player_Vote:
			{
				gMatchVoteMenu.Menu(Player);
				return true;
			}
			case Command_Player_VoteKick:
			{
				gMatchVoteMenu.VoteKick(Player);
				return true;
			}
			case Command_Player_VoteMap:
			{
				gMatchVoteMenu.VoteMap(Player);
				return true;
			}
			case Command_Player_VotePause:
			{
				gMatchVoteMenu.VotePause(Player);
				break;
			}
			case Command_Player_VoteRestart:
			{
				gMatchVoteMenu.VoteRestart(Player);
				break;
			}
			// Admin Commands
			case Command_Admin_Menu:
			case Command_Console_Menu:
			{
				gMatchAdminMenu.MainMenu(Player->entindex());
				return true;
			}
			case Command_Admin_Kick:
			case Command_Console_Kick:
			{
				gMatchAdminMenu.KickMenu(Player->entindex());
				return true;
			}
			case Command_Admin_Ban:
			case Command_Console_Ban:
			{
				gMatchAdminMenu.BanMenu(Player->entindex());
				return true;
			}
			case Command_Admin_Kill:
			case Command_Console_Kill:
			{
				gMatchAdminMenu.SlayMenu(Player->entindex());
				return true;
			}
			case Command_Admin_Team:
			case Command_Console_Team:
			{
				gMatchAdminMenu.TeamMenu(Player->entindex());
				return true;
			}
			case Command_Admin_Map:
			case Command_Console_Map:
			{
				gMatchAdminMenu.MapMenu(Player->entindex());
				return true;
			}
			case Command_Admin_Control:
			case Command_Console_Control:
			{
				gMatchAdminMenu.ControlMenu(Player->entindex());
				return true;
			}
			case Command_Admin_Msg:
			case Command_Console_Msg:
			{
				gMatchAdminMenu.Message(Player->entindex(), CMD_ARGS());
				return true;
			}
			case Command_Admin_Cmd:
			case Command_Console_Cmd:
			{
				gMatchAdminMenu.Rcon(Player->entindex(), CMD_ARGS());
				return true;
			}
			case Command_Admin_Swap:
			case Command_Console_Swap:
			{
				gMatchAdminMenu.SwapTeams(Player->entindex());
				return true;
			}
			case Command_Admin_VoteMap:
			case Command_Console_VoteMap:
			{
				gMatchBot.StartVoteMap(Player);
				return true;
			}
			case Command_Admin_VoteTeam:
			case Command_Console_VoteTeam:
			{
				gMatchBot.StartVoteTeam(Player);
				return true;
			}
			case Command_Admin_Start:
			case Command_Console_Start:
			{
				gMatchBot.StartMatch(Player);
				return true;
			}
			case Command_Admin_Stop:
			case Command_Console_Stop:
			{
				gMatchBot.StopMatch(Player);
				return true;
			}
			case Command_Admin_Restart:
			case Command_Console_Restart:
			{
				gMatchBot.RestartMatch(Player);
				return true;
			}
			case Command_Admin_Pause:
			case Command_Console_Pause:
			{
				gMatchPause.Init(Player);
				return true;
			}
			case Command_Admin_Help:
			case Command_Console_Help:
			{
				gMatchBot.Help(Player, true);
				return true;
			}
		}
	}

	return false;
}

