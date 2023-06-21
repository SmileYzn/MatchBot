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
	for (auto it = this->m_Data.cbegin(); it != this->m_Data.cend(); ++it)
	{
		this->Remove(it->first);
	}
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

void CMatchTask::Frame()
{
	for (auto it = this->m_Data.cbegin(); it != this->m_Data.cend(); ++it)
	{
		if (gpGlobals->time >= it->second.EndTime)
		{
			if (!it->second.Free)
			{
				if (it->second.Loop)
				{
					this->m_Data[it->first].EndTime = (gpGlobals->time + it->second.Time);
				}
				else
				{
					this->m_Data[it->first].Free = true;
				}

				if (this->m_Data[it->first].FunctionCallback)
				{
					((void(*)(int))this->m_Data[it->first].FunctionCallback)(this->m_Data[it->first].FunctionParameter);
				}
			}
			else
			{
				this->m_Data.erase(it->first);
			}
		}
	}
}