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
	if (FunctionCallback != nullptr)
	{
		if (this->m_Data.find(Index) == this->m_Data.end())
		{
			this->m_Data[Index] = { Index, Time, (gpGlobals->time + Time), Loop, FunctionCallback, FunctionParameter};
		}
	}
}

void CMatchTask::Delete(int Index)
{
	this->m_Data.erase(Index);
}

void CMatchTask::Frame()
{
	P_TASK_INFO Task ;

	for (std::map<int, P_TASK_INFO>::iterator it = this->m_Data.begin(); it != this->m_Data.end();)
	{
		if (gpGlobals->time >= it->second.EndTime)
		{
			Task = it->second;

			if (it->second.Loop)
			{
				it->second.EndTime += it->second.Time;

				it++;
			}
			else
			{
				it = this->m_Data.erase(it);
			}

			if (Task.FunctionCallback != nullptr)
			{
				((void(*)(int))Task.FunctionCallback)(Task.FunctionParameter);
			}
		}
		else
		{
			it++;
		}
	}
}