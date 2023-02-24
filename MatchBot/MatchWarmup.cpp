#include "precompiled.h"

CMatchWarmup gMatchWarmup;

void CMatchWarmup::Init()
{
	this->m_Running = true;

	this->RemoveMapObjective(true);

	CVAR_SET_FLOAT("bot_deathmatch", 1.0f);

	gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Deathmatch respawing is enabled until match begin!"));
}

void CMatchWarmup::Stop()
{
	if (this->m_Running)
	{
		this->m_Running = false;

		this->RemoveMapObjective(false);

		CVAR_SET_FLOAT("bot_deathmatch", 0.0f);

		gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Deathmatch respawing disabled."));
	}
}

void CMatchWarmup::RemoveMapObjective(bool Remove)
{
	if (g_ReGameFuncs)
	{
		std::vector<std::vector<std::string>> Targets =
		{
			{"func_bomb_target", "_func_bomb_target"},
			{"info_bomb_target","_info_bomb_target"},
			{"func_hostage_rescue","_func_hostage_rescue"},
			{"info_hostage_rescue","_info_hostage_rescue"},
			{"info_vip_start","_info_vip_start"},
			{"func_vip_safetyzone","_func_vip_safetyzone"},
			{"func_escapezone","_func_escapezone"}
		};

		CBaseEntity* Entity = nullptr;

		for (auto& Name : Targets)
		{
			while ((Entity = g_ReGameFuncs->UTIL_FindEntityByString(Entity, "classname", Remove ? Name[0].c_str() : Name[1].c_str())) != nullptr)
			{
				Entity->edict()->v.classname = ALLOC_STRING(Remove ? Name[1].c_str() : Name[0].c_str());
			}
		}
	}
}

bool CMatchWarmup::PlayerAddAccount(CBasePlayer* Player, int amount, RewardType type, bool bTrackChange)
{
	if (this->m_Running)
	{
		if (type == RT_PLAYER_BOUGHT_SOMETHING)
		{
			return true;
		}
	}

	return false;
}

bool CMatchWarmup::PlayerHasRestrictItem(CBasePlayer* Player, ItemID item, ItemRestType type)
{
	if (this->m_Running)
	{
		if (item == ITEM_SHIELDGUN || item == ITEM_HEGRENADE || item == ITEM_C4 || item == ITEM_SMOKEGRENADE || item == ITEM_FLASHBANG || item == ITEM_DEFUSEKIT)
		{
			if (type == ITEM_TYPE_BUYING)
			{
				gMatchUtil.ClientPrint(Player->edict(), PRINT_CENTER, "#Cstrike_TitlesTXT_Weapon_Not_Available");
			}
			
			return true;
		}
	}

	return false;
}
