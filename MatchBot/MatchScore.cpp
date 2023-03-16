#include "precompiled.h"

CMatchScore gMatchScore;

// On server activate event
void CMatchScore::ServerActivate()
{
	// Register TeamScore message hook
	gMatchMessage.RegisterHook("TeamScore", this->TeamScore);

	// Register ScoreInfo message hook
	gMatchMessage.RegisterHook("ScoreInfo", this->ScoreInfo);
}

// TeamScore HL1 message
bool CMatchScore::TeamScore(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity)
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

	// Do not block original message call
	return false;
}

// ScoreInfo HL1 message
bool CMatchScore::ScoreInfo(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity)
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

	// Do not block original message call
	return false;
}