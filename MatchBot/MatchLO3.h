#pragma once

class CMatchLO3
{
public:
	void Start();
	void RoundRestart();

private:
	int m_Restart = 0;
};

extern CMatchLO3 gMatchLO3;