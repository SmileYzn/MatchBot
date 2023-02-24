#pragma once

struct P_MAP_ITEM
{
	int Index;
	int Votes;
	std::string Name;
};

class CMatchVoteMap
{
public:
	void Init();
	std::vector<P_MAP_ITEM> Load();
	static void Stop();

	void AddVote(int Item, int Vote);

	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);

	void VoteList();

	P_MAP_ITEM GetWinner();

private:
	std::vector<P_MAP_ITEM> m_Data;

	short m_PlayerNum = 0;
	short m_VoteCount = 0;
};

extern CMatchVoteMap gMatchVoteMap;