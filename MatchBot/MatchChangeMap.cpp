#include "precompiled.h"

CMatchChangeMap gMatchChangeMap;

void CMatchChangeMap::ChangeMap(std::string MapName, float Delay, bool IntermissionMsg)
{
	if (!MapName.empty())
	{
		this->m_NextMap = MapName;

		gMatchTask.Create(TASK_CHANGE_MAP, 5.0f, false, (void*)this->ChangeLevel, RANDOM_LONG(0, 1));

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

			MESSAGE_BEGIN(MSG_ALL, SVC_INTERMISSION);
			MESSAGE_END();
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
