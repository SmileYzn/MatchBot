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
void ReGameDLL_CBasePlayer_AddAccount(IReGameHook_CBasePlayer_AddAccount* chain, CBasePlayer* pthis, int amount, RewardType type, bool bTrackChange);
bool ReGameDLL_CBasePlayer_HasRestrictItem(IReGameHook_CBasePlayer_HasRestrictItem* chain, CBasePlayer* pthis, ItemID item, ItemRestType type);
void ReGameDLL_InternalCommand(IReGameHook_InternalCommand* chain, edict_t* pEntity, const char* pcmd, const char* parg1);
void ReGameDLL_CSGameRules_OnRoundFreezeEnd(IReGameHook_CSGameRules_OnRoundFreezeEnd* chain);
void ReGameDLL_CSGameRules_RestartRound(IReGameHook_CSGameRules_RestartRound* chain);
bool ReGameDLL_RoundEnd(IReGameHook_RoundEnd* chain, int winStatus, ScenarioEventEndRound event, float tmDelay);
int ReGameDLL_CBasePlayer_TakeDamage(IReGameHook_CBasePlayer_TakeDamage* chain, CBasePlayer* pThis, entvars_t* pevInflictor, entvars_t* pevAttacker, float& flDamage, int bitsDamageType);
void ReGameDLL_CSGameRules_PlayerKilled(IReGameHook_CSGameRules_PlayerKilled* chain, CBasePlayer* pVictim, entvars_t* pevKiller, entvars_t* pevInflictor);
void ReGameDLL_CBasePlayer_SetAnimation(IReGameHook_CBasePlayer_SetAnimation* chain, CBasePlayer* pthis, PLAYER_ANIM playerAnim);
CBaseEntity* ReGameDLL_CBasePlayer_DropPlayerItem(IReGameHook_CBasePlayer_DropPlayerItem* chain, CBasePlayer* pthis, const char* pszItemName);
CGrenade* ReGameDLL_PlantBomb(IReGameHook_PlantBomb* chain, entvars_t* pevOwner, Vector& vecStart, Vector& vecVelocity);
void ReGameDLL_CGrenade_DefuseBombStart(IReGameHook_CGrenade_DefuseBombStart* chain, CGrenade* pthis, CBasePlayer* pPlayer);
void ReGameDLL_CGrenade_DefuseBombEnd(IReGameHook_CGrenade_DefuseBombEnd* chain, CGrenade* pthis, CBasePlayer* pPlayer, bool bDefused);
void ReGameDLL_CGrenade_ExplodeBomb(IReGameHook_CGrenade_ExplodeBomb* chain, CGrenade* pthis, TraceResult* ptr, int bitsDamageType);
void ReGameDLL_CGrenade_ExplodeSmokeGrenade(IReGameHook_CGrenade_ExplodeSmokeGrenade* chain, CGrenade* pthis);
void ReGameDLL_CBasePlayer_Duck(IReGameHook_CBasePlayer_Duck* chain, CBasePlayer* Player);