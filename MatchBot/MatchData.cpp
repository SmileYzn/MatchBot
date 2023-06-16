#include "precompiled.h"

CMatchData gMatchData;

void CMatchData::ServerActivate()
{
	g_engfuncs.pfnAddServerCommand("mb_get_status", this->GetStatus);
	g_engfuncs.pfnAddServerCommand("mb_get_players", this->GetPlayers);
}

void CMatchData::GetStatus()
{
	// Data
	nlohmann::ordered_json json;

	// Server Data
	json =
	{
		g_engfuncs.pfnCVarGetString("hostname"),
		g_engfuncs.pfnCVarGetString("net_address"),
		STRING(gpGlobals->mapname),
		gMatchBot.GetState(gMatchBot.GetState()),
		gMatchBot.GetKnifeRound(),
		gMatchBot.GetScore(TERRORIST),
		gMatchBot.GetScore(CT)
	};
	//
	LOG_CONSOLE(PLID, "%s", json.dump().c_str());
}

void CMatchData::GetPlayers()
{
	nlohmann::ordered_json json;

	auto Players = gMatchUtil.GetPlayers(true, true);

	if (Players.size())
	{
		for (auto const& Player : Players)
		{
			json =
			{
				STRING(Player->edict()->v.netname),
				g_engfuncs.pfnGetPlayerAuthId(Player->edict()),
				g_engfuncs.pfnGetPlayerUserId(Player->edict()),
				Player->edict()->v.frags,
				Player->m_iDeaths
			};
			//
			LOG_CONSOLE(PLID, "%s", json.dump().c_str());
		}
	}
}