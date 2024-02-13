#include "precompiled.h"

CMatchStats gMatchStats;

// On match bot new state
void CMatchStats::SetState(int State, bool KnifeRound)
{
	// Switch states
	switch (State)
	{
		case STATE_DEAD:
		case STATE_WARMUP:
		case STATE_START:
		{
			// Reset match data
			this->m_Match.Reset();

			// Reset round event data
			this->m_Event.clear();

			// Reset round data
			this->m_RoundData.Reset();
			break;
		}
		case STATE_FIRST_HALF:
		{
			// Set Hostname
			this->m_Match.HostName = (gMatchBot.m_Hostname) ? gMatchBot.m_Hostname->string : "";

			// Set Map
			this->m_Match.Map = STRING(gpGlobals->mapname);

			// Set address
			this->m_Match.Address = (gMatchBot.m_NetAddress) ? gMatchBot.m_NetAddress->string : "";

			// Set match start time
			this->m_Match.Time[0] = time(nullptr);

			// Set match end time
			this->m_Match.Time[1] = 0;

			// Reset game mode
			this->m_Match.GameMode = gMatchVoteTeam.GetMode();

			// Match has Knife Round
			this->m_Match.KnifeRound = KnifeRound ? 1 : 0;

			// Reset round event data
			this->m_Event.clear();

			// Reset round data
			this->m_RoundData.Reset();
			break;
		}
		case STATE_HALFTIME:
		case STATE_SECOND_HALF:
		case STATE_OVERTIME:
		{
			// Reset round data
			this->m_RoundData.Reset();
			break;
		}
		case STATE_END:
		{
			// Set match end time
			this->m_Match.Time[1] = time(nullptr);

			// Reset round data
			this->m_RoundData.Reset();
			break;
		}
	}
}

// On BOT manager event
void CMatchStats::CBotManager_OnEvent(GameEventType GameEvent, CBaseEntity* pEntity, CBaseEntity* pOther)
{
	// If match is live
	if ((gMatchBot.GetState() == STATE_FIRST_HALF) || (gMatchBot.GetState() == STATE_SECOND_HALF) || (gMatchBot.GetState() == STATE_OVERTIME))
	{
		// Player Event Data
		this->PlayerEvent(GameEvent, pEntity, pOther);

		// Match Event Data
		this->MatchEvent(GameEvent, pEntity, pOther);

		// Round Event Data
		this->RoundEvent(GameEvent, pEntity, pOther);
	}
}

// On Player event
void CMatchStats::PlayerEvent(GameEventType GameEvent, CBaseEntity* pEntity, CBaseEntity* pOther)
{
	// Switch event
	switch (GameEvent)
	{
		case EVENT_PLAYER_DIED:
		{
			break;
		}
	}
}

// On Match event
void CMatchStats::MatchEvent(GameEventType GameEvent, CBaseEntity* pEntity, CBaseEntity* pOther)
{
	// Switch event
	switch (GameEvent)
	{
		case EVENT_ROUND_START:
		{
			// Reset round data
			this->m_RoundData.Reset();
			break;
		}
	}
}

// On Round event
void CMatchStats::RoundEvent(GameEventType GameEvent, CBaseEntity* pEntity, CBaseEntity* pOther)
{
	// If is some of these events
	if ((GameEvent == EVENT_PLAYER_DIED) || (GameEvent == EVENT_BOMB_PLANTED) || (GameEvent == EVENT_BOMB_PICKED_UP) || (GameEvent == EVENT_BOMB_DEFUSED) || (GameEvent == EVENT_BOMB_EXPLODED) || (GameEvent == EVENT_TERRORISTS_WIN) || (GameEvent == EVENT_CTS_WIN))
	{
		// Round event data struct
		P_ROUND_EVENT Event = { };

		// Set current round count
		Event.Round = (gMatchBot.GetRound() + 1);

		// If has CSGameRules class
		if (g_pGameRules)
		{
			// Set round remaining time
			Event.Time = CSGameRules()->GetRoundRemainingTimeReal();
		}

		// Set what event is
		Event.GameEvent = GameEvent;

		// Switch events
		switch (GameEvent)
		{
			case EVENT_PLAYER_DIED:
			{
				if (!FNullEnt(pEntity))
				{
					auto Victim = static_cast<CBasePlayer*>(pEntity);

					if (Victim)
					{
						Event.Victim = gMatchUtil.GetPlayerAuthId(Victim->edict());

						Event.VictimOrigin = Victim->edict()->v.origin;

						Event.ScenarioEvent = ROUND_NONE;

						Event.Loser = Victim->m_iTeam;

						Event.IsHeadShot = Victim->m_bHeadshotKilled;

						if (Victim->m_bKilledByBomb)
						{
							Event.ItemIndex = WEAPON_C4;
						}

						if (Victim->m_bKilledByGrenade)
						{
							Event.ItemIndex = WEAPON_HEGRENADE;
						}
					}
				}

				if (!FNullEnt(pOther))
				{
					auto Killer = static_cast<CBasePlayer*>(pOther);

					if (Killer)
					{
						Event.Killer = gMatchUtil.GetPlayerAuthId(Killer->edict());

						Event.KillerOrigin = Killer->edict()->v.origin;

						Event.ScenarioEvent = ROUND_NONE;

						Event.Winner = Killer->m_iTeam;

						if (Killer->m_pActiveItem)
						{
							if (Event.ItemIndex == WEAPON_NONE)
							{
								Event.ItemIndex = Killer->m_pActiveItem->m_iId;
							}
						}
					}
				}

				break;
			}
			case EVENT_BOMB_PLANTED:
			{
				Event.ScenarioEvent = ROUND_NONE;

				if (!FNullEnt(pEntity))
				{
					auto Player = UTIL_PlayerByIndexSafe(pEntity->entindex());

					if (Player)
					{
						Event.Killer = gMatchUtil.GetPlayerAuthId(pEntity->edict());

						Event.KillerOrigin = Player->edict()->v.origin;

						Event.Victim = "";

						Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };

						this->m_RoundData.BombPlanter = pEntity->entindex();
					}

					Event.Winner = TERRORIST;

					Event.Loser = CT;

					Event.IsHeadShot = false;

					Event.ItemIndex = WEAPON_C4;
				}

				break;
			}
			case EVENT_BOMB_PICKED_UP:
			{
				Event.ScenarioEvent = ROUND_NONE;

				if (!FNullEnt(pEntity))
				{
					auto Player = UTIL_PlayerByIndexSafe(pEntity->entindex());

					if (Player)
					{
						Event.Killer = gMatchUtil.GetPlayerAuthId(pEntity->edict());

						Event.KillerOrigin = Player->edict()->v.origin;

						Event.Victim = "";

						Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };
					}

					Event.Winner = TERRORIST;

					Event.Loser = CT;

					Event.IsHeadShot = true;

					Event.ItemIndex = WEAPON_C4;
				}

				break;
			}
			case EVENT_BOMB_DEFUSED:
			{
				if (!FNullEnt(pEntity))
				{
					auto Player = UTIL_PlayerByIndexSafe(pEntity->entindex());

					if (Player)
					{
						Event.Killer = gMatchUtil.GetPlayerAuthId(pEntity->edict());

						Event.KillerOrigin = Player->edict()->v.origin;

						Event.Victim = "";

						Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };

						Event.IsHeadShot = Player->m_bHasDefuser;
					}

					Event.ScenarioEvent = ROUND_BOMB_DEFUSED;

					Event.Winner = CT;

					Event.Loser = TERRORIST;

					Event.ItemIndex = WEAPON_C4;
				}

				break;
			}
			case EVENT_BOMB_EXPLODED:
			{
				auto Player = UTIL_PlayerByIndexSafe(this->m_RoundData.BombPlanter);

				if (Player)
				{
					Event.Killer = gMatchUtil.GetPlayerAuthId(Player->edict());

					Event.KillerOrigin = Player->m_vLastOrigin;

					Event.Victim = "";

					Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };

					Event.IsHeadShot = false;
				}

				Event.ScenarioEvent = ROUND_TARGET_BOMB;

				Event.Winner = TERRORIST;

				Event.Loser = CT;

				Event.ItemIndex = WEAPON_C4;

				break;
			}
			case EVENT_TERRORISTS_WIN:
			{
				Event.ScenarioEvent = ROUND_TERRORISTS_WIN;

				Event.Winner = TERRORIST;

				Event.Loser = CT;

				Event.Killer = "";

				Event.Victim = "";

				Event.KillerOrigin = { 0.0f, 0.0f, 0.0f };

				Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };

				Event.IsHeadShot = false;

				Event.ItemIndex = WEAPON_NONE;

				break;
			}
			case EVENT_CTS_WIN:
			{
				Event.ScenarioEvent = ROUND_CTS_WIN;

				Event.Winner = CT;

				Event.Loser = TERRORIST;

				Event.Killer = "";

				Event.Victim = "";

				Event.KillerOrigin = { 0.0f, 0.0f, 0.0f };

				Event.VictimOrigin = { 0.0f, 0.0f, 0.0f };

				Event.IsHeadShot = false;

				Event.ItemIndex = WEAPON_NONE;
				break;
			}
		}

		// Push that match event
		this->m_Event.push_back(Event);
	}
}
