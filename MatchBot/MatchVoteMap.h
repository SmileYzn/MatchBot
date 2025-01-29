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
	// Init vote map
	void Init(int VoteMapType, int VoteMapFail);

	// Return loaded map list
	std::vector<P_MAP_ITEM> Load();

	// Stop vote map
	static void Stop(int VoteFailType);

	// Increment vote
	void AddVote(int Item, int Vote);

	// Vote menu handler
	static void MenuHandle(int EntityIndex, P_MENU_ITEM Item);

	// Updte vote hud list message task
	static void UpdateVoteList(int PlayerCount);

	// Updte vote hud list message
	void VoteList();

	// Get map winner of vote map
	P_MAP_ITEM GetWinner();

	// Change to a random map
	void ChangeRandomMap();

private:
	std::vector<P_MAP_ITEM> m_Data;

	int m_PlayerNum = 0;
	int m_VoteCount = 0;
	int m_VoteFailType = 0;
};

extern CMatchVoteMap gMatchVoteMap;