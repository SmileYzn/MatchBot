#include "precompiled.h"

CMatchRestrictItem gMatchRestrictItem;

// Check item restrictions
bool CMatchRestrictItem::PlayerHasRestrictItem(CBasePlayer* Player, ItemID item, ItemRestType type)
{
	// Block Result
	bool BlockItem = false;

	// If has set variable
	if (gMatchBot.m_RestrictWeapons->string)
	{
		// If is an valid item
		if (item >= ITEM_SHIELDGUN && item <= ITEM_BATTERY)
		{
			// If character differs from zero
			if (gMatchBot.m_RestrictWeapons->string[item] != '0')
			{
				// Block this item
				BlockItem = true;
			}
		}
	}

	// Check if playing Knife Round
	if (gMatchBot.GetKnifeRound())
	{
		// Do not allow if item differs from Klevar, Assault Suit or Knife
		if (item != ITEM_KEVLAR && item != ITEM_ASSAULT && item != ITEM_KNIFE)
		{
			// Block this item
			BlockItem = true;
		}
	}

	// If item was blocked
	if (BlockItem)
	{
		// If comes from buy menu
		if(type == ITEM_TYPE_BUYING)
		{
			// Send blocked message
			gMatchUtil.ClientPrint(Player->edict(), PRINT_CENTER, "#Cstrike_TitlesTXT_Weapon_Not_Available");
		}
	}

	// Return the result
	return BlockItem;
}

