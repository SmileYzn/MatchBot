#pragma once

#pragma once

struct P_SWAP_TEAM_ITEM
{
	int Index;
	int Votes;
	std::string Name;
};

class CMatchVoteSwapTeam
{
public:
	void Init(TeamName Winner);
	static void Stop(int WinnerTeam);

	void AddVote(int Item, int Vote);

	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);

	static void UpdateVoteList(int DummyIndex);
	void VoteList();

	P_SWAP_TEAM_ITEM GetWinner();

private:
	std::vector<P_SWAP_TEAM_ITEM> m_Data;

	TeamName m_Team = UNASSIGNED;
	short m_PlayerNum = 0;
	short m_VoteCount = 0;
};

extern CMatchVoteSwapTeam gMatchVoteSwapTeam;