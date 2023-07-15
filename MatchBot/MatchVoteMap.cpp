#include "precompiled.h"

CMatchVoteMap gMatchVoteMap;

// Start vote map
void CMatchVoteMap::Init(int VoteMapType, int VoteMapFail)
{
    // Clear map data
    this->m_Data.clear();

    // Load maps data
    this->m_Data = this->Load();

    // Set player count to 0
    this->m_PlayerNum = 0;

    // Set vote count to 0
    this->m_VoteCount = 0;

    // Set vote map fail type
    this->m_VoteFailType = VoteMapFail;

    // If vote map is enabled
    if (VoteMapType == 1)
    {
        // Get players
        auto Players = gMatchUtil.GetPlayers(true, false);

        // Loop players
        for (auto const& Player : Players)
        {
            // Get entity index
            auto EntityIndex = Player->entindex();

            // Create vote menu
            gMatchMenu[EntityIndex].Create(_T("Vote Map:"), false, (void*)this->MenuHandle);

            // Loop map items
            for (auto const& Item : this->m_Data)
            {
                // Add to vote map menu
                gMatchMenu[EntityIndex].AddItem(Item.Index, Item.Name);
            }

            // Increment player count
            this->m_PlayerNum++;

            // Display vote map menu
            gMatchMenu[EntityIndex].Show(EntityIndex);
        }

        // Vote map list task
        gMatchTask.Create(TASK_VOTE_LIST, 0.5f, true, (void*)this->UpdateVoteList, TASK_VOTE_LIST);

        // Create stop task
        gMatchTask.Create(TASK_VOTE_TIMER, 15.0f, false, (void*)this->Stop, this->m_VoteFailType);

        // Send message
        gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Starting Vote Map."));
    }
    else
    {
        // Change to a random map
        this->ChangeRandomMap();
    }
}

// Stop vote map
void CMatchVoteMap::Stop(int VoteFailType)
{
    // Get all players
    auto Players = gMatchUtil.GetPlayers(true, false);

    // Loop
    for (auto const& Player : Players)
    {
        // Get entity index
        auto EntityIndex = Player->entindex();

        // Hide menu
        gMatchMenu[EntityIndex].Hide(EntityIndex);
    }

    // Delete vote list tas
    gMatchTask.Remove(TASK_VOTE_LIST);

    // Delete vote timer
    gMatchTask.Remove(TASK_VOTE_TIMER);

    auto Winner = gMatchVoteMap.GetWinner();

    if (Winner.Votes > 0)
    {
        g_engfuncs.pfnCvar_DirectSet(gMatchBot.m_VoteMap, "0");

        gMatchChangeMap.ChangeMap(Winner.Name, 5.0f, true);

        gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Changing map to ^4%s^1..."), Winner.Name.c_str());
    }
    else
    {
        // Send vote failed message
        gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("The map choice has failed: ^3No votes."));

        // What to do if vote map fail
        switch (VoteFailType)
        {
            case 1: // Restart vote map
            {
                gMatchBot.StartVoteMap(nullptr);
                break;
            }
            case 2: // Choose a random map
            {
                gMatchVoteMap.ChangeRandomMap();
                break;
            }
            default: // No action, continue match
            {
                gMatchTask.Create(TASK_CHANGE_STATE, 2.0f, false, (void*)gMatchBot.NextState, STATE_START);
                break;
            }
        }
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
    for (auto const& Map : Maps)
    {
        // Add to vote pool
        MapList.push_back({ Map.first, 0, Map.second });
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
        this->Stop(this->m_VoteFailType);
    }
}

void CMatchVoteMap::MenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
    auto Player = UTIL_PlayerByIndexSafe(EntityIndex);
    
    if (Player)
    {
        if (!Item.Disabled)
        {
            gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 choosed ^3%s^1"), STRING(Player->edict()->v.netname), Item.Text.c_str());

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

    gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(255, 255, 225, 0.23, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 2), "\n%s", (!VoteList.empty() && VoteList.length() > 0) ? VoteList.c_str() : _T("No votes..."));
}

P_MAP_ITEM CMatchVoteMap::GetWinner()
{
    P_MAP_ITEM Winner = {0, 0, "de_dust2"};

    if (!this->m_Data.empty())
    {
        Winner = this->m_Data.front();

        for (auto const& Item : this->m_Data)
        {
            if (Item.Votes > Winner.Votes)
            {
                Winner = Item;
            }
            else if (Item.Votes == Winner.Votes)
            {
                if (g_engfuncs.pfnRandomLong(0, 1))
                {
                    Winner = Item;
                }
            }
        }
    }

    return Winner;
}

bool CMatchVoteMap::ChangeRandomMap()
{
    // Load map list from maps.ini skipping current map
    auto MapList = gMatchUtil.GetMapList(false);

    // If list is empty
    if (MapList.empty())
    {
        // Fill with default maps
        MapList.insert(std::make_pair(0, "de_dust2"));
        MapList.insert(std::make_pair(1, "de_inferno"));
        MapList.insert(std::make_pair(2, "de_nuke"));
        MapList.insert(std::make_pair(3, "de_train"));
    }

    // Get first item of list
    auto Item = MapList.begin();

    // Advance to a random map position
    std::advance(Item, g_engfuncs.pfnRandomLong(0, MapList.size()));

    // Check if map name is not empty
    if (!Item->second.empty())
    {
        // Dump Map Name to pointer
        auto Map = Q_strdup(Item->second.data());

        // Check if is a valid map
        if (g_engfuncs.pfnIsMapValid(Map))
        {
            // Send message to players
            gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Changing map to ^4%s^1..."), Map);

            // Remove Vote Map Variable
            g_engfuncs.pfnCvar_DirectSet(gMatchBot.m_VoteMap, "0");

            // Change map
            gMatchChangeMap.ChangeMap(Item->second, 5.0f, true);

            // Return Result
            return true;
        }
    }

    // Try to change to a random map until this function returns true
    this->ChangeRandomMap();

    // Return Result
    return false;
}