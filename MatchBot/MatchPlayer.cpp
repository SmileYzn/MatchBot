#include "precompiled.h"

CMatchPlayer gMatchPlayer;

// On Server Activate
void CMatchPlayer::ServerActivate()
{
	// Clear Player Data
	this->m_Player.clear();
}

// On Player Connect
bool CMatchPlayer::PlayerConnect(edict_t* pEdict, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	// If is not null entity
	if (!FNullEnt(pEdict))
	{
		// Get Auth Index
		auto Auth = g_engfuncs.pfnGetPlayerAuthId(pEdict);

		// If is not null
		if (Auth)
		{
			// If is not HLTV
			if (!((pEdict->v.flags & FL_PROXY) == FL_PROXY))
			{
				// Name
				this->m_Player[Auth].Name = (pszName) ? pszName : "";

				// Address
				this->m_Player[Auth].Address = (pszAddress) ? pszAddress : "";

				// Flags
				this->m_Player[Auth].Flags = gMatchAdmin.GetFlags(pEdict);

				// User Index
				this->m_Player[Auth].UserIndex = g_engfuncs.pfnGetPlayerUserId(pEdict);

				// Status
				this->m_Player[Auth].Status = 1;

				// Last Team
				this->m_Player[Auth].LastTeam = TeamName::UNASSIGNED;
			}
		}
	}

	// Allow Connection
	return true;
}

// On Player Join Game
void CMatchPlayer::PlayerGetIntoGame(CBasePlayer* Player)
{
	// Get Auth Index
	auto Auth = g_engfuncs.pfnGetPlayerAuthId(Player->edict());

	// If is not null
	if (Auth)
	{
		// Name
		this->m_Player[Auth].Name = STRING(Player->edict()->v.netname);

		// Set Team
		this->m_Player[Auth].LastTeam = Player->m_iTeam;

		// Status
		this->m_Player[Auth].Status = 2;
	}
}

// On Player Change Team
void CMatchPlayer::PlayerSwitchTeam(CBasePlayer* Player)
{
	// Get Auth Index
	auto Auth = g_engfuncs.pfnGetPlayerAuthId(Player->edict());

	// If is not null
	if (Auth)
	{
		// Name
		this->m_Player[Auth].Name = STRING(Player->edict()->v.netname);

		// Set Team
		this->m_Player[Auth].LastTeam = Player->m_iTeam;

		// Status
		this->m_Player[Auth].Status = 2;
	}
}

// On Player Disconnect
void CMatchPlayer::PlayerDisconnect(edict_t* pEdict)
{
	// If is not null entity
	if (!FNullEnt(pEdict))
	{
		// Get Auth Index
		auto Auth = g_engfuncs.pfnGetPlayerAuthId(pEdict);

		// If is not null
		if (Auth)
		{
			// If is not HLTV
			if (!((pEdict->v.flags & FL_PROXY) == FL_PROXY))
			{
				// Status
				this->m_Player[Auth].Status = 0;
			}
		}
	}
}

// Player Menu
void CMatchPlayer::PlayerMenu(int EntityIndex)
{
	auto Flags = gMatchAdmin.GetFlags(EntityIndex);

	if (!(Flags & ADMIN_MENU))
	{
		gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	if (!this->m_Player.empty())
	{
		gMatchMenu[EntityIndex].Create(gMatchUtil.FormatString(_T("Player List (Size %zu)"), this->m_Player.size()), true, (void*)this->PlayerMenuHandle);

		for (auto const& Player : this->m_Player)
		{
			gMatchMenu[EntityIndex].AddItem(Player.second.UserIndex, gMatchUtil.FormatString("%s \\R%s", Player.second.Name.c_str(), Player.second.Status ? "\\yON" : "\\rOFF"), false, Player.second.UserIndex);
		}

		gMatchMenu[EntityIndex].Show(EntityIndex);
	}
	else
	{
		gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("Player List is empty..."));
	}
}

// Payer Menu Handler
void CMatchPlayer::PlayerMenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		gMatchPlayer.ShowInfo(Player, Item.Info);
	}
}

// Show Player Info
void CMatchPlayer::ShowInfo(CBasePlayer* Player,int UserIndex)
{
	for (auto const& User : this->m_Player)
	{
		if (UserIndex == User.second.UserIndex)
		{
			char Info[512] = { 0 };

			Q_snprintf
			(
				Info,
				sizeof(Info),
				"\\y %s Information:\\w\n\nSteam: \\y%s\\w\nIP: \\y%s\\w\nFlags: \\y%s\\w\nTeam: \\y%s\\w\nUser Id: \\y%d\\w\nStatus: \\y%s",
				User.second.Name.c_str(),
				User.first.c_str(),
				User.second.Address.c_str(),
				User.second.Flags.c_str(),
				gMatchBot.GetTeam(User.second.LastTeam,false),
				User.second.UserIndex,
				(User.second.Status > 0) ? (User.second.Status == 1 ? "Connected" : "In Game") : "Disconnected"
			);

			gMatchMenu[Player->entindex()].ShowMenu(Player->entindex(), MENU_KEY_0, -1, Info);
		}
	}
}