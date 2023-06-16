#pragma once

class CMatchData
{
public:
	void ServerActivate();

	static void GetStatus();
	static void GetPlayers();
};

extern CMatchData gMatchData;
