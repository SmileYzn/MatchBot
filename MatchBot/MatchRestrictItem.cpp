#include "precompiled.h"

CMatchRestrictItem gMatchRestrictItem;

// On Round Restart Event
void CMatchRestrictItem::RoundRestart()
{
	// If Match BOT is not dead
	if (gMatchBot.GetState() != STATE_DEAD)
	{
		// Get in game players
		auto Players = gMatchUtil.GetPlayers(true, true);

		// Loop Players
		for (auto& Player : Players)
		{
			// Reset item count
			memset(&Player->m_rgItems, 0, sizeof(Player->m_rgItems));
		}
	}
}

// Check item restrictions
bool CMatchRestrictItem::PlayerHasRestrictItem(CBasePlayer* Player, ItemID item, ItemRestType type)
{
	// If has set variable
	if (gMatchBot.m_RestrictWeapons->string)
	{
		// If is an valid item
		if (item >= ITEM_SHIELDGUN && item <= ITEM_BATTERY)
		{
			// If character differs from zero
			if (gMatchBot.m_RestrictWeapons->string[item] != '0')
			{
				// Send blocked message
				gMatchUtil.ClientPrint(Player->edict(), PRINT_CENTER, "#Cstrike_TitlesTXT_Weapon_Not_Available");

				// Block item
				return true;
			}
		}
	}

	// Check if playing Knife Round
	if (gMatchBot.GetKnifeRound())
	{
		// Do not allow if item differs from Klevar, Assault Suit or Knife
		if (item != ITEM_KEVLAR && item != ITEM_ASSAULT && item != ITEM_KNIFE)
		{
			// Send blocked message
			gMatchUtil.ClientPrint(Player->edict(), PRINT_CENTER, "#Cstrike_TitlesTXT_Weapon_Not_Available");

			// Block item
			return true;
		}
	}

	// If CSGameRoles was set
	if (g_pGameRules)
	{
		// If player is buying
		if (type == ITEM_TYPE_BUYING)
		{
			// Compare item
			switch (item)
			{
				// HE Grenade
				case ITEM_HEGRENADE:
				{
					// If enabled restrion by round count
					if (gMatchBot.m_RoundGrenadeCount->value > 0.0f)
					{
						// Increment and check limit
						if (Player->m_rgItems[0]++ >= static_cast<int>(gMatchBot.m_RoundGrenadeCount->value))
						{
							// Send blocked message
							gMatchUtil.ClientPrint(Player->edict(), PRINT_CENTER, "#Cstrike_TitlesTXT_Weapon_Not_Available");

							// Block Item
							return true;
						}
					}
					break;
				}
				// Smoke Grenade
				case ITEM_SMOKEGRENADE:
				{
					// If enabled restrion by round count
					if (gMatchBot.m_RoundSmokeCount->value > 0.0f)
					{
						// Increment and check limit
						if (Player->m_rgItems[1]++ >= static_cast<int>(gMatchBot.m_RoundSmokeCount->value))
						{
							// Send blocked message
							gMatchUtil.ClientPrint(Player->edict(), PRINT_CENTER, "#Cstrike_TitlesTXT_Weapon_Not_Available");

							// Block Item
							return true;
						}
					}
					break;
				}
				// Flashbang
				case ITEM_FLASHBANG:
				{
					// If enabled restrion by round count
					if (gMatchBot.m_RoundSmokeCount->value > 0.0f)
					{
						// Increment and check limit
						if (Player->m_rgItems[2]++ >= static_cast<int>(gMatchBot.m_RoundFlashCount->value))
						{
							// Send blocked message
							gMatchUtil.ClientPrint(Player->edict(), PRINT_CENTER, "#Cstrike_TitlesTXT_Weapon_Not_Available");

							// Block Item
							return true;
						}
					}
					break;
				}
			}
		}
	}

	// Allow item
	return false;
}

