#pragma once

constexpr auto MINIMUM_REPORT_REASON_LENGTH = 10;

typedef struct S_REPORT_DATA
{
	std::string Player;
	int PlayerTeam;

	std::string Target;
	int TargetTeam;

	time_t Time;
	std::string Reason;
} P_REPORT_DATA, *LP_REPORT_DATA;

class CMatchReport
{
public:
	void ServerActivate();

	bool Report(CBasePlayer* Player);
	void Menu(CBasePlayer* Player, std::string Reason);
	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);

private:
	std::vector<P_REPORT_DATA> m_Data;
	std::map<int, std::string> m_Reason;
};

extern CMatchReport gMatchReport;