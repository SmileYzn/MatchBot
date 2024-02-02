#pragma once

class CMatchPause
{
public:
	// On server activate
	void ServerActivate();

	// Init pause
	void Init(CBasePlayer* Player, TeamName Team);

	// On restart round event
	void RoundRestart();

	//  Pause function
	void Pause();

	// Pause timer hud message 
	static void PauseTimer(int PauseTime);

	// Set Round time
	void SetRoundTime(int Time, bool FreezePeriod);

	bool m_Pause = false;
	bool m_Paused = false;
	std::string m_PauseTeam;
	float m_BuyTime = 0.0f;
};

extern CMatchPause gMatchPause;