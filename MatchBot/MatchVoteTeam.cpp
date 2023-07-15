#include "precompiled.h"

CMatchVoteTeam gMatchVoteTeam;

void CMatchVoteTeam::Init(int TeamPickupType, int PlayersMin)
{
	// If team pickup method is vote
	if (TeamPickupType == -1)
	{
		// Clear menu data
		this->m_Data.clear();

		// Flags For default
		auto MenuFlags = TEAM_ALL;

		// If variable is not null
		if (gMatchBot.m_TeamPickMenu)
		{
			// If string is not null
			if (gMatchBot.m_TeamPickMenu->string)
			{
				// If string is not empty
				if (gMatchBot.m_TeamPickMenu->string[0] != '\0')
				{
					// Read menu flags from team pickup variable
					MenuFlags |= gMatchAdmin.ReadFlags(gMatchBot.m_TeamPickMenu->string);
				}
			}
		}
		
		this->m_Data.push_back({ 0, 0, _T("Leaders Sorted") });
		this->m_Data.push_back({ 1, 0, _T("Random") });
		this->m_Data.push_back({ 2, 0, _T("Not Sorted") });
		this->m_Data.push_back({ 3, 0, _T("Skill Sorted") });
		this->m_Data.push_back({ 4, 0, _T("Swap Teams") });
		this->m_Data.push_back({ 5, 0, _T("Knife Round") });

		this->m_PlayerMin = PlayersMin;
		this->m_PlayerNum = 0;
		this->m_VoteCount = 0;
		this->m_GameMode = 0;

		auto Players = gMatchUtil.GetPlayers(true, false);

		for (auto const& Player : Players)
		{
			auto EntityIndex = Player->entindex();

			gMatchMenu[EntityIndex].Create(_T("Game Mode:"), false, (void*)this->MenuHandle);

			for (auto const& Item : this->m_Data)
			{
				// If is set to put on menu
				if (MenuFlags == TEAM_ALL || (MenuFlags & BIT(Item.Index)))
				{
					gMatchMenu[EntityIndex].AddItem(Item.Index, Item.Name);
				}
			}

			this->m_PlayerNum++;

			gMatchMenu[EntityIndex].Show(EntityIndex);
		}

		this->VoteList();

		gMatchTask.Create(TASK_VOTE_LIST, 0.5f, true, (void*)this->UpdateVoteList, TASK_VOTE_LIST);

		gMatchTask.Create(TASK_VOTE_TIMER, 15.0f, false, (void*)this->Stop, 1);

		gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Select teams started."));
	}
	else
	{
		this->SetMode(TeamPickupType);
	}
}

void CMatchVoteTeam::Stop()
{
	auto Players = gMatchUtil.GetPlayers(true, false);

	for (auto const& Player : Players)
	{
		auto EntityIndex = Player->entindex();

		gMatchMenu[EntityIndex].Hide(EntityIndex);
	}

	gMatchTask.Remove(TASK_VOTE_LIST);

	gMatchTask.Remove(TASK_VOTE_TIMER);

	auto Winner = gMatchVoteTeam.GetWinner();

	if (Winner.Votes > 0)
	{
		gMatchVoteTeam.SetMode(Winner.Index);
	}
	else
	{
		gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("The choice of the teams failed: ^3No votes."));

		gMatchVoteTeam.SetMode(RANDOM_LONG(1,2));
	}
}

void CMatchVoteTeam::AddVote(int ItemIndex, int Vote)
{
	this->m_Data[ItemIndex].Votes += Vote;

	this->m_VoteCount++;

	this->VoteList();

	if (this->m_VoteCount >= this->m_PlayerNum)
	{
		this->Stop();
	}
}

void CMatchVoteTeam::MenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 choosed ^3%s^1"), STRING(Player->edict()->v.netname), Item.Text.c_str());

		gMatchVoteTeam.AddVote(Item.Info, 1);
	}
}

void CMatchVoteTeam::UpdateVoteList(int PlayerCount)
{
	gMatchVoteTeam.VoteList();
}

void CMatchVoteTeam::VoteList()
{
	std::string VoteList;

	for (auto const & Item : this->m_Data)
	{
		if (Item.Votes > 0)
		{
			VoteList += Item.Name;
			VoteList += " - ";
			VoteList += std::to_string(Item.Votes);
			VoteList += " ";
			VoteList += (Item.Votes > 1) ? _T("votes") : _T("vote");
			VoteList += "\n";
		}
	}

	gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(0, 255, 0, 0.23, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 1), _T("Game Mode:"));

	gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(255, 255, 225, 0.23, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 2), "\n%s", VoteList.length() ? VoteList.c_str() : _T("No votes..."));
}

P_TEAM_ITEM CMatchVoteTeam::GetWinner()
{
	auto Winner = this->m_Data[0];

	for (auto const & Item : this->m_Data)
	{
		if (Item.Votes > Winner.Votes)
		{
			Winner = Item;
		}
		else if (Item.Votes == Winner.Votes)
		{
			if (RANDOM_LONG(0, 1))
			{
				Winner = Item;
			}
		}
	}

	return Winner;
}

void CMatchVoteTeam::SetMode(int GameMode)
{
	this->m_GameMode = GameMode;

	switch (GameMode)
	{
		case 0:
		{
			gMatchCaptain.Init(this->m_PlayerMin);
			return;
		}
		case 1:
		{
			this->TeamsRandomize();

			gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Mixing teams now."));
			break;
		}
		case 2:
		{
			gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("The teams will remain the same."));
			break;
		}
		case 3:
		{
			this->TeamsOptimize();

			gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Balancing teams by skills."));
			break;
		}
		case 4:
		{
			if (g_pGameRules)
			{
				CSGameRules()->SwapAllPlayers();
			}

			gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Swaping teams now."));
			break;
		}
		case 5:
		{
			gMatchBot.SetKnifeRound(true);

			gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Knife Round Enabled"));
			break;
		}
	}

	gMatchTask.Create(TASK_CHANGE_STATE, 3.0f, false, (void*)gMatchBot.NextState, STATE_FIRST_HALF);
}

int CMatchVoteTeam::GetMode()
{
	// Return match game mode
	return m_GameMode;
}

void CMatchVoteTeam::TeamsRandomize()
{
	// Entity Array
	CBasePlayer* Players[MAX_CLIENTS] = { nullptr };

	// Get players that are joined in TR or CT
	auto Num = gMatchUtil.GetPlayers(Players, true);

	// Set first random team (TR or CT)
	TeamName Team = (TeamName)RANDOM_LONG(TERRORIST, CT);

	// While have players count
	while (Num)
	{
		// Get Random index from player count minus one (Like Num)
		auto Random = RANDOM_LONG(0, Num - 1);

		// Pointer
		auto Player = Players[Random];

		// If is not null
		if (Player)
		{
			// Set flag to dead
			Player->edict()->v.deadflag = DEAD_DEAD;

			// Set Team
			Player->CSPlayer()->JoinTeam(Team);

			// Set flag to alive
			Player->edict()->v.deadflag = DEAD_NO;

			// Remove this entity from players count number
			Players[Random] = Players[--Num];

			// Set next team for next random player
			Team = (TeamName)(Team % CT + TERRORIST);
		}
	}

	// If has CSGameRules
	if (g_pGameRules)
	{
		// Restart Round
		CSGameRules()->RestartRound();
	}
}

void CMatchVoteTeam::TeamsOptimize()
{
	// Skills Array
	std::array<float, MAX_CLIENTS + 1> Skills = { 0.0f };

	// Sorted Skills Array
	std::array<float, MAX_CLIENTS + 1> Sorted = { 0.0f };

	// Get players that are joined in TR or CT
	auto Players = gMatchUtil.GetPlayers(true, true);

	// Loop
	for (auto const & Player : Players)
	{
		// Get Frags
		int Frags = Player->edict()->v.frags;

		// Get Deaths
		int Death = Player->m_iDeaths;

		// Store Efficiency
		Sorted[Player->entindex()] = Skills[Player->entindex()] = (Frags ? (100.0f * float(Frags) / float(Frags + Death)) : 0);
	}

	// Sort skill percentage descending in array
	std::sort(Sorted.begin(), Sorted.end(), std::greater<float>());

	// The first random team for most skilled player
	TeamName Team = (TeamName)RANDOM_LONG(TERRORIST, CT);

	// Loop sorted array
	for (size_t i = 0; i < sizeof(Sorted); i++)
	{
		// Loop players
		for (size_t j = 0;j < Players.size(); ++j)
		{
			// Pointer
			auto Player = Players[j];

			// If is not null
			if (Player)
			{
				// If player skill math with sorted array position
				if (Skills[j] == Sorted[i])
				{
					// Set flag to dead
					Player->edict()->v.deadflag = DEAD_DEAD;

					// Set entity team
					Player->CSPlayer()->JoinTeam(Team);

					// Get the next team oposite from current team for next skilled player
					Team = (TeamName)(Team % CT + TERRORIST);

					// Set flag to alive
					Player->edict()->v.deadflag = DEAD_NO;
				}
			}
		}
	}

	// If has CSGameRules
	if (g_pGameRules)
	{
		// Restart Round
		CSGameRules()->RestartRound();
	}
}
