#pragma once

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

	void TeamsRandomize();
	void TeamsOptimize();

private:
	std::vector<P_TEAM_ITEM> m_Data;

	int m_PlayerMin = 0;
	int m_PlayerNum = 0;
	int m_VoteCount = 0;
};

extern CMatchVoteTeam gMatchVoteTeam;