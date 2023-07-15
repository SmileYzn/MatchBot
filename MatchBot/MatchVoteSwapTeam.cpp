#include "precompiled.h"

CMatchVoteSwapTeam gMatchVoteSwapTeam;

// Init Vote Swap Team
void CMatchVoteSwapTeam::Init(TeamName Winner)
{
    // Clear Votes
    this->m_Data.clear();

    // Create Menu
    this->m_Data.push_back({ 0, 0, _T("Terrorists") });
    this->m_Data.push_back({ 1, 0, _T("Counter-Terrorists") });

    // Winner Team
    this->m_Team = Winner;

    // Vote Count
    this->m_PlayerNum = 0;
    this->m_VoteCount = 0;

    // Get Players
    auto Players = gMatchUtil.GetPlayers(Winner, false);

    // Loop Players
    for (auto const& Player : Players)
    {
        // Entity Index
        auto EntityIndex = Player->entindex();

        // Create menu for that player
        gMatchMenu[EntityIndex].Create(gMatchUtil.FormatString(_T("%s won, what team you want?"), gMatchBot.GetTeam(Winner, false)), false, (void*)this->MenuHandle);

        // Loop 
        for (auto const& Item : this->m_Data)
        {
            // Add item to menu
            gMatchMenu[EntityIndex].AddItem(Item.Index, Item.Name);
        }

        // Increment player count
        this->m_PlayerNum++;

        //  Display menu to entity
        gMatchMenu[EntityIndex].Show(EntityIndex);
    }

    // If has ReGameDLL API
    if (g_pGameRules)
    {
        // If round is terminating
        if (CSGameRules()->m_bRoundTerminating)
        {
            // Add 15 seconds to round end time
            CSGameRules()->m_flRestartRoundTime = (gpGlobals->time + 15.0f);
        }
    }

    // Update vote list
    gMatchTask.Create(TASK_VOTE_LIST, 0.5f, true, (void*)this->UpdateVoteList, TASK_VOTE_LIST);

    // Create task to end vote
    gMatchTask.Create(TASK_VOTE_TIMER, 10.0f, false, (void*)this->Stop, (int)Winner);

    // Send Message
    gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("%s won Knife Round, swap teams vote started."), gMatchBot.GetTeam(Winner, false));
}

// End Vote
void CMatchVoteSwapTeam::Stop(int WinnerTeam)
{
    // Get Players
    auto Players = gMatchUtil.GetPlayers(true, false);

    // Loop
    for (auto const& Player : Players)
    {
        // Get Entity Index
        auto EntityIndex = Player->entindex();

        // Hide menu on entity
        gMatchMenu[EntityIndex].Hide(EntityIndex);
    }

    // Delete vote list task
    gMatchTask.Remove(TASK_VOTE_LIST);

    // Delete vote task
    gMatchTask.Remove(TASK_VOTE_TIMER);

    // Get winner option
    auto Winner = gMatchVoteSwapTeam.GetWinner();

    // If has votes
    if (Winner.Votes)
    {
        // Fix winner index to TERRORIST or CT constant from TeamName Struct
        // If winner team is the same team of winner option
        if ((Winner.Index + 1) == WinnerTeam)
        {
            // Stay on same teams, send message
            gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("The teams will remain the same."));
        }
        else
        {
            // Swap teams in game
            if (g_pGameRules)
            {
                CSGameRules()->SwapAllPlayers();
            }

            // Send Message
            gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Swaping teams now."));
        }
    }
    else
    {
        // Send Message
        gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("The choice of the teams failed: ^3No votes."));
    }

    // Continue match
    gMatchTask.Create(TASK_CHANGE_STATE, 2.0f, false, (void*)gMatchBot.NextState, STATE_FIRST_HALF);
}

// Add Vote
void CMatchVoteSwapTeam::AddVote(int ItemIndex, int Vote)
{
    this->m_Data[ItemIndex].Votes += Vote;

    this->m_VoteCount++;

    if (this->m_VoteCount >= this->m_PlayerNum)
    {
        this->Stop((int)this->m_Team);
    }
}

void CMatchVoteSwapTeam::MenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
    auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

    if (Player)
    {
        if (!Item.Disabled)
        {
            gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 choosed ^3%s^1"), STRING(Player->edict()->v.netname), Item.Text.c_str());

            gMatchVoteSwapTeam.AddVote(Item.Info, 1);
        }
    }
}

void CMatchVoteSwapTeam::UpdateVoteList(int DummyIndex)
{
    gMatchVoteSwapTeam.VoteList();
}

void CMatchVoteSwapTeam::VoteList()
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

    gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(0, 255, 0, 0.23, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 1), _T("Swap Teams Vote:"));

    gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(255, 255, 225, 0.23, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 2), "\n%s", VoteList.length() ? VoteList.c_str() : _T("No votes..."));
}

P_SWAP_TEAM_ITEM CMatchVoteSwapTeam::GetWinner()
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
