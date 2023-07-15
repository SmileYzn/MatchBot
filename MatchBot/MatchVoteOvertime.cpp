#include "precompiled.h"

CMatchVoteOvertime gMatchVoteOvertime;

void CMatchVoteOvertime::Init()
{
    this->m_Data.clear();

    this->m_Data.push_back({ 0, 0, _T("Play Overtime") });
    this->m_Data.push_back({ 1, 0, _T("Play sudden death round") });
    this->m_Data.push_back({ 2, 0, _T("End match with tied scores") });

    this->m_PlayerNum = 0;
    this->m_VoteCount = 0;

    auto Players = gMatchUtil.GetPlayers(true, false);

    for (auto const& Player : Players)
    {
        auto EntityIndex = Player->entindex();

        gMatchMenu[EntityIndex].Create(_T("Match is tied, what you want to do?"), false, (void*)this->MenuHandle);

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

    gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Match is tied, Overtime vote started."));
}

void CMatchVoteOvertime::Stop()
{
    auto Players = gMatchUtil.GetPlayers(true, false);

    for (auto const& Player : Players)
    {
        auto EntityIndex = Player->entindex();

        gMatchMenu[EntityIndex].Hide(EntityIndex);
    }

    gMatchTask.Remove(TASK_VOTE_LIST);

    gMatchTask.Remove(TASK_VOTE_TIMER);

    auto Winner = gMatchVoteOvertime.GetWinner();

    if (Winner.Votes > 0)
    {
        switch (Winner.Index)
        {
            case 0:
            {
                gMatchBot.SetState(STATE_OVERTIME);
                break;
            }
            case 1:
            {
                if (g_pGameRules)
                {
                    CSGameRules()->m_bGameStarted = true;
                }

                gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("Match will continue in: ^3Sudden Death Round."));
                break;
            }
            case 2:
            {
                gMatchBot.SetState(STATE_END);
                break;
            }
        }
    }
    else
    {
        gMatchUtil.SayText(nullptr, PRINT_TEAM_DEFAULT, _T("The choice of overtime failed: ^3No votes."));

        gMatchBot.SetState(STATE_END);
    }
}

void CMatchVoteOvertime::AddVote(int ItemIndex, int Vote)
{
    this->m_Data[ItemIndex].Votes += Vote;

    this->m_VoteCount++;

    this->VoteList();

    if (this->m_VoteCount >= this->m_PlayerNum)
    {
        this->Stop();
    }
}

void CMatchVoteOvertime::MenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
    auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

    if (Player)
    {
        if (!Item.Disabled)
        {
            gMatchUtil.SayText(nullptr, Player->entindex(), _T("^3%s^1 choosed ^3%s^1"), STRING(Player->edict()->v.netname), Item.Text.c_str());

            gMatchVoteOvertime.AddVote(Item.Info, 1);
        }
    }
}

void CMatchVoteOvertime::UpdateVoteList(int DummyIndex)
{
    gMatchVoteOvertime.VoteList();
}

void CMatchVoteOvertime::VoteList()
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

    gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(0, 255, 0, 0.23, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 1), _T("Overtime Vote:"));

    gMatchUtil.HudMessage(NULL, gMatchUtil.HudParam(255, 255, 225, 0.23, 0.02, 0, 0.0, 0.8, 0.0, 0.0, 2), "\n%s", VoteList.length() ? VoteList.c_str() : _T("No votes..."));
}

P_OT_ITEM CMatchVoteOvertime::GetWinner()
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
