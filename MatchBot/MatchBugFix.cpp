#include "precompiled.h"

CMatchBugFix gMatchBugFix;

void CMatchBugFix::ExplodeSmokeGrenade(CGrenade* Entity)
{
	if (gMatchBot.m_ExtraSmokeCount->value > 0.0f)
	{
		auto State = gMatchBot.GetState();

		if (State == STATE_FIRST_HALF || State == STATE_SECOND_HALF || State == STATE_OVERTIME)
		{
			auto Count = (int)(gMatchBot.m_ExtraSmokeCount->value);

			for (int i = 0; i < Count; i++)
			{
				g_engfuncs.pfnPlaybackEvent(FEV_GLOBAL, 0, Entity->m_usEvent, 0.0f, Entity->edict()->v.origin, Entity->edict()->v.angles, 0.0f, 0.0f, 0, 1, 1, 0);
			}
		}
	}
}

void CMatchBugFix::PlayerDuck(CBasePlayer* Player)
{
	if (gMatchBot.m_FixRespawnBug->value > 0.0f)
	{
		if (g_pGameRules)
		{
			if (CSGameRules()->IsFreezePeriod())
			{
				auto State = gMatchBot.GetState();

				if (State == STATE_FIRST_HALF || State == STATE_SECOND_HALF || State == STATE_OVERTIME)
				{
					if ((Player->edict()->v.origin - Player->m_vLastOrigin).Length2D() > 0.0f)
					{
						g_engfuncs.pfnSetOrigin(Player->edict(), Player->m_vLastOrigin);
					}
				}
			}
		}
	}
}
