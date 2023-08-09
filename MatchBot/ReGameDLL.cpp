#include "precompiled.h"

IReGameApi			*g_ReGameApi;
const ReGameFuncs_t	*g_ReGameFuncs;
IReGameHookchains	*g_ReGameHookchains;
CGameRules			*g_pGameRules;

bool ReGameDLL_Init()
{
	const char *szGameDLLModule = GET_GAME_INFO(PLID, GINFO_DLL_FULLPATH);

	if (!szGameDLLModule)
	{
		LOG_CONSOLE(PLID, "[%s] Failed to get GameDLL module", Plugin_info.logtag);
		return false;
	}

	CSysModule *gameModule = Sys_LoadModule(szGameDLLModule);

	if (!gameModule)
	{
		LOG_CONSOLE(PLID, "[%s] Failed to locate GameDLL module", Plugin_info.logtag);
		return false;
	}

	CreateInterfaceFn ifaceFactory = Sys_GetFactory(gameModule);

	if (!ifaceFactory)
	{
		LOG_CONSOLE(PLID, "[%s] Failed to locate interface factory in GameDLL module", Plugin_info.logtag);
		return false;
	}

	int retCode = 0;

	g_ReGameApi = (IReGameApi *)ifaceFactory(VRE_GAMEDLL_API_VERSION, &retCode);

	if (!g_ReGameApi)
	{
		LOG_CONSOLE(PLID, "[%s] Failed to locate retrieve rehlds api interface from GameDLL module, return code is %d", Plugin_info.logtag, retCode);
		return false;
	}

	int majorVersion = g_ReGameApi->GetMajorVersion();
	int minorVersion = g_ReGameApi->GetMinorVersion();

	if (majorVersion != REGAMEDLL_API_VERSION_MAJOR)
	{
		LOG_CONSOLE(PLID, "[%s] ReGameDLL API major version mismatch; expected %d, real %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MAJOR, majorVersion);

		if (majorVersion < REGAMEDLL_API_VERSION_MAJOR)
		{
			LOG_CONSOLE(PLID, "[%s] Please update the ReGameDLL up to a major version API >= %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MAJOR);
		}
		else if (majorVersion > REGAMEDLL_API_VERSION_MAJOR)
		{
			LOG_CONSOLE(PLID, "[%s] Please update the %s up to a major version API >= %d", Plugin_info.logtag, Plugin_info.logtag, majorVersion);
		}

		return false;
	}

	if (minorVersion < REGAMEDLL_API_VERSION_MINOR)
	{
		LOG_CONSOLE(PLID, "[%s] ReGameDLL API minor version mismatch; expected at least %d, real %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MINOR, minorVersion);
		LOG_CONSOLE(PLID, "[%s] Please update the ReGameDLL up to a minor version API >= %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MINOR);
		return false;
	}

	g_ReGameFuncs = g_ReGameApi->GetFuncs();

	g_ReGameHookchains = g_ReGameApi->GetHookchains();

	if (!g_ReGameApi->BGetICSEntity(CSENTITY_API_INTERFACE_VERSION))
	{
		LOG_CONSOLE(PLID, "[%s] Interface CCSEntity API version '%s' not found", Plugin_info.logtag, CSENTITY_API_INTERFACE_VERSION);
		return false;
	}

	g_ReGameHookchains->InstallGameRules()->registerHook(ReGameDLL_InstallGameRules);

	g_ReGameHookchains->HandleMenu_ChooseTeam()->registerHook(ReGameDLL_HandleMenu_ChooseTeam);

	g_ReGameHookchains->CBasePlayer_GetIntoGame()->registerHook(ReGameDLL_CBasePlayer_GetIntoGame);

	g_ReGameHookchains->CBasePlayer_SwitchTeam()->registerHook(ReGameDLL_CBasePlayer_SwitchTeam);

	g_ReGameHookchains->CBasePlayer_AddAccount()->registerHook(ReGameDLL_CBasePlayer_AddAccount);

	g_ReGameHookchains->CBasePlayer_HasRestrictItem()->registerHook(ReGameDLL_CBasePlayer_HasRestrictItem);

	g_ReGameHookchains->InternalCommand()->registerHook(ReGameDLL_InternalCommand);

	g_ReGameHookchains->CSGameRules_OnRoundFreezeEnd()->registerHook(ReGameDLL_CSGameRules_OnRoundFreezeEnd);

	g_ReGameHookchains->CSGameRules_RestartRound()->registerHook(ReGameDLL_CSGameRules_RestartRound);

	g_ReGameHookchains->RoundEnd()->registerHook(ReGameDLL_RoundEnd);

	g_ReGameHookchains->CBasePlayer_TakeDamage()->registerHook(ReGameDLL_CBasePlayer_TakeDamage);

	g_ReGameHookchains->CGrenade_ExplodeSmokeGrenade()->registerHook(ReGameDLL_CGrenade_ExplodeSmokeGrenade);

	g_ReGameHookchains->CSGameRules_CanPlayerHearPlayer()->registerHook(ReGameDLL_CSGameRules_CanPlayerHearPlayer);

	return true;
}

bool ReGameDLL_Stop()
{
	g_ReGameHookchains->InstallGameRules()->unregisterHook(ReGameDLL_InstallGameRules);

	g_ReGameHookchains->HandleMenu_ChooseTeam()->unregisterHook(ReGameDLL_HandleMenu_ChooseTeam);

	g_ReGameHookchains->CBasePlayer_GetIntoGame()->unregisterHook(ReGameDLL_CBasePlayer_GetIntoGame);

	g_ReGameHookchains->CBasePlayer_SwitchTeam()->unregisterHook(ReGameDLL_CBasePlayer_SwitchTeam);

	g_ReGameHookchains->CBasePlayer_AddAccount()->unregisterHook(ReGameDLL_CBasePlayer_AddAccount);

	g_ReGameHookchains->CBasePlayer_HasRestrictItem()->unregisterHook(ReGameDLL_CBasePlayer_HasRestrictItem);

	g_ReGameHookchains->InternalCommand()->unregisterHook(ReGameDLL_InternalCommand);

	g_ReGameHookchains->CSGameRules_OnRoundFreezeEnd()->unregisterHook(ReGameDLL_CSGameRules_OnRoundFreezeEnd);

	g_ReGameHookchains->CSGameRules_RestartRound()->unregisterHook(ReGameDLL_CSGameRules_RestartRound);

	g_ReGameHookchains->RoundEnd()->unregisterHook(ReGameDLL_RoundEnd);

	g_ReGameHookchains->CBasePlayer_TakeDamage()->unregisterHook(ReGameDLL_CBasePlayer_TakeDamage);

	g_ReGameHookchains->CGrenade_ExplodeSmokeGrenade()->unregisterHook(ReGameDLL_CGrenade_ExplodeSmokeGrenade);

	g_ReGameHookchains->CSGameRules_CanPlayerHearPlayer()->unregisterHook(ReGameDLL_CSGameRules_CanPlayerHearPlayer);

	return true;
}

CGameRules *ReGameDLL_InstallGameRules(IReGameHook_InstallGameRules* chain)
{
	auto gamerules = chain->callNext();

	if (!g_ReGameApi->BGetIGameRules(GAMERULES_API_INTERFACE_VERSION))
	{
		LOG_CONSOLE(PLID, "[%s] Interface GameRules API version '%s' not found", Plugin_info.logtag, GAMERULES_API_INTERFACE_VERSION);
	}
	else
	{
		g_pGameRules = gamerules;
	}
	
	return gamerules;
}

BOOL ReGameDLL_HandleMenu_ChooseTeam(IReGameHook_HandleMenu_ChooseTeam* chain, CBasePlayer* Player, int Slot)
{
	if (gMatchBot.PlayerJoinTeam(Player, Slot))
	{
		Slot = 0;
	}

	return chain->callNext(Player, Slot);
}

bool ReGameDLL_CBasePlayer_GetIntoGame(IReGameHook_CBasePlayer_GetIntoGame* chain, CBasePlayer* Player)
{
	bool ret = chain->callNext(Player);

	gMatchBot.PlayerGetIntoGame(Player);

	gMatchReady.PlayerGetIntoGame(Player);

	gMatchPlayer.PlayerGetIntoGame(Player);

	return ret;
}

void ReGameDLL_CBasePlayer_SwitchTeam(IReGameHook_CBasePlayer_SwitchTeam* chain, CBasePlayer* Player)
{
	chain->callNext(Player);

	gMatchPlayer.PlayerSwitchTeam(Player);
}

void ReGameDLL_CBasePlayer_AddAccount(IReGameHook_CBasePlayer_AddAccount* chain, CBasePlayer* pthis, int amount, RewardType type, bool bTrackChange)
{
	if (gMatchWarmup.PlayerAddAccount(pthis, amount, type, bTrackChange))
	{
		amount = 0;
	}

	chain->callNext(pthis, amount, type, bTrackChange);
}

bool ReGameDLL_CBasePlayer_HasRestrictItem(IReGameHook_CBasePlayer_HasRestrictItem* chain, CBasePlayer* pthis, ItemID item, ItemRestType type)
{
	auto ret = chain->callNext(pthis, item, type);

	if (gMatchRestrictItem.PlayerHasRestrictItem(pthis, item, type))
	{
		ret = true;
	}

	return ret;
}

void ReGameDLL_InternalCommand(IReGameHook_InternalCommand* chain, edict_t* pEntity, const char* pcmd, const char* parg1)
{
	auto Player = UTIL_PlayerByIndexSafe(ENTINDEX(pEntity));

	if (Player)
	{
		if (gMatchCommand.ClientCommand(Player, pcmd, parg1))
		{
			return;
		}
	}

	chain->callNext(pEntity, pcmd, parg1);
}

void ReGameDLL_CSGameRules_OnRoundFreezeEnd(IReGameHook_CSGameRules_OnRoundFreezeEnd* chain)
{
	chain->callNext();

	gMatchBot.RoundStart();

	gMatchStats.RoundStart();
}

void ReGameDLL_CSGameRules_RestartRound(IReGameHook_CSGameRules_RestartRound* chain)
{
	gMatchBot.RoundRestart(true);

	chain->callNext();

	gMatchBot.RoundRestart(false);

	gMatchLO3.RoundRestart();

	gMatchPause.RoundRestart();
}

bool ReGameDLL_RoundEnd(IReGameHook_RoundEnd* chain, int winStatus, ScenarioEventEndRound event, float tmDelay)
{
	gMatchStats.RoundEnd(winStatus, event, tmDelay);

	auto ret = chain->callNext(winStatus, event, tmDelay);

	gMatchBot.RoundEnd(winStatus, event, tmDelay);

	return ret;
}

int ReGameDLL_CBasePlayer_TakeDamage(IReGameHook_CBasePlayer_TakeDamage* chain, CBasePlayer* pThis, entvars_t* pevInflictor, entvars_t* pevAttacker, float& flDamage, int bitsDamageType)
{
	int ret = chain->callNext(pThis, pevInflictor, pevAttacker, flDamage, bitsDamageType);

	gMatchStats.PlayerDamage(pThis, pevInflictor, pevAttacker, flDamage, bitsDamageType);

	return ret;
}

void ReGameDLL_CGrenade_ExplodeSmokeGrenade(IReGameHook_CGrenade_ExplodeSmokeGrenade* chain, CGrenade* pthis)
{
	chain->callNext(pthis);

	gMatchBugFix.ExplodeSmokeGrenade(pthis);
}

bool ReGameDLL_CSGameRules_CanPlayerHearPlayer(IReGameHook_CSGameRules_CanPlayerHearPlayer* chain, CBasePlayer* pListener, CBasePlayer* pSender)
{
	auto ret = chain->callNext(pListener, pSender);

	if (gMatchMute.GetMute(pListener, pSender))
	{
		return false;
	}

	return ret;
}