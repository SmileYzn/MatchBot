#include "precompiled.h"

IReGameApi *g_ReGameApi;
const ReGameFuncs_t *g_ReGameFuncs;
IReGameHookchains *g_ReGameHookchains;
CGameRules *g_pGameRules;

bool ReGameDLL_Init()
{
	const char *szGameDLLModule = GET_GAME_INFO(&Plugin_info, GINFO_DLL_FULLPATH);

	if (!szGameDLLModule)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s] Failed to get GameDLL module", Plugin_info.logtag);
		return false;
	}

	CSysModule *gameModule = Sys_LoadModule(szGameDLLModule);

	if (!gameModule)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s] Failed to locate GameDLL module", Plugin_info.logtag);
		return false;
	}

	CreateInterfaceFn ifaceFactory = Sys_GetFactory(gameModule);

	if (!ifaceFactory)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s] Failed to locate interface factory in GameDLL module", Plugin_info.logtag);
		return false;
	}

	int retCode = 0;

	g_ReGameApi = (IReGameApi *)ifaceFactory(VRE_GAMEDLL_API_VERSION, &retCode);

	if (!g_ReGameApi)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s] Failed to locate retrieve rehlds api interface from GameDLL module, return code is %d", Plugin_info.logtag, retCode);
		return false;
	}

	int majorVersion = g_ReGameApi->GetMajorVersion();
	int minorVersion = g_ReGameApi->GetMinorVersion();

	if (majorVersion != REGAMEDLL_API_VERSION_MAJOR)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s] ReGameDLL API major version mismatch; expected %d, real %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MAJOR, majorVersion);

		if (majorVersion < REGAMEDLL_API_VERSION_MAJOR)
		{
			gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s] Please update the ReGameDLL up to a major version API >= %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MAJOR);
		}
		else if (majorVersion > REGAMEDLL_API_VERSION_MAJOR)
		{
			gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s] Please update the %s up to a major version API >= %d", Plugin_info.logtag, Plugin_info.logtag, majorVersion);
		}

		return false;
	}

	if (minorVersion < REGAMEDLL_API_VERSION_MINOR)
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s] ReGameDLL API minor version mismatch; expected at least %d, real %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MINOR, minorVersion);
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s] Please update the ReGameDLL up to a minor version API >= %d", Plugin_info.logtag, REGAMEDLL_API_VERSION_MINOR);
		return false;
	}

	g_ReGameFuncs = g_ReGameApi->GetFuncs();

	g_ReGameHookchains = g_ReGameApi->GetHookchains();

	if (!g_ReGameApi->BGetICSEntity(CSENTITY_API_INTERFACE_VERSION))
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s] Interface CCSEntity API version '%s' not found", Plugin_info.logtag, CSENTITY_API_INTERFACE_VERSION);
		return false;
	}

	g_ReGameHookchains->InstallGameRules()->registerHook(ReGameDLL_InstallGameRules);

	g_ReGameHookchains->HandleMenu_ChooseTeam()->registerHook(ReGameDLL_HandleMenu_ChooseTeam);

	g_ReGameHookchains->CBasePlayer_GetIntoGame()->registerHook(ReGameDLL_CBasePlayer_GetIntoGame);

	g_ReGameHookchains->CBasePlayer_SwitchTeam()->registerHook(ReGameDLL_CBasePlayer_SwitchTeam);

	g_ReGameHookchains->CBasePlayer_RoundRespawn()->registerHook(ReGameDLL_CBasePlayer_RoundRespawn);

	g_ReGameHookchains->CBasePlayer_AddAccount()->registerHook(ReGameDLL_CBasePlayer_AddAccount);

	g_ReGameHookchains->CBasePlayer_HasRestrictItem()->registerHook(ReGameDLL_CBasePlayer_HasRestrictItem);

	g_ReGameHookchains->InternalCommand()->registerHook(ReGameDLL_InternalCommand);

	g_ReGameHookchains->CSGameRules_OnRoundFreezeEnd()->registerHook(ReGameDLL_CSGameRules_OnRoundFreezeEnd);

	g_ReGameHookchains->CSGameRules_RestartRound()->registerHook(ReGameDLL_CSGameRules_RestartRound);

	g_ReGameHookchains->RoundEnd()->registerHook(ReGameDLL_RoundEnd);

	g_ReGameHookchains->CBasePlayer_TakeDamage()->registerHook(ReGameDLL_CBasePlayer_TakeDamage);

	g_ReGameHookchains->CGrenade_ExplodeSmokeGrenade()->registerHook(ReGameDLL_CGrenade_ExplodeSmokeGrenade);

	g_ReGameHookchains->CSGameRules_CanPlayerHearPlayer()->registerHook(ReGameDLL_CSGameRules_CanPlayerHearPlayer);

	g_ReGameHookchains->CSGameRules_SendDeathMessage()->registerHook(ReGameDLL_CSGameRules_SendDeathMessage);

	g_ReGameHookchains->CBotManager_OnEvent()->registerHook(ReGameDLL_CBotManager_OnEvent);

	return true;
}

bool ReGameDLL_Stop()
{
	g_ReGameHookchains->InstallGameRules()->unregisterHook(ReGameDLL_InstallGameRules);

	g_ReGameHookchains->HandleMenu_ChooseTeam()->unregisterHook(ReGameDLL_HandleMenu_ChooseTeam);

	g_ReGameHookchains->CBasePlayer_GetIntoGame()->unregisterHook(ReGameDLL_CBasePlayer_GetIntoGame);

	g_ReGameHookchains->CBasePlayer_SwitchTeam()->unregisterHook(ReGameDLL_CBasePlayer_SwitchTeam);

	g_ReGameHookchains->CBasePlayer_RoundRespawn()->unregisterHook(ReGameDLL_CBasePlayer_RoundRespawn);

	g_ReGameHookchains->CBasePlayer_AddAccount()->unregisterHook(ReGameDLL_CBasePlayer_AddAccount);

	g_ReGameHookchains->CBasePlayer_HasRestrictItem()->unregisterHook(ReGameDLL_CBasePlayer_HasRestrictItem);

	g_ReGameHookchains->InternalCommand()->unregisterHook(ReGameDLL_InternalCommand);

	g_ReGameHookchains->CSGameRules_OnRoundFreezeEnd()->unregisterHook(ReGameDLL_CSGameRules_OnRoundFreezeEnd);

	g_ReGameHookchains->CSGameRules_RestartRound()->unregisterHook(ReGameDLL_CSGameRules_RestartRound);

	g_ReGameHookchains->RoundEnd()->unregisterHook(ReGameDLL_RoundEnd);

	g_ReGameHookchains->CBasePlayer_TakeDamage()->unregisterHook(ReGameDLL_CBasePlayer_TakeDamage);

	g_ReGameHookchains->CGrenade_ExplodeSmokeGrenade()->unregisterHook(ReGameDLL_CGrenade_ExplodeSmokeGrenade);

	g_ReGameHookchains->CSGameRules_CanPlayerHearPlayer()->unregisterHook(ReGameDLL_CSGameRules_CanPlayerHearPlayer);

	g_ReGameHookchains->CSGameRules_SendDeathMessage()->unregisterHook(ReGameDLL_CSGameRules_SendDeathMessage);

	g_ReGameHookchains->CBotManager_OnEvent()->unregisterHook(ReGameDLL_CBotManager_OnEvent);

	return true;
}

CGameRules *ReGameDLL_InstallGameRules(IReGameHook_InstallGameRules *chain)
{
	auto gamerules = chain->callNext();

	if (!g_ReGameApi->BGetIGameRules(GAMERULES_API_INTERFACE_VERSION))
	{
		gpMetaUtilFuncs->pfnLogConsole(PLID, "[%s] Interface GameRules API version '%s' not found", Plugin_info.logtag, GAMERULES_API_INTERFACE_VERSION);
	}
	else
	{
		g_pGameRules = gamerules;
	}

	return gamerules;
}

BOOL ReGameDLL_HandleMenu_ChooseTeam(IReGameHook_HandleMenu_ChooseTeam *chain, CBasePlayer *Player, int Slot)
{
	if (gMatchBot.PlayerJoinTeam(Player, Slot))
	{
		Slot = 0;
	}

	return chain->callNext(Player, Slot);
}

bool ReGameDLL_CBasePlayer_GetIntoGame(IReGameHook_CBasePlayer_GetIntoGame *chain, CBasePlayer *Player)
{
	bool ret = chain->callNext(Player);

	gMatchBot.PlayerGetIntoGame(Player);

	gMatchReady.PlayerGetIntoGame(Player);

	gMatchPlayer.PlayerGetIntoGame(Player);

	gMatchStats.PlayerGetIntoGame(Player);

	return ret;
}

void ReGameDLL_CBasePlayer_SwitchTeam(IReGameHook_CBasePlayer_SwitchTeam *chain, CBasePlayer *Player)
{
	chain->callNext(Player);

	gMatchPlayer.PlayerSwitchTeam(Player);

	gMatchStats.PlayerSwitchTeam(Player);
}

void ReGameDLL_CBasePlayer_RoundRespawn(IReGameHook_CBasePlayer_RoundRespawn *chain, CBasePlayer *Player)
{
	chain->callNext(Player);

	gMatchStats.PlayerRespawn(Player);
}

void ReGameDLL_CBasePlayer_AddAccount(IReGameHook_CBasePlayer_AddAccount *chain, CBasePlayer *Player, int Amount, RewardType Type, bool TrackChange)
{
	if (gMatchWarmup.PlayerAddAccount(Player, Amount, Type, TrackChange))
	{
		Amount = 0;
	}

	chain->callNext(Player, Amount, Type, TrackChange);

	gMatchPlayer.PlayerAddAccount(Player, Amount, Type, TrackChange);
}

bool ReGameDLL_CBasePlayer_HasRestrictItem(IReGameHook_CBasePlayer_HasRestrictItem *chain, CBasePlayer *pthis, ItemID item, ItemRestType type)
{
	auto ret = chain->callNext(pthis, item, type);

	if (gMatchRestrictItem.PlayerHasRestrictItem(pthis, item, type))
	{
		ret = true;
	}

	return ret;
}

void ReGameDLL_InternalCommand(IReGameHook_InternalCommand *chain, edict_t *pEntity, const char *pcmd, const char *parg1)
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

void ReGameDLL_CSGameRules_OnRoundFreezeEnd(IReGameHook_CSGameRules_OnRoundFreezeEnd *chain)
{
	chain->callNext();

	gMatchBot.RoundStart();

	gMatchRound.RoundStart();
}

void ReGameDLL_CSGameRules_RestartRound(IReGameHook_CSGameRules_RestartRound *chain)
{
	gMatchStats.RoundRestart();

	chain->callNext();

	gMatchLO3.RoundRestart();

	gMatchPause.RoundRestart();

	gMatchRestrictItem.RoundRestart();
}

bool ReGameDLL_RoundEnd(IReGameHook_RoundEnd *chain, int winStatus, ScenarioEventEndRound event, float tmDelay)
{
	gMatchRound.RoundEnd(winStatus, event, tmDelay);

	auto ret = chain->callNext(winStatus, event, tmDelay);

	gMatchBot.RoundEnd(winStatus, event, tmDelay);

	return ret;
}

int ReGameDLL_CBasePlayer_TakeDamage(IReGameHook_CBasePlayer_TakeDamage *chain, CBasePlayer *pThis, entvars_t *pevInflictor, entvars_t *pevAttacker, float &flDamage, int bitsDamageType)
{
	int ret = chain->callNext(pThis, pevInflictor, pevAttacker, flDamage, bitsDamageType);

	gMatchRound.PlayerDamage(pThis, pevInflictor, pevAttacker, flDamage, bitsDamageType);

	return ret;
}

void ReGameDLL_CGrenade_ExplodeSmokeGrenade(IReGameHook_CGrenade_ExplodeSmokeGrenade *chain, CGrenade *pthis)
{
	chain->callNext(pthis);

	gMatchBugFix.ExplodeSmokeGrenade(pthis);
}

bool ReGameDLL_CSGameRules_CanPlayerHearPlayer(IReGameHook_CSGameRules_CanPlayerHearPlayer *chain, CBasePlayer *pListener, CBasePlayer *pSender)
{
	auto ret = chain->callNext(pListener, pSender);

	if (gMatchMute.GetMute(pListener, pSender))
	{
		return false;
	}

	return ret;
}

void ReGameDLL_CSGameRules_SendDeathMessage(IReGameHook_CSGameRules_SendDeathMessage *chain, CBaseEntity *pKiller, CBasePlayer *pVictim, CBasePlayer *pAssister, entvars_t *pevInflictor, const char *killerWeaponName, int iDeathMessageFlags, int iRarityOfKill)
{
	chain->callNext(pKiller, pVictim, pAssister, pevInflictor, killerWeaponName, iDeathMessageFlags, iRarityOfKill);

	gMatchStats.SendDeathMessage(pKiller, pVictim, pAssister, pevInflictor, killerWeaponName, iDeathMessageFlags, iRarityOfKill);
}

void ReGameDLL_CBotManager_OnEvent(IReGameHook_CBotManager_OnEvent *chain, GameEventType event, CBaseEntity *pEntity, CBaseEntity *pOther)
{
	chain->callNext(event, pEntity, pOther);

	gMatchStats.CBotManager_OnEvent(event, pEntity, pOther);
}