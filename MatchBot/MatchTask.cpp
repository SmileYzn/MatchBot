#include "precompiled.h"

CMatchTask gMatchTask;

void CMatchTask::ServerActivate()
{
	this->Clear();
}

void CMatchTask::ServerDeactivate()
{
	this->Clear();
}

void CMatchTask::Clear()
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
		this->m_Data[Index].Remove = true;
	}
}

void CMatchTask::Frame()
{
	for (std::map<int, P_TASK_INFO>::iterator it = this->m_Data.begin(); it != this->m_Data.end(); )
	{
		if (gpGlobals->time >= it->second.EndTime)
		{
			if (it->second.Loop)
			{
				it->second.EndTime += it->second.Time;
			}
			else
			{
				it->second.Remove = true;
			}

			if (it->second.FunctionCallback)
			{
				((void(*)(int))it->second.FunctionCallback)(it->second.FunctionParameter);
			}
		}

		if (it->second.Remove)
		{
			this->m_Data.erase(it++);
		}
		else
		{
			it++;
		}
	}
}