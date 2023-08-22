#include "precompiled.h"

CMatchCvarMenu gMatchCvarMenu;

// ON Server Activate
void CMatchCvarMenu::ServerActivate()
{
	// Clear data
	this->m_Data.clear();

	try
	{
		// File stream
		std::ifstream fp(MB_CVAR_MENU_FILE, std::ios::in);

		// If file is open
		if (fp)
		{
			// Reset pointer
			fp.clear();

			// Go to begin of file
			fp.seekg(0, std::ios::beg);

			// Read data from json file
			auto json = nlohmann::json::parse(fp, nullptr, true, true);

			// If is not empty
			if (!json.empty())
			{
				//
				this->m_CvarCount = 0;

				// Loop each item of array
				for (auto const& row : json.items())
				{
					// Get admin data as map string
					auto Name = row.key().c_str();

					// If name is not empty
					if (Name)
					{
						// Set variable
						this->m_Data[this->m_CvarCount].Variable = g_engfuncs.pfnCVarGetPointer(Name);

						// If is a valid cvar
						if (this->m_Data[this->m_CvarCount].Variable)
						{
							// Get cvar object value
							auto cvar = row.value();

							// If is not empty
							if (!cvar.empty())
							{
								// If has array of values and access
								if (cvar.contains("values") && cvar.contains("access"))
								{
									// If values is array and access is string
									if (cvar["values"].is_array() && cvar["access"].is_string())
									{
										// If is not empty
										if (!cvar["values"].empty() && !cvar["access"].empty())
										{
											// Access
											this->m_Data[this->m_CvarCount].Access = gMatchAdmin.ReadFlags(std::string(cvar["access"]).c_str());

											// Values
											this->m_Data[this->m_CvarCount].Values = cvar["values"].get<std::vector<std::string>>();

											// Increment variable total count
											this->m_CvarCount++;
										}
									}
								}
							}
						}
					}
				}
			}

			// Close file
			fp.close();
		}
		else
		{
			// Log fail
			LOG_CONSOLE(PLID, "[%s] Failed to open file: %s", __func__, MB_CVAR_MENU_FILE);
		}
	}
	catch (nlohmann::json::parse_error& e)
	{
		// Get exception
		LOG_CONSOLE(PLID, "[%s] %s", __func__, e.what());
	}
}

// Cvar Menu
void CMatchCvarMenu::Menu(CBasePlayer* Player)
{
	// If did not have cvar flag
	if (!(gMatchAdmin.GetFlags(Player->entindex()) & ADMIN_CVAR))
	{
		gMatchUtil.SayText(Player->edict(), PRINT_TEAM_DEFAULT, _T("You do not have access to that command."));
		return;
	}

	// If cvar list is not empty
	if (!this->m_Data.empty())
	{
		// Get entity index
		auto EntityIndex = Player->entindex();

		// Create menu
		gMatchMenu[EntityIndex].Create(_T("Cvars Menu"), true, (void*)this->MenuHandle);

		// Loop variable items
		for (auto const& row : this->m_Data)
		{
			// Check if user has access to that variable
			if (gMatchAdmin.Access(EntityIndex, row.second.Access))
			{
				// Add item on menu
				gMatchMenu[EntityIndex].AddItem(row.first, gMatchUtil.FormatString("%s \\R%s", row.second.Variable->name, row.second.Variable->string), false, row.first);
			}
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
		gMatchCvarMenu.UpdateValue(Item.Info);

		gMatchCvarMenu.Menu(Player);
	}
}

void CMatchCvarMenu::UpdateValue(int ItemIndex)
{
	if (this->m_Data.find(ItemIndex) != this->m_Data.end())
	{
		auto end = this->m_Data[ItemIndex].Values.end();

		auto begin = this->m_Data[ItemIndex].Values.begin();

		for (auto it = begin; ; ++it)
		{
			if (it < end)
			{
				if (!it->compare(this->m_Data[ItemIndex].Variable->string))
				{
					if (++it >= end)
					{
						it = begin;
					}

					g_engfuncs.pfnCVarSetString(this->m_Data[ItemIndex].Variable->name, it->c_str());

					break;
				}
			}
			else
			{
				g_engfuncs.pfnCVarSetString(this->m_Data[ItemIndex].Variable->name, begin->c_str());
				break;;
			}
		}
	}
}