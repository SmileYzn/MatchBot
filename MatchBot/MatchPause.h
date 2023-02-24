#pragma once

#define MATCH_PAUSE_TIME 60

class CMatchPause
{
public:
	void Init(CBasePlayer* Player);

	void RoundRestart();

	static void PauseTimer(int PauseTime);

	void SetRoundTime(int Time, bool FreezePeriod);
private:
	bool m_Pause = false;
};

extern CMatchPause gMatchPause;