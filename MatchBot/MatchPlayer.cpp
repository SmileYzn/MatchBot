#include "precompiled.h"

CMatchPlayer gMatchPlayer;

// On Server Activate
void CMatchPlayer::ServerActivate()
{
	// Clear Player Data
	this->m_Player.clear();
}

// Get Player Info
LP_PLAYER_INFO CMatchPlayer::GetInfo(const char* AuthId)
{
	// If is not null
	if (AuthId)
	{
		// If has data
		if (AuthId[0u] != '\0')
		{
			// Try to find player by auth id
			if (this->m_Player.find(AuthId) != this->m_Player.end())
			{
				// Return player data
				return &this->m_Player[AuthId];
			}
		}
	}

	// Null
	return nullptr;
}

// Get Player Info
LP_PLAYER_INFO CMatchPlayer::GetInfo(int UserIndex)
{
	// If player list is not empty
	if (!this->m_Player.empty())
	{
		// Loop list
		for (auto const& Player : this->m_Player)
		{
			// If user index is equal
			if (Player.second.UserId == UserIndex)
			{
				// Get Info by auth id
				auto PlayerInfo = this->GetInfo(Player.first.c_str());

				// If is not null
				if (PlayerInfo)
				{
					// Return information
					return PlayerInfo;
				}
			}
		}
	}
	
	// Null
	return nullptr;
}

// On Player Connect
bool CMatchPlayer::PlayerConnect(edict_t* pEdict, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	// If is not null entity
	if (!FNullEnt(pEdict))
	{
		// Get Auth Index
		auto AuthId = g_engfuncs.pfnGetPlayerAuthId(pEdict);

		// If is not null
		if (AuthId)
		{
			// If is not HLTV
			if (!((pEdict->v.flags & FL_PROXY) == FL_PROXY))
			{
				// User Index
				this->m_Player[AuthId].UserId = g_engfuncs.pfnGetPlayerUserId(pEdict);

				// Auth
				this->m_Player[AuthId].Auth = AuthId;

				// Name
				this->m_Player[AuthId].Name = (pszName != nullptr) ? pszName : "";

				// Address
				this->m_Player[AuthId].Address = (pszAddress != nullptr) ? pszAddress : "";

				// Admin Flags
				this->m_Player[AuthId].Flags = gMatchAdmin.GetFlags(pEdict);

				// Admin Flags
				this->m_Player[AuthId].AdminFlags = gMatchAdmin.GetFlags(ENTINDEX(pEdict));

				// Status
				this->m_Player[AuthId].Status = 1;

				// Last Team
				this->m_Player[AuthId].LastTeam = TeamName::UNASSIGNED;
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
	auto AuthId = g_engfuncs.pfnGetPlayerAuthId(Player->edict());

	// If is not null
	if (AuthId)
	{
		// User Index
		this->m_Player[AuthId].UserId = g_engfuncs.pfnGetPlayerUserId(Player->edict());

		// Auth
		this->m_Player[AuthId].Auth = AuthId;

		// Name
		this->m_Player[AuthId].Name = STRING(Player->edict()->v.netname);

		// Address
		this->m_Player[AuthId].Address = Player->IsBot() ? "loopback" : this->m_Player[AuthId].Address;

		// Admin Flags
		this->m_Player[AuthId].Flags = gMatchAdmin.GetFlags(Player->edict());

		// Admin Flags
		this->m_Player[AuthId].AdminFlags = gMatchAdmin.GetFlags(Player->entindex());

		// Status
		this->m_Player[AuthId].Status = 2;

		// Last Team
		this->m_Player[AuthId].LastTeam = Player->m_iTeam;
	}
}

// On Player Change Team
void CMatchPlayer::PlayerSwitchTeam(CBasePlayer* Player)
{
	// Get Auth Index
	auto AuthId = g_engfuncs.pfnGetPlayerAuthId(Player->edict());

	// If is not null
	if (AuthId)
	{
		// User Index
		this->m_Player[AuthId].UserId = g_engfuncs.pfnGetPlayerUserId(Player->edict());

		// Auth
		this->m_Player[AuthId].Auth = AuthId;

		// Name
		this->m_Player[AuthId].Name = STRING(Player->edict()->v.netname);

		// Address
		this->m_Player[AuthId].Address = Player->IsBot() ? "loopback" : this->m_Player[AuthId].Address;

		// Admin Flags
		this->m_Player[AuthId].Flags = gMatchAdmin.GetFlags(Player->edict());

		// Admin Flags
		this->m_Player[AuthId].AdminFlags = gMatchAdmin.GetFlags(Player->entindex());

		// Status
		this->m_Player[AuthId].Status = 2;

		// Last Team
		this->m_Player[AuthId].LastTeam = Player->m_iTeam;
	}
}

// On Player Disconnect
void CMatchPlayer::PlayerDisconnect(edict_t* pEdict)
{
	// If is not null entity
	if (!FNullEnt(pEdict))
	{
		// Get Auth Index
		auto AuthId = g_engfuncs.pfnGetPlayerAuthId(pEdict);

		// If is not null
		if (AuthId)
		{
			// If is not HLTV
			if (!((pEdict->v.flags & FL_PROXY) == FL_PROXY))
			{
				// Status
				this->m_Player[AuthId].Status = 0;
			}
		}
	}
}

// Player Menu
void CMatchPlayer::PlayerMenu(CBasePlayer* Player)
{
	auto EntityIndex = Player->entindex();

	if (EntityIndex)
	{
		auto Flags = gMatchAdmin.GetFlags(EntityIndex);

		if (!(Flags & ADMIN_MENU))
		{
			gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
			return;
		}

		if (!this->m_Player.empty())
		{
			gMatchMenu[EntityIndex].Create(_T("Player List"), true, (void*)this->PlayerMenuHandle);

			for (auto const& Target : this->m_Player)
			{
				if (!gMatchAdmin.Access(Target.second.Auth, ADMIN_IMMUNITY))
				{
					if (Target.second.Status == 2)
					{
						gMatchMenu[EntityIndex].AddItem(Target.second.UserId, gMatchUtil.FormatString("%s \\R\\y%s", Target.second.Name.c_str(), gMatchBot.GetTeam(Target.second.LastTeam, true)), false, Target.second.UserId);
					}
					else
					{
						gMatchMenu[EntityIndex].AddItem(Target.second.UserId, gMatchUtil.FormatString("%s \\R\\y%s", Target.second.Name.c_str(), (Target.second.Status == 1) ? _T("Online") : _T("Offline")), false, Target.second.UserId);
					}
				}
			}

			gMatchMenu[EntityIndex].Show(EntityIndex);
		}
		else
		{
			gMatchUtil.SayText(INDEXENT(EntityIndex), PRINT_TEAM_DEFAULT, _T("Player List is empty..."));
		}
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
void CMatchPlayer::ShowInfo(CBasePlayer* Player, int UserIndex)
{
	// Get Info
	auto PlayerInfo = this->GetInfo(UserIndex);

	// If is not null
	if (PlayerInfo)
	{
		// Get entity index
		auto EntityIndex = Player->entindex();

		// Console Print
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s", std::string(32, '=').c_str());
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Name"), PlayerInfo->Name.c_str());
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Steam"), PlayerInfo->Auth.c_str());
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Address"), PlayerInfo->Address.c_str());
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Flags"), PlayerInfo->Flags.c_str());
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Team"), gMatchBot.GetTeam(PlayerInfo->LastTeam, false));
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: #%d", _T("User Index"), PlayerInfo->UserId);
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Status"), (PlayerInfo->Status > 0) ? (PlayerInfo->Status == 1 ? _T("Online") : _T("In Game")) : _T("Offline"));
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s", std::string(32, '=').c_str());

		// Create Menu
		gMatchMenu[EntityIndex].Create
		(
			gMatchUtil.FormatString
			(
				"%s:\\w\n\n%s: \\y%s\\w\n%s: \\y%s\\w\n%s: \\y%s\\w\n%s: \\y%s\\w\n%s: \\y#%d\\w\n%s: \\y%s",
				PlayerInfo->Name.c_str(),
				_T("Steam"),
				PlayerInfo->Auth.c_str(),
				_T("Address"),
				PlayerInfo->Address.c_str(),
				_T("Flags"),
				PlayerInfo->Flags.c_str(),
				_T("Team"),
				gMatchBot.GetTeam(PlayerInfo->LastTeam, false),
				_T("User Index"),
				PlayerInfo->UserId,
				_T("Status"),
				(PlayerInfo->Status > 0) ? (PlayerInfo->Status == 1 ? _T("Online") : _T("In Game")) : _T("Offline")
			),
			true,
			(void*)this->PlayerMenuActionHandle
		);

		// If is an admin with immunity flag
		auto IsAdmin = static_cast<bool>(PlayerInfo->AdminFlags & ADMIN_IMMUNITY);

		// If is online
		auto IsConnected = (PlayerInfo->Status > 0);

		// Ban
		gMatchMenu[EntityIndex].AddItem(0, _T("Ban Player"), IsAdmin, PlayerInfo->UserId);

		// Kick
		gMatchMenu[EntityIndex].AddItem(1, _T("Kick Player"), (IsAdmin || !IsConnected), PlayerInfo->UserId);

		// Kill
		gMatchMenu[EntityIndex].AddItem(2, _T("Slay Player"), (IsAdmin || !IsConnected), PlayerInfo->UserId);

		// Change Team
		gMatchMenu[EntityIndex].AddItem(3, _T("Team Player"), (IsAdmin || !IsConnected), PlayerInfo->UserId);

		// Show Menu
		gMatchMenu[EntityIndex].Show(EntityIndex);
	}
}

// Player Menu Action Handler
void CMatchPlayer::PlayerMenuActionHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		if (Item.Extra)
		{
			auto PlayerInfo = gMatchPlayer.GetInfo(Item.Extra);

			if (PlayerInfo)
			{
				switch (Item.Info)
				{
					case 0: // Ban
					{
						gMatchMenu[EntityIndex].Create(gMatchUtil.FormatString(_T("Choose a time to ban: ^w%s^y"), PlayerInfo->Name.c_str()), true, (void*)gMatchPlayer.PlayerBanMenuActionHandle);

						std::vector<time_t> BanTimes = { 0, 5, 10, 15, 30, 60, 120, 240, 480, 960, 1440, 10080, 43200 };

						for (auto const& Time : BanTimes)
						{
							gMatchMenu[EntityIndex].AddItem(Time, CTimeFormat::GetTimeLength(Time, CTimeFormat::TIMEUNIT_MINUTES), false, Item.Extra);
						}

						gMatchMenu[EntityIndex].Show(EntityIndex);

						break;
					}
					case 1: // Kick
					{
						auto Target = gMatchUtil.GetPlayerByUserId(Item.Extra);

						if (Target)
						{
							gMatchUtil.ServerCommand("kick #%d", Item.Extra);

							gpMetaUtilFuncs->pfnLogMessage
							(
								&Plugin_info,
								"\"%s<%i><%s><%s>\" kick \"%s<%i><%s><%s>\"",
								STRING(Player->edict()->v.netname),
								g_engfuncs.pfnGetPlayerUserId(Player->edict()),
								g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
								gMatchBot.GetTeam(Player->m_iTeam, true),
								STRING(Target->edict()->v.netname),
								g_engfuncs.pfnGetPlayerUserId(Target->edict()),
								g_engfuncs.pfnGetPlayerAuthId(Target->edict()),
								gMatchBot.GetTeam(Target->m_iTeam, true)
							);
						}
						else
						{
							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("^3%s^1 is not in connected."), PlayerInfo->Name.c_str());
						}
						
						break;
					}
					case 2: // Kill
					{
						auto Target = gMatchUtil.GetPlayerByUserId(Item.Extra);

						if (Target)
						{
							MDLL_ClientKill(Target->edict());

							gpMetaUtilFuncs->pfnLogMessage
							(
								&Plugin_info,
								"\"%s<%i><%s><%s>\" slays \"%s<%i><%s><%s>\"",
								STRING(Player->edict()->v.netname),
								g_engfuncs.pfnGetPlayerUserId(Player->edict()),
								g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
								gMatchBot.GetTeam(Player->m_iTeam, true),
								STRING(Target->edict()->v.netname),
								g_engfuncs.pfnGetPlayerUserId(Target->edict()),
								g_engfuncs.pfnGetPlayerAuthId(Target->edict()),
								gMatchBot.GetTeam(Target->m_iTeam, true)
							);
						}
						else
						{
							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("^3%s^1 is not in connected."), PlayerInfo->Name.c_str());
						}

						break;
					}
					case 3: // Change Team
					{
						auto Target = gMatchUtil.GetPlayerByUserId(Item.Extra);

						if (Target)
						{
							gMatchAdminMenu.TeamMenuHandle(Player->entindex(), { Target->entindex(), STRING(Target->edict()->v.netname), false, 0 });
						}
						else
						{
							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("^3%s^1 is not in connected."), PlayerInfo->Name.c_str());
						}

						break;
					}
				}
			}
		}
	}
}

// Player Ban Menu Action Handler
void CMatchPlayer::PlayerBanMenuActionHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		auto Info = gMatchPlayer.GetInfo(Item.Extra);

		if (Info)
		{
			gMatchUtil.ServerCommand(_T("banid %d %s kick"), Item.Info, Info->Auth.c_str());

			gMatchUtil.ServerCommand("wait;wait;writeid;writeip");

			gpMetaUtilFuncs->pfnLogMessage
			(
				&Plugin_info,
				"\"%s<%i><%s><%s>\" banned \"%s<%i><%s><%s>\" for %d min(s)",
				STRING(Player->edict()->v.netname),
				g_engfuncs.pfnGetPlayerUserId(Player->edict()),
				g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
				gMatchBot.GetTeam(Player->m_iTeam, true),
				Info->Name.c_str(),
				"",
				Info->Auth.c_str(),
				gMatchBot.GetTeam(Info->LastTeam, true),
				Item.Info
			);
		}
	}
}