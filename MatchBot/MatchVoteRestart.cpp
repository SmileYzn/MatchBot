#include "precompiled.h"

CMatchVoteRestart gMatchVoteRestart;

void CMatchVoteRestart::Init(int MatchState)
{
    // Do not init vote restart 
    bool StartVoteRestart = false;

    // Check if an team is lacking the count of players
    if (gMatchBot.m_PlayersMinCheck->value > 0.0f)
    {
        // Get Player Count in teams
        auto PlayerCount = gMatchUtil.GetCount();

        // Get minimum of players in server variable divided by two teams (TRs and CTs) 
        auto MinPlayers = (int)((gMatchBot.m_PlayersMin->value / 2.0f) - gMatchBot.m_PlayersMinCheck->value);

        // If is lacking Terrorist or CTs players
        if (PlayerCount[TERRORIST] <= MinPlayers || PlayerCount[CT] <= MinPlayers)
        {
            // Start Vote 
            StartVoteRestart = true;
        }
    }

    // If is needed to start vote
    if (StartVoteRestart)
    {
        this->m_Data.clear();

        this->m_Data.push_back({ 0, 0, _T("Keep Playing") });
        this->m_Data.push_back({ 1, 0, gMatchUtil.FormatString("%s %s", _T("Restart"), gMatchBot.GetState(MatchState))});
        this->m_Data.push_back({ 2, 0, _T("Cancel Match") });

        this->m_PlayerNum = 0;
        this->m_VoteCount = 0;

        auto Players = gMatchUtil.GetPlayers(true, false);

        for (auto const& Player : Players)
        {
            auto EntityIndex = Player->entindex();

            gMatchMenu[EntityIndex].Create(_T("The Server detected missing players in the first round,\nwhat do you want to do?"), false, (void*)this->MenuHandle);

            for (auto const& Item : this->m_Data)
            {
                gMatchMenu[EntityIndex].AddItem(Item.Index, Item.Name);
            }

            this->m_PlayerNum++;

            gMatchMenu[EntityIndex].Show(EntityIndex);
        }

        if (g_pGameRules)
        {
            if (CSGameRules()->m_bRoundTerminating)
            {
                CSGameRules()->m_flRestartRoundTime = (gpGlobals->time + 15.0f);
            }
        }

        this->VoteList();

        gMatchTask.Create(TASK_VOTE_LIST, 0.5f, true, (void*)this->UpdateVoteList, TASK_VOTE_LIST);

        gMatchTask.Create(TASK_VOTE_TIMER, 10.0f, false, (void*)this->Stop, 1);

        gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("The Server detected missing players in the first round, what do you want to do?"));
    }
}

void CMatchVoteRestart::Stop()
{
    auto Players = gMatchUtil.GetPlayers(true, false);

    for (auto const& Player : Players)
    {
        auto EntityIndex = Player->entindex();

        gMatchMenu[EntityIndex].Hide(EntityIndex);
    }

    gMatchTask.Remove(TASK_VOTE_LIST);

    gMatchTask.Remove(TASK_VOTE_TIMER);

    auto Winner = gMatchVoteRestart.GetWinner();

    if (Winner.Votes > 0)
    {
        switch (Winner.Index)
        {
            case 0: // Keep Playing
            {
                gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Match will continue without restart..."));
                break;
            }
            case 1: // Restart Round
            {
                gMatchBot.RestartMatch(nullptr);
                break;
            }
            case 2: // Cancel Match
            {
                gMatchBot.StopMatch(nullptr);
                break;
            }
        }
    }
    else
    {
        gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("The vote for restart choice failed: \3No votes."));
    }
}

void CMatchVoteRestart::AddVote(int ItemIndex, int Vote)
{
    this->m_Data[ItemIndex].Votes += Vote;

    this->m_VoteCount++;

    this->VoteList();

    if (this->m_VoteCount >= this->m_PlayerNum)
    {
        this->Stop();
    }
}

void CMatchVoteRestart::MenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
    auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

    if (Player)
    {
        if (!Item.Disabled)
        {
            gMatchUtil.SayText(nullptr, Player->entindex(), _T("\3%s\1 choosed \3%s\1"), STRING(Player->edict()->v.netname), Item.Text.c_str());

            gMatchVoteRestart.AddVote(Item.Info, 1);
        }
    }
}

void CMatchVoteRestart::UpdateVoteList(int DummyIndex)
{
    gMatchVoteRestart.VoteList();
}

void CMatchVoteRestart::VoteList()
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

    gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(0, 255, 0, 0.23, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 1), _T("Vote For Restart:"));

    gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(255, 255, 225, 0.23, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 2), "\n%s", VoteList.length() ? VoteList.c_str() : _T("No votes..."));
}

P_RR_ITEM CMatchVoteRestart::GetWinner()
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
