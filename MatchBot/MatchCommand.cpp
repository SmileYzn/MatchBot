#include "precompiled.h"

CMatchCommand gMatchCommand;

// On Server Activate
void CMatchCommand::ServerActivate()
{
	try
	{
		// File stream
		std::ifstream fp(MB_COMMANDS_FILE, std::ios::in);

		// If file is open
		if (fp)
		{
			// Reset pointer
			fp.clear();

			// Go to begin of file
			fp.seekg(0, std::ios::beg);

			// Read data from json file
			nlohmann::ordered_json Json = nlohmann::ordered_json::parse(fp, nullptr, true, true);

			// Loop
			for (auto & row : Json.items())
			{
				// Get info
				auto Info = row.value();

				// If has id
				if (Info.contains("id") && !Info["id"].empty())
				{
					// If has flag
					if (Info.contains("flag") && !Info["flag"].empty())
					{
						P_COMMAND_INFO Command = { 0 };

						Command.Index = Info["id"].get<int>();

						Command.Flag = gMatchAdmin.ReadFlags(std::string(Info["flag"]).c_str());

						Command.Description = std::string(Info["description"]);

						this->m_Data.insert(std::make_pair(row.key(), Command));
					}
				}
			}

			// Close file
			fp.close();
		}
		else
		{
			LOG_CONSOLE(PLID, "[%s] Failed to open file: %s", __func__, MB_COMMANDS_FILE);
		}
	}
	catch (nlohmann::json::parse_error& e)
	{
		LOG_CONSOLE(PLID, "[%s] %s", __func__, e.what());
	}
}

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
		// Get Command
		auto const Command = this->m_Data.find(pcmd);

		// If command was found
		if (Command != this->m_Data.end())
		{
			// Get Admin Flags
			auto Flags = gMatchAdmin.GetFlags(Player->entindex());

			// If do not have an access to main menu
			if (!(Flags & Command->second.Flag))
			{
				gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
				return true;
			}

			LOG_CONSOLE(PLID, "[%s] Flags: %d Flag: %d",__func__,Flags,Command->second.Flag);

			// Switch of command index
			switch (Command->second.Index)
			{
				case CMD_PLAYER_STATUS:
				{
					gMatchBot.Status(Player);
					break;
				}
				case CMD_PLAYER_SCORE:
				{
					gMatchBot.Scores(Player, false);
					break;
				}
				case CMD_PLAYER_READY:
				{
					gMatchReady.Ready(Player);
					break;
				}
				case CMD_PLAYER_NOTREADY:
				{
					gMatchReady.NotReady(Player);
					break;
				}
				case CMD_PLAYER_HP:
				{
					gMatchStats.ShowHP(Player, true, false);
					break;
				}
				case CMD_PLAYER_DMG:
				{
					gMatchStats.ShowDamage(Player, true, false);
					break;
				}
				case CMD_PLAYER_RDMG:
				{
					gMatchStats.ShowReceivedDamage(Player, true, false);
					break;
				}
				case CMD_PLAYER_SUM:
				{
					gMatchStats.ShowSummary(Player, true, false);
					break;
				}
				case CMD_PLAYER_HELP:
				{
					gMatchBot.Help(Player, false);
					break;
				}
				case CMD_PLAYER_VOTE:
				{
					gMatchVoteMenu.Menu(Player);
					break;
				}
				case CMD_PLAYER_VOTE_KICK:
				{
					gMatchVoteMenu.VoteKick(Player);
					break;
				}
				case CMD_PLAYER_VOTE_MAP:
				{
					gMatchVoteMenu.VoteMap(Player);
					break;
				}
				case CMD_PLAYER_VOTE_PAUSE:
				{
					gMatchVoteMenu.VotePause(Player);
					break;
				}
				case CMD_PLAYER_VOTE_RESTART:
				{
					gMatchVoteMenu.VoteRestart(Player);
					break;
				}
				case CMD_PLAYER_VOTE_STOP:
				{
					gMatchVoteMenu.VoteStop(Player);
					break;
				}
				case CMD_PLAYER_REPORT:
				{
					gMatchReport.Menu(Player);
					break;
				}
				case CMD_ADMIN_MENU:
				{
					gMatchAdminMenu.MainMenu(Player->entindex());
					break;
				}
				case CMD_ADMIN_KICK:
				{
					gMatchAdminMenu.KickMenu(Player->entindex());
					break;
				}
				case CMD_ADMIN_BAN:
				{
					gMatchAdminMenu.BanMenu(Player->entindex());
					break;
				}
				case CMD_ADMIN_KILL:
				{
					gMatchAdminMenu.SlayMenu(Player->entindex());
					break;
				}
				case CMD_ADMIN_TEAM:
				{
					gMatchAdminMenu.TeamMenu(Player->entindex());
					break;
				}
				case CMD_ADMIN_MAP:
				{
					gMatchAdminMenu.MapMenu(Player->entindex());
					break;
				}
				case CMD_ADMIN_CONTROL:
				{
					gMatchAdminMenu.ControlMenu(Player->entindex());
					break;
				}
				case CMD_ADMIN_MESSAGE:
				{
					gMatchAdminMenu.Message(Player);
					break;
				}
				case CMD_ADMIN_COMMAND:
				{
					gMatchAdminMenu.Rcon(Player);
					break;
				}
				case CMD_ADMIN_SWAP:
				{
					gMatchAdminMenu.SwapTeams(Player->entindex());
					break;
				}
				case CMD_ADMIN_VOTE_MAP:
				{
					gMatchBot.StartVoteMap(Player);
					break;
				}
				case CMD_ADMIN_VOTE_TEAM:
				{
					gMatchBot.StartVoteTeam(Player);
					break;
				}
				case CMD_ADMIN_START_MATCH:
				{
					gMatchBot.StartMatch(Player);
					break;
				}
				case CMD_ADMIN_STOP_MATCH:
				{
					gMatchBot.StopMatch(Player);
					break;
				}
				case CMD_ADMIN_RESTART_MATCH:
				{
					gMatchBot.RestartMatch(Player);
					break;
				}
				case CMD_ADMIN_PAUSE_MATCH:
				{
					gMatchPause.Init(Player);
					break;
				}
				case CMD_ADMIN_HELP:
				{
					gMatchBot.Help(Player, true);
					break;
				}
			}

			// Return result
			return true;
		}
	}

	// Return result
	return false;
}