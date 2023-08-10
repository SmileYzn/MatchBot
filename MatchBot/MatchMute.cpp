#include "precompiled.h"

CMatchMute gMatchMute;

void CMatchMute::Menu(CBasePlayer* Player)
{
	if (Player)
	{
		auto EntityIndex = Player->entindex();

		if (EntityIndex)
		{
			gMatchMenu[EntityIndex].Create(_T("Mute Menu:"), true, (void*)this->MenuHandle);

			auto Players = gMatchUtil.GetPlayers(false, true);

			for (auto const& Target : Players)
			{
				if (Target->entindex() != EntityIndex)
				{
					if (!gMatchAdmin.Access(EntityIndex, ADMIN_IMMUNITY))
					{
						if (this->GetMute(Player, Target))
						{
							gMatchMenu[EntityIndex].AddItem(Target->entindex(), gMatchUtil.FormatString(_T("%s ^r^RMuted"), STRING(Target->edict()->v.netname)), false, 0);
						}
						else
						{
							gMatchMenu[EntityIndex].AddItem(Target->entindex(), gMatchUtil.FormatString(_T("%s ^y^RListening"), STRING(Target->edict()->v.netname)), false, 1);
						}
					}
				}
			}

			gMatchMenu[EntityIndex].Show(EntityIndex);
		}
	}
}

void CMatchMute::MenuHandle(int EntityIndex, P_MENU_ITEM Item)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		auto Target = UTIL_PlayerByIndexSafe(Item.Info);

		if (Target)
		{
			gMatchMute.SetMute(Player, Target, Item.Extra ? true : false);

			gMatchMute.Menu(Player);
		}
	}
}

bool CMatchMute::GetMute(CBasePlayer* Player, CBasePlayer* Target)
{
	if (Player)
	{
		if (Target)
		{
			if (!Player->IsDormant() && !Target->IsDormant())
			{
				if (!Player->IsBot() && !Target->IsBot())
				{
					auto PlayerAuth = g_engfuncs.pfnGetPlayerAuthId(Player->edict());

					if (PlayerAuth)
					{
						auto TargetAuth = Target->IsBot() ? STRING(Target->edict()->v.netname) : g_engfuncs.pfnGetPlayerAuthId(Target->edict());

						if (TargetAuth)
						{
							if (this->m_Player.find(PlayerAuth) != this->m_Player.end())
							{
								if (this->m_Player[PlayerAuth].find(TargetAuth) != this->m_Player[PlayerAuth].end())
								{
									return this->m_Player[PlayerAuth][TargetAuth];
								}
							}
						}
					}
				}
			}
		}
	}

	return false;
}

void CMatchMute::SetMute(CBasePlayer* Player, CBasePlayer* Target, bool Mute)
{
	if (Player)
	{
		if (Target)
		{
			auto PlayerAuth = g_engfuncs.pfnGetPlayerAuthId(Player->edict());

			if (PlayerAuth)
			{
				auto TargetAuth = Target->IsBot() ? STRING(Target->edict()->v.netname) : g_engfuncs.pfnGetPlayerAuthId(Target->edict());

				if (TargetAuth)
				{
					this->m_Player[PlayerAuth][TargetAuth] = Mute;

					if (Mute)
					{
						gMatchUtil.SayText(Player->edict(), Player->entindex(), _T("You have muted ^3%s..."), STRING(Target->edict()->v.netname));
					}
					else
					{
						gMatchUtil.SayText(Player->edict(), Player->entindex(), _T("You have unmuted ^3%s..."), STRING(Target->edict()->v.netname));
					}
				}
			}
		}
	}
}