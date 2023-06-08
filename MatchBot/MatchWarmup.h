#pragma once

class CMatchWarmup
{
public:
	void Init();
	void Stop();

	void RemoveMapObjective(bool Remove);

	bool PlayerAddAccount(CBasePlayer* Player, int amount, RewardType type, bool bTrackChange);

private:
	bool m_Running = false;
};

extern CMatchWarmup gMatchWarmup;