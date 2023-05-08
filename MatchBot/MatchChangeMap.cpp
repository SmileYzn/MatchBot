#include "precompiled.h"

CMatchChangeMap gMatchChangeMap;

void CMatchChangeMap::ChangeMap(std::string Map, float Delay, bool IntermissionMsg)
{
	if (!Map.empty())
	{
		this->m_NextMap = Map;

		gMatchTask.Create(TASK_CHANGE_MAP, 5.0f, false, (void*)this->ChangeLevel, 1);

		if (IntermissionMsg)
		{
			if (g_pGameRules)
			{
				auto Players = gMatchUtil.GetPlayers(true, true);

				for (auto& Player : Players)
				{
					Player->edict()->v.flags |= FL_FROZEN;
				}
			}
			
			g_engfuncs.pfnMessageBegin(MSG_ALL, SVC_INTERMISSION, NULL, NULL);
			g_engfuncs.pfnMessageEnd();
		}
	}
}

void CMatchChangeMap::ChangeLevel(int MapIndex)
{
	auto Map = gMatchChangeMap.GetNextMap();

	if (!Map.empty())
	{
		gMatchUtil.ServerCommand("changelevel %s", Map.c_str());
	}
}

std::string CMatchChangeMap::GetNextMap()
{
	return this->m_NextMap;
}
