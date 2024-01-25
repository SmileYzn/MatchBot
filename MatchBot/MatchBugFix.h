#pragma once

class CMatchBugFix
{
public:
	void ExplodeSmokeGrenade(CGrenade* Entity);
	bool BuyGrenadesLimit(CBasePlayer *player, int slot);
	void cleanPlayerGrenades();
public:
	int playerGrenades[MAX_CLIENTS + 1][3];
};

extern CMatchBugFix gMatchBugFix;