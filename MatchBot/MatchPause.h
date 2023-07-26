#pragma once

class CMatchPause
{
public:
	void Init(CBasePlayer* Player);

	void RoundRestart();

	static void PauseTimer(int PauseTime);

	void SetRoundTime(int Time, bool FreezePeriod);
	
private:
	bool	m_Pause   = false;
	float	m_BuyTime = 0.0f;
};

extern CMatchPause gMatchPause;