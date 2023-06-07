#pragma once

struct P_RR_ITEM
{
	int Index;
	int Votes;
	std::string Name;
};

class CMatchVoteRestart
{
public:
	void Init(int MatchState);
	static void Stop();

	void AddVote(int Item, int Vote);

	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);

	static void UpdateVoteList(int DummyIndex);
	void VoteList();

	P_RR_ITEM GetWinner();

private:
	std::vector<P_RR_ITEM> m_Data;

	short m_PlayerNum = 0;
	short m_VoteCount = 0;
};

extern CMatchVoteRestart gMatchVoteRestart;

