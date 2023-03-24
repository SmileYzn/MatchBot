#include "precompiled.h"

CMatchScore gMatchScore;

// On server activate event
void CMatchScore::ServerActivate()
{
	// Store team scores in scoreboard
	this->m_TeamScore = gMatchUtil.CvarRegister("mb_scoreboard_team", "1");

	// Store player scores in scoreboard
	this->m_PlayerScore = gMatchUtil.CvarRegister("mb_scoreboard_player", "1");

	// Display states and scores at game description
	this->m_GameName = gMatchUtil.CvarRegister("mb_gamename", "1");

	// Register TeamScore message hook
	gMatchMessage.RegisterHook("TeamScore", this->TeamScore);

	// Register ScoreInfo message hook
	gMatchMessage.RegisterHook("ScoreInfo", this->ScoreInfo);
}

// Update Game Name
void CMatchScore::UpdateGameName()
{
	// If has CSGameRules loaded
	if (g_pGameRules)
	{
		// Store original game description
		if(!this->m_GameDesc[0])
		{
			// Get default game name
			Q_strncpy(this->m_GameDesc, CSGameRules()->GetGameDescription(), sizeof(this->m_GameDesc));
		}
		
		// If is enabled
		if(this->m_GameName && this->m_GameName->value)
		{
			// Get match state
			auto State = gMatchBot.GetState();

			// If is not running, set default name
			if (State == STATE_DEAD)
			{
				// Restore default game name
				Q_strcpy(CSGameRules()->m_GameDesc, this->m_GameDesc);
			}
			else if(State == STATE_WARMUP || State == STATE_START)
			{
				// Set game name from state name
				Q_strcpy(CSGameRules()->m_GameDesc, gMatchBot.GetState(State));
			}
			else if (State >= STATE_FIRST_HALF && State <= STATE_END)
			{
				// Game Name
				char GameName[32] = { 0 };

				// Format game name with teams and scores
				Q_snprintf(GameName, sizeof(GameName), "%s « %d : %d » %s", gMatchBot.GetTeam(TERRORIST, true), gMatchBot.GetScore(TERRORIST), gMatchBot.GetScore(CT), gMatchBot.GetTeam(CT, true));

				// Get to game description
				Q_strcpy(CSGameRules()->m_GameDesc, GameName);
			}
		}
		else
		{
			// Restore default game name
			Q_strcpy(CSGameRules()->m_GameDesc, this->m_GameDesc);
		}
	}
}

// Get TeamScore enabled
bool CMatchScore::GetScoreBoardTeam()
{
	return (this->m_TeamScore && this->m_TeamScore->value > 0.0f);
}

// TeamScore HL1 message
bool CMatchScore::TeamScore(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity)
{
	// If is enabled
	if(gMatchScore.GetScoreBoardTeam())
	{
		// If match running after first half
		if (gMatchBot.GetState() > STATE_FIRST_HALF)
		{
			// Get Team Name
			auto TeamID = gMatchMessage.GetString(0);

			// If is not null
			if (TeamID)
			{
				// Terrorists
				if (TeamID[0] == 'T')
				{
					// Set Score for Terrorists to avoid sv_restart reset
					gMatchMessage.SetArgInt(1, gMatchBot.GetScore(TERRORIST));
				}
				else if (TeamID[0] == 'C')
				{
					// Set Score for CTs  to avoid sv_restart reset
					gMatchMessage.SetArgInt(1, gMatchBot.GetScore(CT));
				}
			}
		}
	}

	// Do not block original message call
	return false;
}

// Get PlayerScore enabled
bool CMatchScore::GetScoreBoardPlayer()
{
	return (this->m_PlayerScore && this->m_PlayerScore->value > 0.0f);
}

// ScoreInfo HL1 message
bool CMatchScore::ScoreInfo(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity)
{
	// If enabled
	if(gMatchScore.GetScoreBoardPlayer())
	{
		// If match running after first half
		if (gMatchBot.GetState() > STATE_FIRST_HALF)
		{
			// Player Index
			auto PlayerID = gMatchMessage.GetByte(0);

			// If is player
			if (PlayerID)
			{
				// Get Score parameter
				auto Frags = gMatchMessage.GetShort(1);

				// Get Deaths Parameter
				auto Deaths = gMatchMessage.GetShort(2);

				// Get Frags and Deaths from stats
				auto Score = gMatchStats.GetScore(PlayerID);

				// If is not null
				if(Score)
				{
					// Increment with current frags
					Frags += Score[0];

					// Increment with current deaths
					Deaths += Score[1];
				}

				// Set player score to message
				gMatchMessage.SetArgInt(1, Frags);

				// Set player deaths to message
				gMatchMessage.SetArgInt(2, Deaths);
			}
		}
	}

	// Do not block original message call
	return false;
}