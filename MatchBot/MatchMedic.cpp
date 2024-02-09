#include "precompiled.h"

CMatchMedic gMatchMedic;

// Matchbot changed state
void CMatchMedic::SetState(int State)
{
	// If match is starting first half
	if (State == STATE_FIRST_HALF)
	{
		// Reset all data
		this->m_Data.clear();
	}
}

// Update Player Data
void CMatchMedic::PlayerUpdate(CBasePlayer* Player)
{
	// If is not null
	if (Player)
	{
		// If is in game
		if ((Player->m_iTeam == TERRORIST) || (Player->m_iTeam == CT))
		{
			// Get Steam ID
			auto AuthId = gMatchUtil.GetPlayerAuthId(Player->edict());

			// If is not null
			if (AuthId)
			{
				// Set Frags
				this->m_Data[AuthId].Frags = static_cast<int>(Player->edict()->v.frags);

				// Set Deaths
				this->m_Data[AuthId].Deaths = Player->m_iDeaths;

				// Set Team
				this->m_Data[AuthId].Team = Player->m_iTeam;

				// Set Money
				this->m_Data[AuthId].Account = Player->m_iAccount;
			}
		}
	}
}

// Account Update
void CMatchMedic::PlayerAddAccount(CBasePlayer* Player)
{
	// If match is live
	if ((gMatchBot.GetState() == STATE_FIRST_HALF) || (gMatchBot.GetState() == STATE_SECOND_HALF) || (gMatchBot.GetState() == STATE_OVERTIME))
	{
		// Update player
		this->PlayerUpdate(Player);
	}
}

// Disconnect
void CMatchMedic::PlayerDisconnect(edict_t* pEntity)
{
	// If match is live
	if ((gMatchBot.GetState() == STATE_FIRST_HALF) || (gMatchBot.GetState() == STATE_SECOND_HALF) || (gMatchBot.GetState() == STATE_OVERTIME))
	{
		// Get CBasePlayer
		auto Player = UTIL_PlayerByIndexSafe(ENTINDEX(pEntity));

		// If is not null
		if (Player)
		{
			// Update player
			this->PlayerUpdate(Player);
		}
	}
}

// Player Get Into Game
void CMatchMedic::PlayerGetIntoGame(CBasePlayer* Player)
{
	// If match is live
	if ((gMatchBot.GetState() >= STATE_FIRST_HALF) && (gMatchBot.GetState() <= STATE_OVERTIME))
	{
		// If joined in a team
		if ((Player->m_iTeam == TERRORIST) || (Player->m_iTeam == CT))
		{
			// Get Steam ID
			auto AuthId = gMatchUtil.GetPlayerAuthId(Player->edict());

			// If is not null
			if (AuthId)
			{
				// If data was set
				if (this->m_Data.find(AuthId) != this->m_Data.end())
				{
					//// Restore Deaths
					//Player->m_iDeaths = this->m_Data[AuthId].Deaths;

					//// Restore Frags and send message to scoreboard
					//Player->AddPoints(this->m_Data[AuthId].Frags, true);

					//// Restore money and send to scoreboard
					//Player->CSPlayer()->AddAccount(this->m_Data[AuthId].Account, RT_NONE, true);
				}
			}
		}
	}
}