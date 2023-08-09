#pragma once

extern IReGameApi			*g_ReGameApi;
extern const ReGameFuncs_t	*g_ReGameFuncs;
extern IReGameHookchains	*g_ReGameHookchains;
extern CGameRules			*g_pGameRules;

extern bool ReGameDLL_Init();
extern bool ReGameDLL_Stop();

CGameRules *ReGameDLL_InstallGameRules(IReGameHook_InstallGameRules* chain);
BOOL ReGameDLL_HandleMenu_ChooseTeam(IReGameHook_HandleMenu_ChooseTeam* chain, CBasePlayer* Player, int Slot);
bool ReGameDLL_CBasePlayer_GetIntoGame(IReGameHook_CBasePlayer_GetIntoGame* chain, CBasePlayer* Player);
void ReGameDLL_CBasePlayer_SwitchTeam(IReGameHook_CBasePlayer_SwitchTeam* chain, CBasePlayer* Player);
void ReGameDLL_CBasePlayer_AddAccount(IReGameHook_CBasePlayer_AddAccount* chain, CBasePlayer* pthis, int amount, RewardType type, bool bTrackChange);
bool ReGameDLL_CBasePlayer_HasRestrictItem(IReGameHook_CBasePlayer_HasRestrictItem* chain, CBasePlayer* pthis, ItemID item, ItemRestType type);
void ReGameDLL_InternalCommand(IReGameHook_InternalCommand* chain, edict_t* pEntity, const char* pcmd, const char* parg1);
void ReGameDLL_CSGameRules_OnRoundFreezeEnd(IReGameHook_CSGameRules_OnRoundFreezeEnd* chain);
void ReGameDLL_CSGameRules_RestartRound(IReGameHook_CSGameRules_RestartRound* chain);
bool ReGameDLL_RoundEnd(IReGameHook_RoundEnd* chain, int winStatus, ScenarioEventEndRound event, float tmDelay);
int ReGameDLL_CBasePlayer_TakeDamage(IReGameHook_CBasePlayer_TakeDamage* chain, CBasePlayer* pThis, entvars_t* pevInflictor, entvars_t* pevAttacker, float& flDamage, int bitsDamageType);
void ReGameDLL_CGrenade_ExplodeSmokeGrenade(IReGameHook_CGrenade_ExplodeSmokeGrenade* chain, CGrenade* pthis);
bool ReGameDLL_CSGameRules_CanPlayerHearPlayer(IReGameHook_CSGameRules_CanPlayerHearPlayer* chain, CBasePlayer* pListener, CBasePlayer* pSender);