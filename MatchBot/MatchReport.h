#pragma once

constexpr auto REPORT_REASON_MININUM_LENGTH = 10;

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
	bool Menu(CBasePlayer* Player);
	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);
	void Insert(CBasePlayer* Player, CBasePlayer* Target);

private:
	std::vector<P_REPORT_DATA> m_Data;
	std::map<int, std::string> m_Reason;
};

extern CMatchReport gMatchReport;