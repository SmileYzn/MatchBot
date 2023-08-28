#include "precompiled.h"

CMatchPlayer gMatchPlayer;

// On Server Activate
void CMatchPlayer::ServerActivate()
{
	// Clear Player Data
	this->m_Player.clear();
}

// Get Player Info
LP_PLAYER_INFO CMatchPlayer::GetInfo(int UserIndex)
{
	// Get Player Info
	if (this->m_Player.find(UserIndex) != this->m_Player.end())
	{
		return &this->m_Player[UserIndex];
	}

	return nullptr;
}

// On Player Connect
bool CMatchPlayer::PlayerConnect(edict_t* pEdict, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	// If is not null entity
	if (!FNullEnt(pEdict))
	{
		// Get Auth Index
		auto UserIndex = g_engfuncs.pfnGetPlayerUserId(pEdict);

		// If is not null
		if (UserIndex)
		{
			// If is not HLTV
			if (!((pEdict->v.flags & FL_PROXY) == FL_PROXY))
			{
				// Auth
				this->m_Player[UserIndex].Auth = g_engfuncs.pfnGetPlayerAuthId(pEdict);

				// Name
				this->m_Player[UserIndex].Name = (pszName != nullptr) ? pszName : "";

				// Address
				this->m_Player[UserIndex].Address = (pszAddress != nullptr) ? pszAddress : "";

				// Admin Flags
				this->m_Player[UserIndex].Flags = gMatchAdmin.GetFlags(pEdict);

				// Admin Flags
				this->m_Player[UserIndex].AdminFlags = gMatchAdmin.GetFlags(ENTINDEX(pEdict));

				// Status
				this->m_Player[UserIndex].Status = 1;

				// Last Team
				this->m_Player[UserIndex].LastTeam = TeamName::UNASSIGNED;
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
	auto UserIndex = g_engfuncs.pfnGetPlayerUserId(Player->edict());

	// If is not null
	if (UserIndex)
	{
		// Auth
		this->m_Player[UserIndex].Auth = g_engfuncs.pfnGetPlayerAuthId(Player->edict());

		// Name
		this->m_Player[UserIndex].Name = STRING(Player->edict()->v.netname);

		// Address
		this->m_Player[UserIndex].Address = Player->IsBot() ? "loopback" : this->m_Player[UserIndex].Address;

		// Admin Flags
		this->m_Player[UserIndex].Flags = gMatchAdmin.GetFlags(Player->edict());

		// Admin Flags
		this->m_Player[UserIndex].AdminFlags = gMatchAdmin.GetFlags(Player->entindex());

		// Status
		this->m_Player[UserIndex].Status = 2;

		// Last Team
		this->m_Player[UserIndex].LastTeam = Player->m_iTeam;
	}
}

// On Player Change Team
void CMatchPlayer::PlayerSwitchTeam(CBasePlayer* Player)
{
	// Get Auth Index
	auto UserIndex = g_engfuncs.pfnGetPlayerUserId(Player->edict());

	// If is not null
	if (UserIndex)
	{
		// Auth
		this->m_Player[UserIndex].Auth = g_engfuncs.pfnGetPlayerAuthId(Player->edict());

		// Name
		this->m_Player[UserIndex].Name = STRING(Player->edict()->v.netname);

		// Address
		this->m_Player[UserIndex].Address = Player->IsBot() ? "loopback" : this->m_Player[UserIndex].Address;

		// Admin Flags
		this->m_Player[UserIndex].Flags = gMatchAdmin.GetFlags(Player->edict());

		// Admin Flags
		this->m_Player[UserIndex].AdminFlags = gMatchAdmin.GetFlags(Player->entindex());

		// Status
		this->m_Player[UserIndex].Status = 2;

		// Last Team
		this->m_Player[UserIndex].LastTeam = Player->m_iTeam;
	}
}

// On Player Disconnect
void CMatchPlayer::PlayerDisconnect(edict_t* pEdict)
{
	// If is not null entity
	if (!FNullEnt(pEdict))
	{
		// Get Auth Index
		auto UserIndex = g_engfuncs.pfnGetPlayerUserId(pEdict);

		// If is not null
		if (UserIndex)
		{
			// If is not HLTV
			if (!((pEdict->v.flags & FL_PROXY) == FL_PROXY))
			{
				// Status
				this->m_Player[UserIndex].Status = 0;
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
						gMatchMenu[EntityIndex].AddItem(Target.first, gMatchUtil.FormatString("%s \\R\\y%s", Target.second.Name.c_str(), gMatchBot.GetTeam(Target.second.LastTeam, true)));
					}
					else
					{
						gMatchMenu[EntityIndex].AddItem(Target.first, gMatchUtil.FormatString("%s \\R\\y%s", Target.second.Name.c_str(), (Target.second.Status == 1) ? _T("Online") : _T("Offline")));
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
void CMatchPlayer::ShowInfo(CBasePlayer* Player,int UserId)
{
	// Find Player
	if (this->m_Player.find(UserId) != this->m_Player.end())
	{
		// Get entity index
		auto EntityIndex = Player->entindex();

		// Console Print
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s", std::string(32, '=').c_str());
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Name"), this->m_Player[UserId].Name.c_str());
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Steam"), this->m_Player[UserId].Auth.c_str());
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Address"), this->m_Player[UserId].Address.c_str());
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Flags"), this->m_Player[UserId].Flags.c_str());
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Team"), gMatchBot.GetTeam(this->m_Player[UserId].LastTeam, false));
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: #%d", _T("User Index"), UserId);
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s: %s", _T("Status"), (this->m_Player[UserId].Status > 0) ? (this->m_Player[UserId].Status == 1 ? _T("Online") : _T("In Game")) : _T("Offline"));
		gMatchUtil.ClientPrint(Player->edict(), PRINT_CONSOLE, "%s", std::string(32, '=').c_str());

		// Create Menu
		gMatchMenu[EntityIndex].Create
		(
			gMatchUtil.FormatString
			(
				"%s:\\w\n\n%s: \\y%s\\w\n%s: \\y%s\\w\n%s: \\y%s\\w\n%s: \\y%s\\w\n%s: \\y#%d\\w\n%s: \\y%s",
				this->m_Player[UserId].Name.c_str(),
				_T("Steam"),
				this->m_Player[UserId].Auth.c_str(),
				_T("Address"),
				this->m_Player[UserId].Address.c_str(),
				_T("Flags"),
				this->m_Player[UserId].Flags.c_str(),
				_T("Team"),
				gMatchBot.GetTeam(this->m_Player[UserId].LastTeam, false),
				_T("User Index"),
				UserId,
				_T("Status"),
				(this->m_Player[UserId].Status > 0) ? (this->m_Player[UserId].Status == 1 ? _T("Online") : _T("In Game")) : _T("Offline")
			),
			true,
			(void*)this->PlayerMenuActionHandle
		);

		// If is an admin with immunity flag
		auto IsAdmin = (this->m_Player[UserId].AdminFlags & ADMIN_IMMUNITY);

		// If is online
		auto IsConnected = (this->m_Player[UserId].Status > 0);

		// Ban
		gMatchMenu[EntityIndex].AddItem(0, _T("Ban Player"), IsAdmin, UserId);

		// Kick
		gMatchMenu[EntityIndex].AddItem(1, _T("Kick Player"), (IsAdmin || !IsConnected), UserId);

		// Kill
		gMatchMenu[EntityIndex].AddItem(2, _T("Slay Player"), (IsAdmin || !IsConnected), UserId);

		// Change Team
		gMatchMenu[EntityIndex].AddItem(3, _T("Team Player"), (IsAdmin || !IsConnected), UserId);

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
			auto Info = gMatchPlayer.GetInfo(Item.Extra);

			if (Info)
			{
				switch (Item.Info)
				{
					case 0: // Ban
					{
						gMatchMenu[EntityIndex].Create(gMatchUtil.FormatString(_T("Choose a time to ban: ^w%s^y"), Info->Name.c_str()), true, (void*)gMatchPlayer.PlayerBanMenuActionHandle);

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

							LOG_MESSAGE
							(
								PLID,
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
							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("^3%s^1 is not in connected."), Info->Name.c_str());
						}
						
						break;
					}
					case 2: // Kill
					{
						auto Target = gMatchUtil.GetPlayerByUserId(Item.Extra);

						if (Target)
						{
							MDLL_ClientKill(Target->edict());

							LOG_MESSAGE
							(
								PLID,
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
							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("^3%s^1 is not in connected."), Info->Name.c_str());
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
							gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("^3%s^1 is not in connected."), Info->Name.c_str());
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

			LOG_MESSAGE
			(
				PLID,
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