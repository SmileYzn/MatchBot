#pragma once

class CMatchRestrictItem
{
public:
	// Check item restrictions
	bool PlayerHasRestrictItem(CBasePlayer* Player, ItemID item, ItemRestType type);
};

extern CMatchRestrictItem gMatchRestrictItem;
