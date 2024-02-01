#pragma once

class CMatchPause
{
public:
	// Init Pause
	void Init(CBasePlayer* Player, TeamName Team);

	// On restart round event
	void RoundRestart();

	// Pause timer hud message 
	static void PauseTimer(int PauseTime);

	// Set Round time
	void SetRoundTime(int Time, bool FreezePeriod);

	bool m_Pause = false;
	const char* m_PauseTeam;
	float m_BuyTime = 0.0f;
};

extern CMatchPause gMatchPause;