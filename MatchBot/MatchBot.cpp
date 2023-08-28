#include "precompiled.h"

CMatchBot gMatchBot;

// Server activate
void CMatchBot::ServerActivate()
{
	// Match BOT is dead
	this->m_State = STATE_DEAD;

	// Match BOT Log Tag
	this->m_MatchTag = gMatchUtil.CvarRegister("mb_log_tag", "BOT");

	// Server Language
	this->m_Language = gMatchUtil.CvarRegister("mb_language", "en");

	// Admin commands prefix (Default !)
	this->m_AdminPrefix = gMatchUtil.CvarRegister("mb_admin_prefix", "!");

	// Player commands prefix (Default .)
	this->m_PlayerPrefix = gMatchUtil.CvarRegister("m_player_prefix", ".");

	// Minimum players in game to start match
	this->m_PlayersMin = gMatchUtil.CvarRegister("mb_players_min", "10");

	// Maximum allowed players in game
	this->m_PlayersMax = gMatchUtil.CvarRegister("mb_players_max", "10");

	// Rounds to play before overtime
	this->m_PlayRounds = gMatchUtil.CvarRegister("mb_play_rounds", "30");

	// Round to play in overtime
	this->m_PlayRoundsOT = gMatchUtil.CvarRegister("mb_play_rounds_ot", "6");

	// Overtime Type (0 Sudden Death Round, 1 Play Overtime, 2 End Match Tied, 3 Users Vote If Tied)
	this->m_PlayMode = gMatchUtil.CvarRegister("mb_play_ot_mode", "3");

	// Ready System type (0 Disabled, 1 Ready System, 2 Ready Timer)
	this->m_ReadyType = gMatchUtil.CvarRegister("mb_ready_type", "1");

	// Ready System timer delay in seconds
	this->m_ReadyTime = gMatchUtil.CvarRegister("mb_ready_time", "60.0");

	// Team Pickup Type (-1 Enable vote, 0 Leaders, 1 Random, 2 None, 3 Skill Balanced, 4 Swap Teams, 5 Knife Round)
	this->m_TeamPickupType = gMatchUtil.CvarRegister("mb_team_pick_type", "-1");

	// Team Pickup Menu Options (a Leaders, b Random, c None, d Skill Balanced, e Swap Teams, f Knife Round)
	this->m_TeamPickMenu = gMatchUtil.CvarRegister("mb_team_pick_menu", "abcdef");

	// Vote percentage in Votes
	this->m_VotePercent = gMatchUtil.CvarRegister("mb_vote_percent", "0.70");

	// Vote Map (Not used at settings file)
	this->m_VoteMap = gMatchUtil.CvarRegister("mb_vote_map", "0");

	// Vote Map type (1 Vote Map, 2 Random Map)
	this->m_VoteMapType = gMatchUtil.CvarRegister("mb_vote_map_type", "1");

	// Start Vote Map at match end (0 Disabled, 1 Enabled, 2 Only when minimum players reached)
	this->m_VoteMapAuto = gMatchUtil.CvarRegister("mb_vote_map_auto", "2");

	// What to do if Vote Map fail (0 None, 1 Restart Vote Map, 2 Choose random map)
	this->m_VoteMapFail = gMatchUtil.CvarRegister("mb_vote_map_fail", "1");

	// Play Knife round to pick starting sides
	this->m_KnifeRound = gMatchUtil.CvarRegister("mb_knife_round", "0");

	// Scores display method (0 Continue match, 1 Show all teams and scores)
	this->m_ScoreType = gMatchUtil.CvarRegister("mb_score_type", "0");
	
	// Store team scores in scoreboard
	this->m_TeamScore = gMatchUtil.CvarRegister("mb_scoreboard_team", "1");

	// Store player scores in scoreboard
	this->m_PlayerScore = gMatchUtil.CvarRegister("mb_scoreboard_player", "1");

	// Display states and scores at game description
	this->m_GameName = gMatchUtil.CvarRegister("mb_gamename", "1");

	// Mininum of players to enable Vote Kick command for players (0 to disble command)
	this->m_PlayerVoteKick = gMatchUtil.CvarRegister("mb_player_vote_kick", "5");

	// Mininum of players to enable Vote Map command for players (0 to disble command)
	this->m_PlayerVoteMap = gMatchUtil.CvarRegister("mb_player_vote_map", "5");

	// Mininum of players to enable Vote Pause command for players (0 to disble command)
	this->m_PlayerVotePause = gMatchUtil.CvarRegister("mb_player_vote_pause", "5");

	// Mininum of players to enable Vote Restart command for players (0 to disble command)
	this->m_PlayerVoteRestart = gMatchUtil.CvarRegister("mb_player_vote_restart", "5");

	// Mininum of players to enable Vote Surrender command for players (0 to disble command)
	this->m_PlayerVoteSurrender = gMatchUtil.CvarRegister("mb_player_vote_surrender", "5");

	// Show round stats on end (1 Show round damage in chat, 2 Show round summary in chat, 3 Show round damage in console, 4 Show round summary in console)
	this->m_RoundEndStats = gMatchUtil.CvarRegister("mb_round_end_stats", "0");

	// Enabled round stats commands in chat (a .hp, b .dmg, c .rdmg, d .sum)
	this->m_StatsCommands = gMatchUtil.CvarRegister("mb_stats_commands", "abcd");

	// Check player count in teams in each round start, if lacking this count open vote to restart (The number of lacking players in each team, 0 to disable check)
	this->m_PlayersMinCheck = gMatchUtil.CvarRegister("mb_players_min_diff", "2");

	// Restricted Weapons by item index slot position (1 to block item, 0 to allow)
	this->m_RestrictWeapons = gMatchUtil.CvarRegister("mb_restrict_weapons", "000000000000000000000000000000000000000");

	// Extra Smokegranade explosion fix (0 to disable fix, or the number of extra smoke puffs)
	this->m_ExtraSmokeCount = gMatchUtil.CvarRegister("mb_extra_smoke_count", "2");

	// Amount of seconds to pause match (0 Disable, or number of seconds to pause the match)
	this->m_PauseTime = gMatchUtil.CvarRegister("mb_pause_time", "60.0");

	// Anti reconnect mode (0 Disabled, 1 Enable when player explicity drop from server, 2 Enabled for any disconnect reason)
	this->m_RetryMode = gMatchUtil.CvarRegister("mb_retry_mode", "0");

	// Anti reconnect time (Time in seconds to prevent the player reconnect to server)
	this->m_RetryTime = gMatchUtil.CvarRegister("mb_retry_time", "30.0");

	// Users Help File or Website url (Without HTTPS)
	this->m_HelpFile = gMatchUtil.CvarRegister("mb_help_file", "cstrike/addons/matchbot/users_help.html");

	// Admin Help File or Website url (Without HTTPS)
	this->m_HelpFileAdmin = gMatchUtil.CvarRegister("mb_help_file_admin", "cstrike/addons/matchbot/admin_help.html");

	// Match Bot main config
	this->m_Config[STATE_DEAD] = gMatchUtil.CvarRegister("mb_cfg_match_bot", "matchbot.cfg");

	// Warmup config
	this->m_Config[STATE_WARMUP] = gMatchUtil.CvarRegister("mb_cfg_warmup", "warmup.cfg");

	// Start config
	this->m_Config[STATE_START] = gMatchUtil.CvarRegister("mb_cfg_start", "start.cfg");

	// First Half config
	this->m_Config[STATE_FIRST_HALF] = gMatchUtil.CvarRegister("mb_cfg_1st", "esl.cfg");

	// Half Time config
	this->m_Config[STATE_HALFTIME] = gMatchUtil.CvarRegister("mb_cfg_halftime", "halftime.cfg");

	// Second Half config
	this->m_Config[STATE_SECOND_HALF] = gMatchUtil.CvarRegister("mb_cfg_2nd", "esl.cfg");
	
	// Overtime config
	this->m_Config[STATE_OVERTIME] = gMatchUtil.CvarRegister("mb_cfg_overtime", "esl-ot.cfg");

	// End config
	this->m_Config[STATE_END] = gMatchUtil.CvarRegister("mb_cfg_end", "end.cfg");

	// Run Match Bot
	this->SetState(STATE_DEAD);

	// Load Language
	gMatchLanguage.Load();
}

// On server deactivate
void CMatchBot::ServerDeactivate()
{
	// Stop Ready System
	gMatchReady.Stop(false);

	// Stop Timer System
	gMatchTimer.Stop(false);

	// If knife round is running
	if (this->m_PlayKnifeRound)
	{
		// Set Playing Knife Round to false
		this->m_PlayKnifeRound = false;

		// Disable BOT deathmatch
		g_engfuncs.pfnCVarSetFloat("bot_deathmatch", 0.0f);

		// Execute command to run with all weapons
		gMatchUtil.ServerCommand("bot_all_weapons");

		// Restore Map Objectives
		gMatchWarmup.RemoveMapObjective(false);
	}

	// Stop match if server is restarted
	if (this->m_State >= STATE_FIRST_HALF && this->m_State <= STATE_OVERTIME)
	{
		// End Match
		this->SetState(STATE_END);
	}
}

// Enable Match BOT if is dead
void CMatchBot::Enable()
{
	if (this->m_State == STATE_DEAD)
	{
		this->SetState(STATE_WARMUP);
	}
}

// Disable Match BOT on warmup
void CMatchBot::Disable()
{
	if (this->m_State == STATE_WARMUP)
	{
		// Stop Ready System
		gMatchReady.Stop(false);
		
		// Stop Timer System
		gMatchTimer.Stop(false);

		// Stop Warmup
		gMatchWarmup.Stop();

		// Kill Match BOT
		this->m_State = STATE_DEAD;
	}
}

// Static set next state
void CMatchBot::NextState(int State)
{
	// Set State (Static)
	gMatchBot.SetState(State);
}

// Get current state index
int CMatchBot::GetState()
{
	// Get Current State
	return this->m_State;
}

// Get current state name
const char* CMatchBot::GetState(int State)
{
	// Get Translated State Name
	return _T(MATCH_BOT_STATES[this->m_State]);
}

// Run a new match state
void CMatchBot::SetState(int State)
{
	// Set Current State
	this->m_State = State;

	switch (this->m_State)
	{
		// Match BOT is Dead, nothing is running
		case STATE_DEAD:
		{
			// Run next Warmup State
			gMatchTask.Create(TASK_CHANGE_STATE, 6.0f, false, (void*)this->NextState, STATE_WARMUP);
			break;
		}
		// Warmup State: Waiting for players until match starts
		case STATE_WARMUP:
		{
			// Display message when starts
			gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Starting ^4%s^1, Get Ready!"), this->GetState(this->m_State));

			// Start Warmup things
			gMatchWarmup.Init();

			// If ready type is 1
			if (this->m_ReadyType->value == 1)
			{
				// Use Ready System
				gMatchReady.Init(this->m_PlayersMin->value);
			}
			// If ready type is 2
			else if (this->m_ReadyType->value > 1)
			{
				// Use Timer System
				gMatchTimer.Init(this->m_PlayersMin->value, this->m_ReadyTime->value);
			}

			break;
		}
		// Start Vote Map or Teams Vote
		case STATE_START:
		{
			// If has votemap
			if (this->m_VoteMap->value)
			{
				// Execute Vote Map
				gMatchVoteMap.Init(this->m_VoteMapType->value, this->m_VoteMapFail->value);

				// Remove Vote Map Variable
				g_engfuncs.pfnCvar_DirectSet(this->m_VoteMap, "0");
			}
			else
			{
				// Init Team pickup methods
				gMatchVoteTeam.Init(this->m_TeamPickupType->value, this->m_PlayersMin->value);

				// Enable votemap to next map
				g_engfuncs.pfnCvar_DirectSet(this->m_VoteMap, "1");
			}

			// If has Knife Round variable
			if (this->m_KnifeRound)
			{
				// If Knife Round is set
				if (this->m_KnifeRound->value)
				{
					// Set to play knife round on first half
					this->m_PlayKnifeRound = true;
				}
			}

			break;
		}
		// Match Is Live: First Half
		case STATE_FIRST_HALF:
		{
			// Stop warmup things
			gMatchWarmup.Stop();

			// Clear Scores
			memset(this->m_Score, 0, sizeof(this->m_Score));

			// Clear OT Scores
			memset(this->m_ScoreOT, 0, sizeof(this->m_ScoreOT));

			// If is set to play knife round
			if (this->m_PlayKnifeRound)
			{
				// Remove map objectives
				gMatchWarmup.RemoveMapObjective(true);

				// Set BOT to deathmatch
				g_engfuncs.pfnCVarSetFloat("bot_deathmatch", 1.0f);

				// Execute command to run only with knives
				gMatchUtil.ServerCommand("bot_knives_only");

				// Send message
				gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Starting ^4Knife Round^1, Get Ready!"));
			}
			else
			{
				// Add map objectives
				gMatchWarmup.RemoveMapObjective(false);

				// Disable BOT deathmatch
				g_engfuncs.pfnCVarSetFloat("bot_deathmatch", 0.0f);

				// Execute command to run with all weapons
				gMatchUtil.ServerCommand("bot_all_weapons");

				// Send message
				gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Starting ^4%s^1, Get Ready!"), this->GetState(this->m_State));
			}

			// Start LO3 script
			gMatchLO3.Start();

			break;
		}
		// Half Time: Match is paused to swap teams
		case STATE_HALFTIME:
		{
			// Swap team scores
			this->SwapScores();

			// Swap team sides
			gMatchTask.Create(TASK_SWAP_TEAMS, 3.0f, false, (void*)this->SwapTeams, 1);

			// If ready mode is enabled or timer system is enabled
			if (this->m_ReadyType->value)
			{
				// If has less players than minimum required
				if ((int)gMatchUtil.GetPlayers(true, true).size() < this->m_PlayersMin->value)
				{
					// Send message
					gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("^3%s^1 started, Get Ready!"), this->GetState(this->m_State));

					// Init Warmup things
					gMatchWarmup.Init();

					// If ready type is 1
					if (this->m_ReadyType->value == 1.0f)
					{
						// Init Ready System
						gMatchReady.Init(this->m_PlayersMin->value);
					}
					// If ready type is 2
					else if (this->m_ReadyType->value >= 2.0f)
					{
						// Init Timer System
						gMatchTimer.Init(this->m_PlayersMin->value, this->m_ReadyTime->value);
					}
				}
				else
				{
					// If match is not played in total of rounds
					if (this->GetRound() < this->m_PlayRounds->value)
					{
						// Play second Half
						gMatchTask.Create(TASK_CHANGE_STATE, 6.0f, false, (void*)this->NextState, STATE_SECOND_HALF);
					}
					else
					{
						// Play Overtime rounds
						gMatchTask.Create(TASK_CHANGE_STATE, 6.0f, false, (void*)this->NextState, STATE_OVERTIME);
					}
				}
			}
			else
			{
				// Send message
				gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("^3%s^1 started, Get Ready!"), this->GetState(this->m_State));

				gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Wait for an server ^3Administrator^1 restart the match!"), this->GetState(this->m_State));
			}

			break;
		}
		// Second Half: Match is running
		case STATE_SECOND_HALF:
		{
			// Stop warmup things
			gMatchWarmup.Stop();

			// Clear Terrorists Scores
			this->m_Score[TERRORIST][STATE_SECOND_HALF] = 0;

			// Clear CTs Scores
			this->m_Score[CT][STATE_SECOND_HALF] = 0;

			// Send messages
			gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Starting ^4%s^1, Get Ready!"), this->GetState(this->m_State));

			// Send scores
			this->Scores(nullptr, true);

			// LO3 Script
			gMatchLO3.Start();

			break;
		}
		// Overtime: Match is tied and trying to get a real winner
		case STATE_OVERTIME:
		{
			// Message
			gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Starting ^4%s^1, Get Ready!"), this->GetState(this->m_State));

			// Send Scores
			this->Scores(nullptr, true);

			// LO3 Script
			gMatchLO3.Start();

			break;
		}
		// Match ended: Send final scores
		case STATE_END:
		{
			// Send scores
			this->Scores(nullptr, false);

			// Next State, Warmup by default
			auto NextState = STATE_WARMUP;

			// Enable votemap
			g_engfuncs.pfnCvar_DirectSet(this->m_VoteMap, "1");

			// If has auto vote map on end
			if (this->m_VoteMapAuto)
			{
				// If is enabled with 1
				if (this->m_VoteMapAuto->value == 1.0f)
				{
					// In next state, start Vote Map
					NextState = STATE_START;
				}
				// If is 2.0f, execute only if has mininum of players
				else if (this->m_VoteMapAuto->value == 2.0f)
				{
					// Get player count
					auto Players = gMatchUtil.GetPlayers(true, true);

					// If reached minimum of players
					if (Players.size() > (size_t)(this->m_PlayersMin->value / 2.0f))
					{
						// In next state, start Vote Map
						NextState = STATE_START;
					}
				}
			}

			// Set next state, match needed to run again
			gMatchTask.Create(TASK_CHANGE_STATE, 6.0f, false, (void*)this->NextState, NextState);

			break;
		}
	}

	// Log
	LOG_MESSAGE(PLID, "%s state: %s", Plugin_info.name, this->GetState(this->m_State));

	// Match Stats
	gMatchStats.SetState(this->m_State, this->m_PlayKnifeRound);

	// Match Scores Game Name
	this->UpdateGameName();

	// If is not null
	if(this->m_Config[this->m_State])
	{
		// If string is not null
		if(this->m_Config[this->m_State]->string)
		{
			// If cvar string is not empty
			if (this->m_Config[this->m_State]->string[0] != '\0')
			{
				// Execute config file
				gMatchUtil.ServerCommand("exec addons/matchbot/cfg/%s", this->m_Config[this->m_State]->string);
			}
		}
	}
}

// Get Final score for a team
int CMatchBot::GetScore(TeamName Team)
{
	return (this->m_Score[Team][STATE_FIRST_HALF] + this->m_Score[Team][STATE_SECOND_HALF] + this->m_Score[Team][STATE_OVERTIME]);
}

// Get Overtime score
int CMatchBot::GetScoreOT(TeamName Team)
{
	return this->m_ScoreOT[Team];
}

// Get the sum of rounds played by teams scores
int CMatchBot::GetRound()
{
	return this->GetScore(TERRORIST) + this->GetScore(CT);
}

// Return log tag to functions
const char* CMatchBot::GetTag()
{
	// If variable is not null
	if(this->m_MatchTag)
	{
		// If string is not null
		if (this->m_MatchTag->string)
		{
			// If string is not empty
			if (this->m_MatchTag->string[0] != '\0')
			{
				// Return string
				return this->m_MatchTag->string;
			}
		}
	}

	// Return default log tag
	return Plugin_info.logtag;
}

// Get team name
const char* CMatchBot::GetTeam(TeamName Team, bool ShortName)
{
	return ShortName ? _T(MATCH_BOT_TEAM_SHORT[Team]) : _T(MATCH_BOT_TEAM_STR[Team]);
}

// Swap scores function
void CMatchBot::SwapScores()
{
	// If we played more than maximum of rounds in match (We in Overtime)
	if (this->GetRound() >= this->m_PlayRounds->value)
	{
		// If scores are tied (This dettermine that OT is starting or restarting)
		if (this->GetScore(TERRORIST) == this->GetScore(CT))
		{
			// Reset Overtime scores (We are restarting OT)
			memset(this->m_ScoreOT, 0, sizeof(this->m_ScoreOT));

			// Do not swap teams or scores in first Overtime Half
			return;
		}
	}

	// Swap scores for each state
	for (auto State = STATE_DEAD; State < STATE_END; State++)
	{
		// Swap scores for each state
		SWAP(this->m_Score[TERRORIST][State], this->m_Score[CT][State]);
	}

	// Swap Overtime scores
	SWAP(this->m_ScoreOT[TERRORIST], this->m_ScoreOT[CT]);
}

// Swap teams function
void CMatchBot::SwapTeams(int ShowMessage)
{
	// Send message
	if (ShowMessage)
	{
		// Send message to all players
		gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Changing teams automatically."));
	}

	// Swap teams in game
	if (g_pGameRules)
	{
		CSGameRules()->SwapAllPlayers();
	}
}

// Get Knife Round Mode
bool CMatchBot::GetKnifeRound()
{
	return this->m_PlayKnifeRound;
}

// Set Knife Round Mode
void CMatchBot::SetKnifeRound(bool PlayKnifeRound)
{
	this->m_PlayKnifeRound = PlayKnifeRound;
}

// On Player Connect
bool CMatchBot::PlayerConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	// Get Entity Index
	auto EntityIndex = ENTINDEX(pEntity);

	// If we not allow spectaors
	if (!CVAR_GET_FLOAT("allow_spectators"))
	{
		// If player do not have reserved slots
		if (!gMatchAdmin.Access(EntityIndex, ADMIN_LEVEL_B))
		{
			// Get Player Count
			auto Players = gMatchUtil.GetPlayers(true, false);

			// If players in team reached maximum of players
			if (Players.size() >= this->m_PlayersMax->value)
			{
				// Reject connection: Server is Full
				Q_strcpy(szRejectReason, _T("Server is full."));

				// Return to engine
				return false;
			}
		}
	}

	// Return to engine allowing connection
	return true;
}

// When player try to join in a team, we do things
bool CMatchBot::PlayerJoinTeam(CBasePlayer* Player, int Slot)
{
	// If player not joined
	if (Player->m_iTeam == UNASSIGNED)
	{
		// If is not bot
		if (!Player->IsBot())
		{
			// Send team info, to enable colors in chat messages
			gMatchUtil.TeamInfo(Player->edict(), MAX_CLIENTS + TERRORIST + 1, "TERRORIST");
			gMatchUtil.TeamInfo(Player->edict(), MAX_CLIENTS + CT + 1, "CT");
		}
	}

	// Do not allow auto selection, this broken team systems
	if (Slot == MENU_SLOT_TEAM_RANDOM)
	{
		// If player is unassigned
		if (Player->m_iTeam != UNASSIGNED)
		{
			// Send message and block it
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("^3Auto Team Select^1 is not allowed."));

			// Block it
			return true;
		}
	}

	// If is spectator
	if (Slot == MENU_SLOT_TEAM_SPECT)
	{
		// If we not allow spectators
		if (!CVAR_GET_FLOAT("allow_spectators"))
		{
			// Send message and block it
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("^3%s^1 are not allowed."), this->GetTeam(SPECTATOR, false));

			// Block it
			return true;
		}
	}

	// If player is trying to choose same team, block to avoid suicide
	if (Player->m_iTeam == Slot)
	{
		if (Player->IsAlive())
		{
			// Send message
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You are already on the ^3%s^1 team."), this->GetTeam(Player->m_iTeam, false));

			// Block it
			return true;
		}
	}

	// If player is in Terrorists or CTs teams
	if (Player->m_iTeam == TERRORIST || Player->m_iTeam == CT)
	{
		// If match is running
		if (this->m_State >= STATE_START && this->m_State <= STATE_OVERTIME)
		{
			// Send message
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Cannot switch when the game is started."));

			// Block it, can't change teams while match is running
			return true;
		}
	}

	// If player is trying to join in Terrorists or CTs team while spectator or unnasigned
	if (Slot == MENU_SLOT_TEAM_TERRORIST || Slot == MENU_SLOT_TEAM_CT)
	{
		// Count player count in desired team, and check if is not full
		if (gMatchUtil.GetCount((TeamName)Slot) >= (int)(this->m_PlayersMax->value / 2.0f))
		{
			// Send message
			gMatchUtil.SayText(Player->edict(), (Slot == TERRORIST) ? PRINT_TEAM_RED : PRINT_TEAM_BLUE, _T("The ^3%s^1 team is complete."), this->GetTeam((TeamName)Slot, false));

			// Block it
			return true;
		}
	}

	// Allow any thing
	return false;
}

// While player get into game (Enter in any TERRORIST or CT Team)
void CMatchBot::PlayerGetIntoGame(CBasePlayer* Player)
{
	// If BOT is not dead
	if (this->m_State != STATE_DEAD)
	{
		// If is not BOT
		if (!Player->IsBot())
		{
			// Send messages
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("%s Build %s (^3%s^1)"), Plugin_info.name, Plugin_info.date, Plugin_info.author);
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("Say ^4.help^1 to view command list."));
		}
	}
}

// When player disconnect from game
void CMatchBot::PlayerDisconnect()
{
	// Get Player count
	auto PlayerCount = gMatchUtil.GetCount();

	// Minimum player count in each team
	auto MinimumCount = (int)((this->m_PlayersMin->value / 2.0f) - 1.0f);

	// If server was lacking minimum of players
	if (PlayerCount[TERRORIST] < MinimumCount || PlayerCount[CT] < MinimumCount)
	{
		// If match is running (LIVE)
		if (this->m_State >= STATE_FIRST_HALF && this->m_State <= STATE_OVERTIME)
		{
			// If server do not have an Spectator
			if (!PlayerCount[SPECTATOR])
			{
				// End match
				gMatchTask.Create(TASK_CHANGE_STATE, 2.0f, false, (void*)this->NextState, STATE_END);
			}
		}
	}
}

// Status command for players
void CMatchBot::Status(CBasePlayer* Player)
{
	// Send message
	gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("^3%s^1: Players %d, (%d Required of %d Allowed)"), this->GetState(this->m_State), gMatchUtil.GetPlayers(true, true).size(), (int)m_PlayersMin->value, (int)m_PlayersMax->value);

	// If match is running
	if (this->m_State >= STATE_FIRST_HALF && this->m_State <= STATE_END)
	{
		// Send scores to
		this->Scores(Player, true);
	}
}

// Player scores command
void CMatchBot::Scores(CBasePlayer* Player, bool Method)
{
	// Target player or all users
	edict_t* Target = Player ? Player->edict() : nullptr;

	// If match is running
	if (this->m_State >= STATE_FIRST_HALF && this->m_State <= STATE_END)
	{
		// Get default sender
		auto Sender = PRINT_TEAM_GREY;

		// Get score Terrorists
		int ScoreTR = this->GetScore(TERRORIST);

		// Get score CTs
		int ScoreCT = this->GetScore(CT);

		// If is not tied
		if (ScoreTR != ScoreCT)
		{
			// Set color based on winner team
			Sender = (ScoreTR > ScoreCT) ? PRINT_TEAM_RED : PRINT_TEAM_BLUE;
		}

		// Score method
		auto ShowMethod = (Method || (this->m_ScoreType && this->m_ScoreType->value));

		// Score Method 1: Show both scores
		if (ShowMethod)
		{
			// If match is ended
			if (this->m_State != STATE_END)
			{
				// Send to player or to all players if empty
				gMatchUtil.SayText(Target, Sender, _T("^3%s^1 (^4%d^1) - (^4%d^1) ^3%s^1"), this->GetTeam(TERRORIST, false), this->GetScore(TERRORIST), this->GetScore(CT), this->GetTeam(CT, false));
			}
			else
			{
				// Send to player or to all players if empty
				gMatchUtil.SayText(Target, Sender, _T("Game Over: ^3%s^1 (^4%d^1) - (^4%d^1) ^3%s^1"), this->GetTeam(TERRORIST, false), this->GetScore(TERRORIST), this->GetScore(CT), this->GetTeam(CT, false));
			}
		}
		else // Score Method 2: Show winner name and scores
		{
			// If match is not tied
			if (ScoreTR != ScoreCT)
			{
				// Get winner team
				auto Winner = (ScoreTR > ScoreCT) ? TERRORIST : CT;

				// Get other team
				auto Losers = (ScoreTR > ScoreCT) ? CT : TERRORIST;

				if (this->m_State != STATE_END)
				{
					// Send message
					gMatchUtil.SayText(Target, Sender, _T("The ^3%s^1 are winning: %d-%d"), this->GetTeam(Winner, false), this->GetScore(Winner), this->GetScore(Losers));
				}
				else
				{
					// Send message
					gMatchUtil.SayText(Target, Sender,(this->m_State == STATE_END) ? _T("Game Over! The ^3%s^1 have won the game: %d-%d") : _T("The ^3%s^1 are winning: %d-%d"),this->GetTeam(Winner, false),this->GetScore(Winner),this->GetScore(Losers));
				}
			}
			else
			{
				if (this->m_State != STATE_END)
				{
					// Send message of tied match
					gMatchUtil.SayText(Target, PRINT_TEAM_DEFAULT, _T("Score is tied: %d-%d"), ScoreTR, ScoreCT);
				}
				else
				{
					// Send message of tied match
					gMatchUtil.SayText(Target, PRINT_TEAM_DEFAULT, _T("Game Over! Score is tied: %d-%d"), ScoreTR, ScoreCT);
				}
			}
		}
	}
	else
	{
		gMatchUtil.SayText(Target, PRINT_TEAM_DEFAULT, _T("Unable to use this command now."));
	}
}

// View Admin / Users Help MOTD
void CMatchBot::Help(CBasePlayer* Player, bool AdminHelp)
{
	// Default Help File Path
	char Path[MAX_PATH] = "cstrike/addons/matchbot/users_help.html";

	// If has admin flag
	if (AdminHelp && gMatchAdmin.Access(Player->entindex(), ADMIN_LEVEL_C))
	{
		// If has admin help variable
		if (this->m_HelpFileAdmin)
		{
			// If string is not null
			if (this->m_HelpFileAdmin->string)
			{
				// Reset memory
				Q_memset(Path, 0, sizeof(Path));

				// Show motd
				Q_strcpy_s(Path, this->m_HelpFileAdmin->string);
			}
		}
	}
	else
	{
		// If has help variable
		if (this->m_HelpFile)
		{
			// If string is not null
			if (this->m_HelpFile->string)
			{
				// Reset memory
				Q_memset(Path, 0, sizeof(Path));

				// Show motd
				Q_strcpy_s(Path, this->m_HelpFile->string);
			}
		}
	}

	// Show motd
	gMatchUtil.ShowMotd(Player->edict(), Path, strlen(Path));
}

// On Round Start: After freezetime end
void CMatchBot::RoundStart()
{
	// If match is running
	if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
	{
		// Send scores on start
		this->Scores(nullptr, false);
	}
}

// On Round End: After a team win the round
void CMatchBot::RoundEnd(int winStatus, ScenarioEventEndRound event, float tmDelay)
{
	// If has a clear winner
	if (winStatus == WINSTATUS_CTS || winStatus == WINSTATUS_TERRORISTS)
	{
		// If match is running
		if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
		{
			// Get Winner Team
			TeamName Winner = (winStatus == WINSTATUS_TERRORISTS) ? TERRORIST : CT;

			// Add score to winner team
			this->m_Score[Winner][this->m_State]++;

			// Add OT score to winner team
			if (this->m_State == STATE_OVERTIME)
			{
				this->m_ScoreOT[Winner]++;
			}

			// Print a console message to all players
			gMatchUtil.ClientPrint(nullptr, PRINT_CONSOLE, _T("* Round %d won by: %s"), this->GetRound(), this->GetTeam(Winner, false));

			// Swich
			switch (this->m_State)
			{
				// First Half: Check for round count to swap teams
				case STATE_FIRST_HALF:
				{
					// If is set to play knife round
					if (this->m_PlayKnifeRound)
					{
						// Start Swap Teams Vote
						gMatchVoteSwapTeam.Init(Winner);
						
						// Disable Knife Round for first half state
						this->m_PlayKnifeRound = false;
					}

					// If sum of scores reached half of total rounds to play
					if (this->GetRound() >= (int)(this->m_PlayRounds->value / 2.0f))
					{
						// Change State
						gMatchTask.Create(TASK_CHANGE_STATE, tmDelay + 1.0f, false, (void*)this->NextState, STATE_HALFTIME);
					}

					break;
				}
				// Second Half: Check for a match winner
				case STATE_SECOND_HALF:
				{
					// Get total of rounds divided by 2
					auto Half = (int)(this->m_PlayRounds->value / 2.0f);

					// Terrorist Score
					auto ScoreTR = this->GetScore(TERRORIST);

					// CTs Score
					auto ScoreCT = this->GetScore(CT);

					// If any team reached Half rounds + 1, end match
					if (ScoreTR > Half || ScoreCT > Half)
					{
						gMatchTask.Create(TASK_CHANGE_STATE, tmDelay + 1.0f, false, (void*)this->NextState, STATE_END);
					}
					// If both teams reached half of total rounds, check for overtime
					else if ((ScoreTR == Half) && (ScoreCT == Half))
					{
						// Play Overtime directly (mb_play_ot_mode = 1)
						if (this->m_PlayMode->value == 1)
						{
							gMatchTask.Create(TASK_CHANGE_STATE, tmDelay + 1.0f, false, (void*)this->NextState, STATE_HALFTIME);
						}
						// End match tied (mb_play_ot_mode = 2)
						else if (this->m_PlayMode->value == 2)
						{
							gMatchTask.Create(TASK_CHANGE_STATE, tmDelay + 1.0f, false, (void*)this->NextState, STATE_END);
						}
						// Start Overtime vote (mb_play_ot_mode = 3)
						else if (this->m_PlayMode->value == 3)
						{
							// Init Vote for Overtime
							gMatchVoteOvertime.Init();
						}
					}

					break;
				}
				// Overtime case: Check for a Overtime winner
				case STATE_OVERTIME:
				{
					// Get total of overtime rounds divided by 2
					auto Half = (int)(this->m_PlayRoundsOT->value / 2.0f);

					// If any team reached required number of rounds to win
					if (this->GetScoreOT(TERRORIST) > Half || this->GetScoreOT(CT) > Half)
					{
						// End match with a winner
						gMatchTask.Create(TASK_CHANGE_STATE, tmDelay + 1.0f, false, (void*)this->NextState, STATE_END);
					}
					// Swap Teams for every half of OT rounds
					else if ((this->GetRound() % (int)(Half)) == 0)
					{
						// Swap Teams
						gMatchTask.Create(TASK_CHANGE_STATE, tmDelay + 1.0f, false, (void*)this->NextState, STATE_HALFTIME);
					}

					break;
				}
			}
		}

		// Match Scores Game Name
		this->UpdateGameName();
	}
}

// Round restart event
void CMatchBot::RoundRestart(bool PreRestart)
{
	// If has CSGameRules loaded
	if (g_pGameRules)
	{
		// If is live
		if (this->m_State >= STATE_HALFTIME)
		{
			// If store team scores in scoreboard is set
			if (this->m_TeamScore && this->m_TeamScore->value)
			{
				// If sv_restart is not set
				if (!CSGameRules()->m_bCompleteReset)
				{
					// Get number of CTs wins
					CSGameRules()->m_iNumCTWins = this->GetScore(CT);

					// Get number of TRs wins
					CSGameRules()->m_iNumTerroristWins = this->GetScore(TERRORIST);

					// Updade scoreboards
					CSGameRules()->UpdateTeamScores();
				}
			}

			// If is set to store player scores on scorebard after half time
			if (this->m_PlayerScore && this->m_PlayerScore->value)
			{
				// Get players
				auto Players = gMatchUtil.GetPlayers(true, true);

				// If is PRE sv_restart event
				if (PreRestart)
				{
					// Loop
					for (auto& Player : Players)
					{
						// Restore Frags
						Player->edict()->v.fuser4 = Player->edict()->v.frags;

						// Restore Deaths
						Player->edict()->v.iuser4 = Player->m_iDeaths;
					}
				}
				// If is POST sv_restart event
				else
				{
					// Loop
					for (auto& Player : Players)
					{
						// Restore Frags
						Player->edict()->v.frags = Player->edict()->v.fuser4;

						// Restore Deaths
						Player->m_iDeaths = Player->edict()->v.iuser4;

						// Update scoreboard
						Player->AddPoints(0, TRUE);
					}
				}
			}
		}
	}
}

// Update Game Name
void CMatchBot::UpdateGameName()
{
	// If has CSGameRules loaded
	if (g_pGameRules)
	{
		// Store original game description
		if (!this->m_GameDesc[0])
		{
			// Get default game name
			Q_strncpy(this->m_GameDesc, CSGameRules()->GetGameDescription(), sizeof(this->m_GameDesc));
		}

		// If is enabled
		if (this->m_GameName && this->m_GameName->value)
		{
			// Get match state
			auto State = gMatchBot.GetState();

			// If is not running, set default name
			if (State == STATE_DEAD)
			{
				// Restore default game name
				Q_strcpy_s(CSGameRules()->m_GameDesc, this->m_GameDesc);
			}
			else if (State == STATE_WARMUP || State == STATE_START)
			{
				// Set game name from state name
				Q_strcpy(CSGameRules()->m_GameDesc, gMatchBot.GetState(State));
			}
			else if (State >= STATE_FIRST_HALF && State <= STATE_END)
			{
				// Game Name
				char GameName[32] = { 0 };

				// Format game name with teams and scores
				Q_snprintf(GameName, sizeof(GameName), _T("%s %d : %d %s"), gMatchBot.GetTeam(TERRORIST, true), gMatchBot.GetScore(TERRORIST), gMatchBot.GetScore(CT), gMatchBot.GetTeam(CT, true));

				// Set to game description
				Q_strcpy_s(CSGameRules()->m_GameDesc, GameName);
			}
		}
		else
		{
			// Restore default game name
			Q_strcpy_s(CSGameRules()->m_GameDesc, this->m_GameDesc);
		}
	}
}

// Start vote map
void CMatchBot::StartVoteMap(CBasePlayer* Player)
{
	// If player is not null, command issued by an admin
	if (Player)
	{
		// Check Accesss
		if (gMatchAdmin.Access(Player->entindex(), ADMIN_VOTE))
		{
			// Check if we can run a vote in correct state: Warmup, First Half, Halftime, Second Half or Overtime
			if (this->m_State != STATE_DEAD && this->m_State != STATE_START && this->m_State != STATE_END)
			{
				// Stop Ready System 
				gMatchReady.Stop(false);

				// Stop Timer System
				gMatchTimer.Stop(false);

				// Enable vote map for this map session
				g_engfuncs.pfnCvar_DirectSet(this->m_VoteMap, "1");

				// Start Match
				this->SetState(STATE_START);

				LOG_MESSAGE
				(
					PLID,
					"\"%s<%i><%s><%s>\" started vote map",
					STRING(Player->edict()->v.netname),
					g_engfuncs.pfnGetPlayerUserId(Player->edict()),
					g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
					gMatchBot.GetTeam(Player->m_iTeam, true)
				);
			}
			else
			{
				// Message here
				gMatchUtil.SayText(Player->edict(), PRINT_TEAM_RED, _T("Cannot start an vote in ^3%s^1 state."), this->GetState(this->m_State));
			}
		}
		else
		{
			// Error message
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		}
	}
	else // Command issued by server, or Match BOT
	{
		// Enable vote map
		g_engfuncs.pfnCvar_DirectSet(this->m_VoteMap, "1");

		// Start Match
		gMatchTask.Create(TASK_CHANGE_STATE, 2.0f, false, (void*)this->NextState, STATE_START);
	}
}

// Start vote team
void CMatchBot::StartVoteTeam(CBasePlayer* Player)
{
	// Check Accesss
	if (gMatchAdmin.Access(Player->entindex(), ADMIN_VOTE))
	{
		// We only can pickup teams in warmup state
		if (this->m_State == STATE_WARMUP)
		{
			// Stop Ready System 
			gMatchReady.Stop(false);

			// Stop Timer System
			gMatchTimer.Stop(false);

			// Disable vote map for this map session
			g_engfuncs.pfnCvar_DirectSet(this->m_VoteMap, "0");

			// Enable team pickup vote for this map session
			g_engfuncs.pfnCvar_DirectSet(this->m_TeamPickupType, "-1");

			// Start Match
			this->SetState(STATE_START);

			LOG_MESSAGE
			(
				PLID,
				"\"%s<%i><%s><%s>\" started vote team",
				STRING(Player->edict()->v.netname),
				g_engfuncs.pfnGetPlayerUserId(Player->edict()),
				g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
				gMatchBot.GetTeam(Player->m_iTeam, true)
			);
		}
		else
		{
			// Message here
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_RED, _T("Cannot start an vote in ^3%s^1 state."), this->GetState(this->m_State));
		}
	}
	else
	{
		// Error message
		gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
	}
}

// Start match
void CMatchBot::StartMatch(CBasePlayer* Player)
{
	// Check Accesss
	if (gMatchAdmin.Access(Player->entindex(), ADMIN_LEVEL_B))
	{
		// If state is Warmup or Halftime
		if (this->m_State == STATE_WARMUP || this->m_State == STATE_HALFTIME)
		{
			// Stop Ready System if any
			gMatchReady.Stop(false);

			// Stop Timer System if any
			gMatchTimer.Stop(false);

			// Send message to all players
			gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 started match."), STRING(Player->edict()->v.netname));

			// Next state is first half if match is in warmup
			auto State = STATE_FIRST_HALF;

			// If current state is half time
			if (this->m_State == STATE_HALFTIME)
			{
				// Next is Second Half
				State = STATE_SECOND_HALF;

				// If we playing Overtime
				if (this->GetRound() >= this->m_PlayRounds->value)
				{
					// Next state is Overtime
					State = STATE_OVERTIME;
				}
			}

			// If state is first half respect knife round
			if (State == STATE_FIRST_HALF)
			{
				// If has Knife Round variable
				if (this->m_KnifeRound)
				{
					// If Knife Round is set
					if (this->m_KnifeRound->value)
					{
						// Set to play knife round on first half
						this->m_PlayKnifeRound = true;
					}
				}
			}

			// Change state
			gMatchTask.Create(TASK_CHANGE_STATE, 3.0f, false, (void*)this->NextState, State);

			LOG_MESSAGE
			(
				PLID,
				"\"%s<%i><%s><%s>\" started match",
				STRING(Player->edict()->v.netname),
				g_engfuncs.pfnGetPlayerUserId(Player->edict()),
				g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
				gMatchBot.GetTeam(Player->m_iTeam, true)
			);
		}
		else
		{
			// Error message
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_RED, _T("Cannot start match in ^3%s^1 state."), this->GetState(this->m_State));
		}
	}
	else
	{
		// Error message
		gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
	}
}

// Stop current running match
void CMatchBot::StopMatch(CBasePlayer* Player)
{
	// Check Accesss
	if (Player == nullptr || gMatchAdmin.Access(Player->entindex(), ADMIN_LEVEL_B))
	{
		// If match is running
		if (this->m_State >= STATE_FIRST_HALF && this->m_State <= STATE_OVERTIME)
		{
			// Send message to all players
			if (Player)
			{
				gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 stopped match."), STRING(Player->edict()->v.netname));
			}
			else
			{
				gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("^3%s^1 stopped match."), Plugin_info.name);
			}

			// If is in Knife Round
			if(this->m_PlayKnifeRound)
			{
				// Add map objectives
				gMatchWarmup.RemoveMapObjective(false);

				// Disable BOT deathmatch
				g_engfuncs.pfnCVarSetFloat("bot_deathmatch", 0.0f);

				// Execute command to run with all weapons
				gMatchUtil.ServerCommand("bot_all_weapons");

				// Disable Knife Round
				this->m_PlayKnifeRound = false;
			}

			// Set end state
			gMatchTask.Create(TASK_CHANGE_STATE, 3.0f, false, (void*)this->NextState, STATE_END);

			if (Player)
			{
				LOG_MESSAGE
				(
					PLID,
					"\"%s<%i><%s><%s>\" stopped match",
					STRING(Player->edict()->v.netname),
					g_engfuncs.pfnGetPlayerUserId(Player->edict()),
					g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
					gMatchBot.GetTeam(Player->m_iTeam, true)
				);
			}
		}
		else
		{
			// Error message
			gMatchUtil.SayText(Player->edict(), PRINT_TEAM_RED, _T("Cannot stop match in ^3%s^1 state."), this->GetState(this->m_State));
		}
	}
	else
	{
		// Error message
		gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
	}
}

// Restart current match
void CMatchBot::RestartMatch(CBasePlayer* Player)
{
	// Check Access
	if (Player == nullptr || gMatchAdmin.Access(Player->entindex(), ADMIN_LEVEL_B))
	{
		// If match is in first half
		if (this->m_State == STATE_FIRST_HALF || this->m_State == STATE_SECOND_HALF || this->m_State == STATE_OVERTIME)
		{
			// If command send by admin
			if (Player)
			{
				// Send message
				gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 restarted %s, get ready!"), STRING(Player->edict()->v.netname), this->GetState(this->m_State));
			}
			else
			{
				// Send message
				gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("^3%s^1 restarted %s, get ready!"), Plugin_info.name, this->GetState(this->m_State));

			}

			// Restart current state
			this->SetState(this->m_State);

			if (Player)
			{
				LOG_MESSAGE
				(
					PLID,
					"\"%s<%i><%s><%s>\" restarted match",
					STRING(Player->edict()->v.netname),
					g_engfuncs.pfnGetPlayerUserId(Player->edict()),
					g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
					gMatchBot.GetTeam(Player->m_iTeam, true)
				);
			}
		}
		else
		{
			// If admin issued command
			if (Player)
			{
				// Error message
				gMatchUtil.SayText(Player->edict(), PRINT_TEAM_RED, _T("Cannot restart match in ^3%s^1 state."), this->GetState(this->m_State));
			}
		}
	}
	else
	{
		// Error message
		gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
	}
}

// End Match with a winner
void CMatchBot::EndMatch(TeamName Loser, TeamName Winner)
{
	// If match is running
	if (this->m_State >= STATE_FIRST_HALF && this->m_State <= STATE_OVERTIME)
	{
		// Max Rounds to Play
		auto HalfRounds = (int)(this->m_PlayRounds->value / 2.0f);

		// Clear Scores
		memset(this->m_Score, 0, sizeof(this->m_Score));

		// Set Score winner
		this->m_Score[Winner][STATE_FIRST_HALF]  = HalfRounds;
		this->m_Score[Winner][STATE_SECOND_HALF] = HalfRounds;

		// Send message
		gMatchUtil.SayText(nullptr, (Winner == TERRORIST) ? PRINT_TEAM_RED : PRINT_TEAM_BLUE, _T("Game Over! The ^3%s^1 team surrendered!!"), this->GetTeam(Loser, false));

		// If is in Knife Round
		if (this->m_PlayKnifeRound)
		{
			// Add map objectives
			gMatchWarmup.RemoveMapObjective(false);

			// Disable BOT deathmatch
			g_engfuncs.pfnCVarSetFloat("bot_deathmatch", 0.0f);

			// Execute command to run with all weapons
			gMatchUtil.ServerCommand("bot_all_weapons");

			// Disable Knife Round
			this->m_PlayKnifeRound = false;
		}

		// Set end state
		gMatchTask.Create(TASK_CHANGE_STATE, 0.5f, false, (void*)this->NextState, STATE_END);
	}
}
