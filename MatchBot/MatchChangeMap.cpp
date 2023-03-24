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
	auto NextMap = gMatchChangeMap.GetNextMap();

	if (!NextMap)
	{
		LOG_CONSOLE(PLID,"[%s] changelevel %s", __func__, NextMap);
		
		//gMatchUtil.ServerCommand("changelevel %s", Map.c_str());
	}
}

const char* CMatchChangeMap::GetNextMap()
{
	if(!this->m_NextMap.empty())
	{
		return this->m_NextMap.c_str();
	}
	
	return nullptr;
}
