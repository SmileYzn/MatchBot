#pragma once

class CMatchTimer
{
public:
	void Init(int PlayersMin, int Delay);
	void Stop(bool ChangeState);

	void SetTime();
	time_t GetTime();

	int GetDelay();

	static void Task(int PlayersMin);

private:
	int m_Delay = 60;
	time_t m_CurrentTime = 0;
};

extern CMatchTimer gMatchTimer;