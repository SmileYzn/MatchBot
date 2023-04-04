#include "precompiled.h"

CMatchVoteMap gMatchVoteMap;

void CMatchVoteMap::Init()
{
    this->m_Data.clear();

    this->m_Data = this->Load();

    this->m_PlayerNum = 0;
    this->m_VoteCount = 0;

    auto Players = gMatchUtil.GetPlayers(true, false);

    for (auto const& Player : Players)
    {
        auto EntityIndex = Player->entindex();

        gMatchMenu[EntityIndex].Create(_T("Vote Map:"), false, (void*)this->MenuHandle);

        for (auto const& Item : this->m_Data)
        {
            gMatchMenu[EntityIndex].AddItem(Item.Index, Item.Name);
        }

        this->m_PlayerNum++;

        gMatchMenu[EntityIndex].Show(EntityIndex);
    }

    this->VoteList();

    gMatchTask.Create(TASK_VOTE_LIST, 0.5f, true, (void*)this->UpdateVoteList, TASK_VOTE_LIST);

    gMatchTask.Create(TASK_VOTE_TIMER, 15.0f, false, (void*)this->Stop, 1);

    gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Starting Vote Map."));
}

void CMatchVoteMap::Stop()
{
    auto Players = gMatchUtil.GetPlayers(true, false);

    for (auto const& Player : Players)
    {
        auto EntityIndex = Player->entindex();

        gMatchMenu[EntityIndex].Hide(EntityIndex);
    }

    gMatchVoteMap.VoteList();

    gMatchTask.Delete(TASK_VOTE_LIST);

    gMatchTask.Delete(TASK_VOTE_TIMER);

    auto Winner = gMatchVoteMap.GetWinner();

    if (Winner.Votes > 0)
    {
        gMatchChangeMap.ChangeMap(Winner.Name.c_str(), 5.0f, true);

        gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Changing map to \4%s\1..."), Winner.Name.c_str());
    }
    else
    {
        gMatchTask.Create(TASK_CHANGE_STATE, 2.0f, false, (void*)gMatchBot.NextState, STATE_START);

        gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("The map choice has failed: \3No votes."));
    }
}

// Load Vote Map maps
std::vector<P_MAP_ITEM> CMatchVoteMap::Load()
{
    // Crete Map List to return
    std::vector<P_MAP_ITEM> MapList;

    // Get maps from maps.ini and skip current map
    auto Maps = gMatchUtil.GetMapList(false);

    // Loop loaded map list
    for (auto const& row : Maps)
    {
        // Add to vote pool
        MapList.push_back({ row.first, 0, row.second });
    }

    // Return
    return MapList;
}

void CMatchVoteMap::AddVote(int ItemIndex, int Vote)
{
    this->m_Data[ItemIndex].Votes += Vote;

    this->m_VoteCount++;

    this->VoteList();

    if (this->m_VoteCount >= this->m_PlayerNum)
    {
        this->Stop();
    }
}

void CMatchVoteMap::MenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
    auto Player = UTIL_PlayerByIndexSafe(EntityIndex);
    
    if (Player)
    {
        if (!Item.Disabled)
        {
            gMatchUtil.SayText(nullptr, Player->entindex(), _T("\3%s\1 choosed \3%s\1"), STRING(Player->edict()->v.netname), Item.Text.c_str());

            gMatchVoteMap.AddVote(Item.Info, 1);
        }
    }
}

void CMatchVoteMap::UpdateVoteList(int DummyIndex)
{
    gMatchVoteMap.VoteList();
}

void CMatchVoteMap::VoteList()
{
    std::string VoteList;

    for (auto const& Item : this->m_Data)
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

    gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(0, 255, 0, 0.23, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 1), _T("Vote Map:"));

    gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(255, 255, 225, 0.23, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 2), "\n%s", VoteList.length() ? VoteList.c_str() : _T("No votes..."));
}

P_MAP_ITEM CMatchVoteMap::GetWinner()
{
    auto Winner = this->m_Data[0];

    for (auto const& Item : this->m_Data)
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
