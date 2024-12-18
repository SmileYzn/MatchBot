#include "precompiled.h"

CMatchTask gMatchTask;

void CMatchTask::ServerActivate()
{
	this->m_Data.clear();
}

void CMatchTask::ServerDeactivate()
{
	this->m_Data.clear();
}

void CMatchTask::Create(int Index, float Time, bool Loop, void* FunctionCallback, int FunctionParameter)
{
	if (FunctionCallback)
	{
		this->m_Data[Index] = { Index, Time, (gpGlobals->time + Time), Loop, false, FunctionCallback, FunctionParameter };
	}
}

void CMatchTask::Remove(int Index)
{
	if (this->m_Data.find(Index) != this->m_Data.end())
	{
		this->m_Data[Index].Free = true;
	}
}

void CMatchTask::ServerFrame()
{
	for (auto it = this->m_Data.begin(); it != this->m_Data.end(); it++)
	{
		if (gpGlobals->time >= it->second.EndTime)
		{
			if (!it->second.Free)
			{
				if (it->second.Loop)
				{
					it->second.EndTime = (gpGlobals->time + it->second.Time);
				}
				else
				{
					it->second.Free = true;
				}

				if (it->second.FunctionCallback)
				{
					((void(*)(int))it->second.FunctionCallback)(it->second.FunctionParameter);
				}
			}
		}
	}
}