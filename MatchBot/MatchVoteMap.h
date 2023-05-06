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
	//
	void Init(int VoteMapType, int VoteMapFail);

	//
	std::vector<P_MAP_ITEM> Load();

	//
	static void Stop(int VoteFailType);

	//
	void AddVote(int Item, int Vote);

	//
	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);

	//
	static void UpdateVoteList(int PlayerCount);

	//
	void VoteList();

	//
	P_MAP_ITEM GetWinner();

	// Change to a random map
	bool ChangeRandomMap();

private:
	std::vector<P_MAP_ITEM> m_Data;

	int m_PlayerNum = 0;
	int m_VoteCount = 0;
	int m_VoteFailType = 0;
};

extern CMatchVoteMap gMatchVoteMap;