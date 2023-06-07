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
		if (this->m_Data.find(Index) != this->m_Data.end())
		{
			this->m_Data[Index].Time = Time;

			this->m_Data[Index].EndTime = (gpGlobals->time + Time);

			this->m_Data[Index].Loop = Loop;

			this->m_Data[Index].FunctionParameter = FunctionParameter;
		}
		else
		{
			this->m_Data[Index] = { Index, Time, (gpGlobals->time + Time), Loop, FunctionCallback, FunctionParameter };
		}
	}
}

void CMatchTask::EndTime(int Index, float EndTime)
{
	if (this->m_Data.find(Index) != this->m_Data.end())
	{
		this->m_Data[Index].EndTime = EndTime;
	}
}

void CMatchTask::Remove(int Index)
{
	if (this->m_Data.find(Index) != this->m_Data.end())
	{
		this->m_Data.erase(Index);
	}
}

void CMatchTask::Execute(int Index)
{
	if (this->m_Data.find(Index) != this->m_Data.end())
	{
		P_TASK_INFO Task = this->m_Data[Index];

		if (Task.FunctionCallback)
		{
			((void(*)(int))Task.FunctionCallback)(Task.FunctionParameter);
		}
	}
}

void CMatchTask::Frame()
{
	for (auto it = this->m_Data.cbegin(); it != this->m_Data.cend(); ++it)
	{
		if (gpGlobals->time >= it->second.EndTime)
		{
			if (it->second.Loop)
			{
				this->EndTime(it->first, (gpGlobals->time + it->second.Time));

				this->Execute(it->first);
			}
			else
			{
				this->Execute(it->first);

				this->Remove(it->first);
			}
		}
	}
}