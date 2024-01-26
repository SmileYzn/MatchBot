#pragma once

class CMatchRestrictItem
{
public:
	// On Round Restart
	void RoundRestart();

	// Check item restrictions
	bool PlayerHasRestrictItem(CBasePlayer* Player, ItemID item, ItemRestType type);
};

extern CMatchRestrictItem gMatchRestrictItem;
