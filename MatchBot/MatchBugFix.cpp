#include "precompiled.h"

CMatchBugFix gMatchBugFix;

void CMatchBugFix::ExplodeSmokeGrenade(CGrenade* Entity)
{
	if (gMatchBot.m_ExtraSmokeCount->value > 0.0f)
	{
		if (gMatchBot.GetState() == STATE_FIRST_HALF || gMatchBot.GetState() == STATE_SECOND_HALF || gMatchBot.GetState() == STATE_OVERTIME)
		{
			auto Count = static_cast<int>(gMatchBot.m_ExtraSmokeCount->value);

			for (int i = 0; i < Count; i++)
			{
				g_engfuncs.pfnPlaybackEvent(FEV_GLOBAL, 0, Entity->m_usEvent, 0.0f, Entity->edict()->v.origin, Entity->edict()->v.angles, 0.0f, 0.0f, 0, 1, 1, 0);
			}
		}
	}
}
