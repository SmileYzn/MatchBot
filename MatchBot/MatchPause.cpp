#include "precompiled.h"

CMatchPause gMatchPause;

// Init Match Pause
void CMatchPause::Init(CBasePlayer* Player)
{
	// Check Access
	if (Player == nullptr || gMatchAdmin.Access(Player->entindex(), ADMIN_LEVEL_B))
	{
		// Check if match is live
		if (gMatchBot.GetState() == STATE_FIRST_HALF || gMatchBot.GetState() == STATE_SECOND_HALF || gMatchBot.GetState() == STATE_OVERTIME)
		{
			// If is not in freeze time period
			if (!CSGameRules()->IsFreezePeriod())
			{
				// Pause Match
				this->m_Pause = true;

				// If map has buyzone
				if (CSGameRules()->m_bMapHasBuyZone)
				{
					// Get default buy time
					this->m_BuyTime = g_engfuncs.pfnCVarGetFloat("mp_buytime");

					// Set buytime
					g_engfuncs.pfnCVarSetFloat("mp_buytime", gMatchBot.m_PauseTime->value);
				}
				
				// If admin issued command
				if (Player)
				{
					// Send Message
					gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 Paused Match: Game will pause on next round start."), STRING(Player->edict()->v.netname));
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

			// Pause for 60 seconds
			this->SetRoundTime(static_cast<int>(gMatchBot.m_PauseTime->value) + 1, true);

			// Create Pause Timer Task
			gMatchTask.Create(TASK_PAUSE_MATCH, 0.5, true, (void*)this->PauseTimer, static_cast<int>(gMatchBot.m_PauseTime->value));

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
	}
}

// Pause Timer Count Function
void CMatchPause::PauseTimer(int PauseTime)
{
	// If has CS GameRules
	if (g_pGameRules)
	{
		// Get Rem ain time of current pause
		time_t RemainTime = (time_t)((time_t)PauseTime - (time_t)(gpGlobals->time - CSGameRules()->m_fRoundStartTime));

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
					gMatchUtil.HudMessage(nullptr, gMatchUtil.HudParam(0, 255, 0, -1.0, 0.2, 0, 0.53, 0.53, 0.0, 0.0, 1), _T("MATCH PAUSED^n%s LEFT"), Time);
				}
			}
		}
		else
		{
			// Remove Pause Task
			gMatchTask.Remove(TASK_PAUSE_MATCH);
			
			// If has CS Game Rules
			if (g_pGameRules)
			{
				//if has buyzone
				if (CSGameRules()->m_bMapHasBuyZone)
				{
					// Restore default buy time
					g_engfuncs.pfnCVarSetFloat("mp_buytime", gMatchPause.m_BuyTime);
				}
			}

			// Restore Freezetime
			gMatchPause.SetRoundTime((int)CVAR_GET_FLOAT("mp_freezetime"), true);
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
	if (iMsgRoundTime || (iMsgRoundTime = gpMetaUtilFuncs->pfnGetUserMsgID(PLID, "RoundTime", NULL)))
	{
		g_engfuncs.pfnMessageBegin(MSG_ALL, iMsgRoundTime, NULL, NULL);
		g_engfuncs.pfnWriteShort(Time);
		g_engfuncs.pfnMessageEnd();
	}
}
