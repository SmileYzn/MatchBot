#pragma once

class CMatchMute
{
public:
	void Menu(CBasePlayer* Player);
	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);

	bool GetMute(CBasePlayer* Player, CBasePlayer* Target);
	void SetMute(CBasePlayer* Player, CBasePlayer* Target, bool Mute);

private:
	std::unordered_map<std::string, std::map<std::string, bool>> m_Player;
};

extern CMatchMute gMatchMute;