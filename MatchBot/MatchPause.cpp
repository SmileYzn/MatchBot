#include "precompiled.h"

CMatchPause gMatchPause;

// On server activate
void CMatchPause::ServerActivate()
{
	this->m_Pause = false;

	this->m_Paused = false;

	this->m_PauseTeam = "";

	this->m_BuyTime = 0.0f;
}

// Init pause
void CMatchPause::Init(CBasePlayer* Player, TeamName Team)
{
	// Check Access
	if ((Player == nullptr) || (gMatchAdmin.Access(Player->entindex(), ADMIN_LEVEL_B)))
	{
		// Check if match is  live
		if ((gMatchBot.GetState() == STATE_FIRST_HALF) || (gMatchBot.GetState() == STATE_SECOND_HALF) || (gMatchBot.GetState() == STATE_OVERTIME))
		{
			// If has CS game rules
			if (g_pGameRules)
			{
				// If is not in freeze time period
				if (!CSGameRules()->IsFreezePeriod())
				{
					// Pause Match
					this->m_Pause = true;

					// Is not paused yet
					this->m_Paused = false;

					// Clear string
					this->m_PauseTeam.clear();

					// If player is null, pause was called by a team
					if (Player == nullptr)
					{
						// Set team name
						this->m_PauseTeam = gMatchBot.GetTeam(Team, false);
					}
					else
					{
						// Set Admin Name
						this->m_PauseTeam = STRING(Player->edict()->v.netname);
					}

					// If admin issued command
					if (!this->m_PauseTeam.empty())
					{
						// Send Message
						gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 Paused Match: Game will pause on next round start."), this->m_PauseTeam.c_str());
					}
					else
					{
						// Send Message
						gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Match will pause on next round start."));
					}
				}
				else
				{
					// If has player
					if (Player)
					{
						// Send message
						gMatchUtil.SayText(Player->edict(), PRINT_TEAM_RED, _T("Cannot pause match in freeze time period."));
					}
				}
			}
		}
		else
		{
			// If admin issued command
			if (Player)
			{
				// Send error message
				gMatchUtil.SayText(Player->edict(), PRINT_TEAM_RED, _T("Cannot pause match in ^3%s^1 state."), gMatchBot.GetState(gMatchBot.GetState()));
			}
		}
	}
	else
	{
		gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
	}
}

// On Round Freezetime init event
void CMatchPause::RoundRestart()
{
	// Check if match is live
	if (gMatchBot.GetState() == STATE_FIRST_HALF || gMatchBot.GetState() == STATE_SECOND_HALF || gMatchBot.GetState() == STATE_OVERTIME)
	{
		// If we set to pause match
		if (this->m_Pause)
		{
			// Disable it
			this->m_Pause = false;

			// Pause
			this->Pause();
		}
		else
		{
			// If auto pause is enabled
			if (gMatchBot.m_PauseAuto->value > 0.0f)
			{
				// Get player count
				auto PlayerCount = gMatchUtil.GetCount();

				// If server do not have an spectator
				if (PlayerCount[SPECTATOR] < 1)
				{
					// Get player count in teams
					auto PlayerNum = (PlayerCount[TERRORIST] + PlayerCount[CT]);

					// Minimum of required players
					auto PlayersMin = static_cast<int>(gMatchBot.m_PlayersMin->value);

					// If player count is less than required players
					if (PlayerNum < PlayersMin)
					{
						// Send message
						gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Paused Match: ^3%d^1 player(s) left to continue the match."), abs(PlayersMin - PlayerNum));

						// Pause match
						this->Pause();
					}
				}
			}
		}
	}
}

void CMatchPause::Pause()
{
	// Is paused
	this->m_Paused = true;

	// If has CS game rules
	if (g_pGameRules)
	{
		// If default buy time was not set yet
		if (this->m_BuyTime <= 0.0f)
		{
			// Get default buy time
			this->m_BuyTime = gMatchBot.m_BuyTime->value;
		}

		// Set buytime
		g_engfuncs.pfnCvar_DirectSet(gMatchBot.m_BuyTime, std::to_string(gMatchBot.m_PauseTime->value).c_str());
	}

	// Pause for 60 seconds
	this->SetRoundTime(static_cast<int>(gMatchBot.m_PauseTime->value) + 1, true);

	// Create Pause Timer Task
	gMatchTask.Create(TASK_PAUSE_MATCH, 0.5f, true, (void*)this->PauseTimer, static_cast<int>(gMatchBot.m_PauseTime->value));

	// Remain time in seconds
	time_t RemainTime = static_cast<time_t>(gMatchBot.m_PauseTime->value);

	// Create time struct
	struct tm* tm_info = localtime(&RemainTime);

	// If has data
	if (tm_info)
	{
		// Time String
		char Time[16];

		// If formated string with time 
		if (strftime(Time, sizeof(Time), "%M:%S", tm_info) > 0)
		{
			// Send message
			gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Match paused: Match will continue in ^3%s^1."), Time);
		}
	}
}

// Pause Timer Count Function
void CMatchPause::PauseTimer(int PauseTime)
{
	// If has CS GameRules
	if (g_pGameRules)
	{
		// Is paused
		gMatchPause.m_Paused = true;

		// Get Rem ain time of current pause
		auto RemainTime = (static_cast<time_t>(PauseTime) - static_cast<time_t>(gpGlobals->time - CSGameRules()->m_fRoundStartTime));

		// If has time
		if (RemainTime > 0)
		{
			// Get time structure
			struct tm* tm_info = localtime(&RemainTime);

			// If has data
			if (tm_info)
			{
				// Char array
				char Time[16];

				// Format time to char array
				if (strftime(Time, sizeof(Time), "%M:%S", tm_info) > 0)
				{
					// Send message
					if (!gMatchPause.m_PauseTeam.empty())
					{
						gMatchUtil.HudMessage(nullptr, gMatchUtil.HudParam(0, 255, 0, -1.0, 0.2, 0, 0.53, 0.53, 0.0, 0.0, 1), _T("MATCH PAUSED BY %s^n%s LEFT"), gMatchPause.m_PauseTeam.c_str(), Time);
					}
					else
					{
						gMatchUtil.HudMessage(nullptr, gMatchUtil.HudParam(0, 255, 0, -1.0, 0.2, 0, 0.53, 0.53, 0.0, 0.0, 1), _T("MATCH PAUSED^n%s LEFT"), Time);
					}
				}
			}
		}
		else
		{
			// Is paused
			gMatchPause.m_Paused = false;

			// Remove Pause Task
			gMatchTask.Remove(TASK_PAUSE_MATCH);
			
			// If has CS Game Rules
			if (g_pGameRules)
			{
				// Restore default buy time
				g_engfuncs.pfnCvar_DirectSet(gMatchBot.m_BuyTime, std::to_string(gMatchPause.m_BuyTime).c_str());
			}
			
			// Restore Freezetime
			gMatchPause.SetRoundTime(static_cast<int>(gMatchBot.m_Freezetime->value), true);
		}
	}
}


void CMatchPause::SetRoundTime(int Time, bool FreezePeriod)
{
	// If has CS Game Rules
	if (g_pGameRules)
	{
		// If is not a full sv_restart
		if (!CSGameRules()->m_bCompleteReset)
		{
			// Set Freeze Period 
			CSGameRules()->m_bFreezePeriod = FreezePeriod;

			// Set Round Time Seconds
			CSGameRules()->m_iRoundTimeSecs = Time;

			// Set Round Time Freeze Time
			CSGameRules()->m_iIntroRoundTime = Time;

			// Set Round Start Time
			CSGameRules()->m_fRoundStartTime = gpGlobals->time;
		}
	}

	//  Message Index
	static int iMsgRoundTime;

	// If has message index or set message index if is null
	if (iMsgRoundTime || (iMsgRoundTime = gpMetaUtilFuncs->pfnGetUserMsgID(&Plugin_info, "RoundTime", NULL)))
	{
		g_engfuncs.pfnMessageBegin(MSG_ALL, iMsgRoundTime, NULL, NULL);
		g_engfuncs.pfnWriteShort(Time);
		g_engfuncs.pfnMessageEnd();
	}
}
