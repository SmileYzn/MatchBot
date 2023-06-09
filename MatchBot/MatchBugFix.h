#pragma once

class CMatchBugFix
{
public:
	void ExplodeSmokeGrenade(CGrenade* Entity);
	void PlayerDuck(CBasePlayer* Player);
};

extern CMatchBugFix gMatchBugFix;