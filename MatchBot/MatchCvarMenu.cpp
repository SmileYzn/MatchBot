#include "precompiled.h"

CMatchCvarMenu gMatchCvarMenu;

// ON Server Activate
void CMatchCvarMenu::ServerActivate()
{
	// Clear data
	this->m_Data.clear();

	// Clear Count
	this->m_CvarCount = 0;

	// Memory Script instance
	CMemScript* lpMemScript = new CMemScript;

	// If is not null
	if (lpMemScript)
	{
		// Try to load file
		if (lpMemScript->SetBuffer(MB_CVAR_MENU_FILE))
		{
			try
			{
				// Loop lines
				while (true)
				{
					// If file content ended
					if (lpMemScript->GetToken() == eTokenResult::TOKEN_END)
					{
						// Break loop
						break;
					}

					// Cvar
					auto Cvar = lpMemScript->GetString();

					// If is not empty
					if (!Cvar.empty())
					{
						// Get variable pointer
						auto Pointer = g_engfuncs.pfnCVarGetPointer(Cvar.c_str());

						// If is not null
						if (Pointer)
						{
							// Set pointer
							this->m_Data[this->m_CvarCount].Pointer = Pointer;

							// Loop lines
							while (true)
							{
								// If is end of section
								if (lpMemScript->GetAsString().compare("end") == 0)
								{
									// Break loop
									break;
								}

								// Get Value
								this->m_Data[this->m_CvarCount].Values.push_back(lpMemScript->GetString());
							}

							// Increment cvar counter
							this->m_CvarCount++;
						}
					}
				}
			}
			catch (...)
			{
				// Catch for erros
				LOG_CONSOLE(PLID, "[%s] %s", __func__, lpMemScript->GetError().c_str());
			}
		}

		// Delete Memory Script instance
		delete lpMemScript;
	}
}

// Cvar Menu
void CMatchCvarMenu::Menu(CBasePlayer* Player)
{
	// Get entity index
	auto EntityIndex = Player->entindex();

	// If did not have cvar flag
	if (!gMatchAdmin.Access(EntityIndex, ADMIN_CVAR))
	{
		gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	// If cvar list is not empty
	if (!this->m_Data.empty())
	{
		// Create menu
		gMatchMenu[EntityIndex].Create(_T("Cvars Menu"), true, (void*)this->MenuHandle);

		// Loop variable items
		for (auto const& row : this->m_Data)
		{
			// Add item on menu
			gMatchMenu[EntityIndex].AddItem(row.first, gMatchUtil.FormatString("%s \\R%s", row.second.Pointer->name, row.second.Pointer->string), false, row.first);
		}

		// Show menu to player
		gMatchMenu[EntityIndex].Show(EntityIndex);
	}
}

// Cvar Menu Handler
void CMatchCvarMenu::MenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		gMatchCvarMenu.UpdateValue(Player, Item.Info);

		gMatchCvarMenu.Menu(Player);
	}
}

void CMatchCvarMenu::UpdateValue(CBasePlayer* Player, int ItemIndex)
{
	if (this->m_Data.find(ItemIndex) != this->m_Data.end())
	{
		auto end = this->m_Data[ItemIndex].Values.end();

		auto begin = this->m_Data[ItemIndex].Values.begin();

		for (auto it = begin; ; ++it)
		{
			if (it < end)
			{
				if (!it->compare(this->m_Data[ItemIndex].Pointer->string))
				{
					if (++it >= end)
					{
						it = begin;
					}

					g_engfuncs.pfnCVarSetString(this->m_Data[ItemIndex].Pointer->name, it->c_str());

					LOG_MESSAGE
					(
						PLID,
						"\"%s<%i><%s><%s>\" changed cvar %s to %s",
						STRING(Player->edict()->v.netname),
						g_engfuncs.pfnGetPlayerUserId(Player->edict()),
						g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
						gMatchBot.GetTeam(Player->m_iTeam, true),
						this->m_Data[ItemIndex].Pointer->name,
						it->c_str()
					);
					break;
				}
			}
			else
			{
				g_engfuncs.pfnCVarSetString(this->m_Data[ItemIndex].Pointer->name, begin->c_str());

				LOG_MESSAGE
				(
					PLID,
					"\"%s<%i><%s><%s>\" changed cvar %s to %s",
					STRING(Player->edict()->v.netname),
					g_engfuncs.pfnGetPlayerUserId(Player->edict()),
					g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
					gMatchBot.GetTeam(Player->m_iTeam, true),
					this->m_Data[ItemIndex].Pointer->name,
					it->c_str()
				);
				break;;
			}
		}
	}
}