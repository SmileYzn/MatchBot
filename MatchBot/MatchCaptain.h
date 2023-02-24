#pragma once

#define BIT_CONDITION_LEADER BIT(24)
#define BIT_CONDITION_INMENU BIT(25)

class CMatchCaptain
{
public:
	void Init(int PlayersMin);
	void Stop(int NextState);

	void PlayerDisconnect(edict_t* pEdict);

	void SetCaptain(CBasePlayer* Player, TeamName Team);
	void GetPlayer(CBasePlayer* Captain, CBasePlayer* Target);
	void NextMenu(CBasePlayer* Captain);

	void Menu(CBasePlayer* Captain);
	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);

	static void GetRandomPlayer(int EntityIndex);
	static void PlayerList(int PlayersMin);

private:
	bool m_Running = false;
	int m_PlayersMin = 0;
};

extern CMatchCaptain gMatchCaptain;