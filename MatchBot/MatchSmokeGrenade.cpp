#include "precompiled.h"

CMatchSmokeGrenade gMatchSmokeGrenade;

void CMatchSmokeGrenade::ExplodeSmokeGrenade(CGrenade* Entity)
{
	if (Entity)
	{
		if (gMatchBot.m_ExtraSmokeCount->value > 0.0f)
		{
			auto SmokeCount = (int)(gMatchBot.m_ExtraSmokeCount->value);

			for (int i = 0; i < SmokeCount; i++)
			{
				g_engfuncs.pfnPlaybackEvent(FEV_GLOBAL, 0, Entity->m_usEvent, 0.0f, Entity->edict()->v.origin, Entity->edict()->v.angles, 0.0f, 0.0f, 0, 1, 1, 0);
			}
		}
	}
}
