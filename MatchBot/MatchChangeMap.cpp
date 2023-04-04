#include "precompiled.h"

CMatchChangeMap gMatchChangeMap;

void CMatchChangeMap::ChangeMap(const char* MapName, float Delay, bool IntermissionMsg)
{
	if (MapName)
	{
		Q_strncpy(this->m_NextMap, MapName, sizeof(this->m_NextMap));

		if (IS_MAP_VALID(this->m_NextMap))
		{
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

				MESSAGE_BEGIN(MSG_ALL, SVC_INTERMISSION);
				MESSAGE_END();
			}
		}
	}
}

void CMatchChangeMap::ChangeLevel(int MapIndex)
{
	char* Map = gMatchChangeMap.GetNextMap();

	if (Map)
	{
		if (IS_MAP_VALID(Map))
		{
			gMatchUtil.ServerCommand("changelevel %s", Map);
		}
	}
}

char* CMatchChangeMap::GetNextMap()
{
	return this->m_NextMap;
}
