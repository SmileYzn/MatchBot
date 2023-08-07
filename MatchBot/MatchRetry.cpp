#include "precompiled.h"

CMatchRetry gMatchRetry;

bool CMatchRetry::PlayerConnect(edict_t* pEdict, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	if (!FNullEnt(pEdict))
	{
		auto Auth = g_engfuncs.pfnGetPlayerAuthId(pEdict);

		if (Auth)
		{
			if (this->m_Data.find(Auth) != this->m_Data.end())
			{
				auto TimeLeft = (int)(this->m_Data[Auth] - gpGlobals->time);

				if (TimeLeft > 0)
				{
					gMatchUtil.DropClient(ENTINDEX(pEdict), _T("Wait %d seconds before reconnect."), TimeLeft);
				}
				else
				{
					this->m_Data.erase(Auth);
				}
			}
		}
	}

	// Allow Connection
	return true;
}

void CMatchRetry::PlayerDisconnect(edict_t* pEdict, bool crash, const char* Reason)
{
	if (gMatchBot.m_RetryMode->value > 0.0f)
	{
		if (gMatchBot.m_RetryTime->value > 0.0f)
		{
			if (gMatchBot.GetState() != STATE_DEAD)
			{
				if (!FNullEnt(pEdict))
				{
					auto Player = UTIL_PlayerByIndexSafe(ENTINDEX(pEdict));

					if (Player)
					{
						if (!gMatchAdmin.Access(Player->entindex(), ADMIN_IMMUNITY))
						{
							if (Player->m_iTeam == TERRORIST || Player->m_iTeam == CT)
							{
								if (!Player->IsBot() && !Player->IsDormant())
								{
									auto Auth = g_engfuncs.pfnGetPlayerAuthId(pEdict);

									if (Auth)
									{
										if ((gMatchBot.m_RetryMode->value > 1.0f) || ((gMatchBot.m_RetryMode->value == 1.0f) && !Q_stricmp(Reason, "Client sent 'drop'")))
										{
											this->m_Data[Auth] = (gpGlobals->time + gMatchBot.m_RetryTime->value);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
