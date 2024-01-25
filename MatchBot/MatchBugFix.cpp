#include "precompiled.h"

#define FLASH 0
#define HEGRE 1
#define SMOKE 2

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

bool CMatchBugFix::BuyGrenadesLimit(CBasePlayer *player, int slot)
{
	// slot smoke = 3
	// slot hegre = 4
	// slot smoke = 5
	// slot - 3 = FLASH or HEGRE or SMOKE
	if (slot < 3 || slot > 5)
    {
        return true;
    }

	cvar_t *buyLimit[] = {
		gMatchBot.m_BlFlashCount,
		gMatchBot.m_BlHeGreCount,
		gMatchBot.m_BlSmokeCount
	};
    if (buyLimit[slot - 3]->value != 0.0f && gMatchBugFix.playerGrenades[player->entindex()][slot - 3] >= buyLimit[slot - 3]->value)
    {
        return false;
    }

    gMatchBugFix.playerGrenades[player->entindex()][slot - 3]++;
	return true;
}

void CMatchBugFix::cleanPlayerGrenades()
{
	for (int i = 1; i <= MAX_CLIENTS; ++i) {
        this->playerGrenades[i][FLASH] = 0;
        this->playerGrenades[i][HEGRE] = 0;
        this->playerGrenades[i][SMOKE] = 0;
    }
}
