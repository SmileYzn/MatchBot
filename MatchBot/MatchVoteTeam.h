#pragma once

// Flags for team vote menu
constexpr auto TEAM_ALL				= 0;		/* All Options */
constexpr auto TEAM_LEADERS			= BIT(0);   /* flag "a" */
constexpr auto TEAM_RANDOM			= BIT(1);	/* flag "b" */
constexpr auto TEAM_UNSORTED		= BIT(2);	/* flag "c" */
constexpr auto TEAM_SKILL_SORTED	= BIT(3);	/* flag "d" */
constexpr auto TEAM_SWAP_TEAMS		= BIT(4);	/* flag "e" */
constexpr auto TEAM_KNIFE_ROUND		= BIT(5);	/* flag "f" */

struct P_TEAM_ITEM
{
	int Index;
	int Votes;
	std::string Name;
};

class CMatchVoteTeam
{
public:
	void Init(int TeamPickupType, int PlayersMin);
	static void Stop();

	void AddVote(int Item, int Vote);
	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);
	
	static void UpdateVoteList(int PlayerCount);
	void VoteList();

	P_TEAM_ITEM GetWinner();

	void SetMode(int GameMode);
	int  GetMode();

	void TeamsRandomize();
	void TeamsOptimize();

private:
	std::vector<P_TEAM_ITEM> m_Data;

	int m_PlayerMin = 0;
	int m_PlayerNum = 0;
	int m_VoteCount = 0;
	int m_GameMode = 0;
};

extern CMatchVoteTeam gMatchVoteTeam;