#include "precompiled.h"

plugin_info_t Plugin_info =
{
	META_INTERFACE_VERSION,
	"Match Bot",
	"1.0.4",
	__DATE__,
	"SmileY",
	"https://pugbr.net",
	"BOT",
	PT_ANYTIME,
	PT_ANYTIME,
};

// Metamod Functions Table
META_FUNCTIONS gMetaFunctionTable;

// Engine Functions
enginefuncs_t g_engfuncs;

// Global Variables
globalvars_t *gpGlobals;

// Metamod Functions
meta_globals_t *gpMetaGlobals;
gamedll_funcs_t *gpGamedllFuncs;
mutil_funcs_t *gpMetaUtilFuncs;

C_DLLEXPORT void WINAPI GiveFnptrsToDll(enginefuncs_t *pengfuncsFromEngine, globalvars_t *pGlobals)
{
	memcpy(&g_engfuncs, pengfuncsFromEngine, sizeof(enginefuncs_t));

	gpGlobals = pGlobals;
}

C_DLLEXPORT int Meta_Attach(PLUG_LOADTIME now, META_FUNCTIONS *pFunctionTable, meta_globals_t *pMGlobals, gamedll_funcs_t *pGamedllFuncs)
{
	memset(&gMetaFunctionTable, 0, sizeof(META_FUNCTIONS));

	gMetaFunctionTable.pfnGetEntityAPI2 = GetEntityAPI2;

	gMetaFunctionTable.pfnGetEntityAPI2_Post = GetEntityAPI2_Post;

	gMetaFunctionTable.pfnGetEngineFunctions = GetEngineFunctions;

	gMetaFunctionTable.pfnGetEngineFunctions_Post = GetEngineFunctions_Post;

	memcpy(pFunctionTable, &gMetaFunctionTable, sizeof(META_FUNCTIONS));

	gpMetaGlobals = pMGlobals;

	gpGamedllFuncs = pGamedllFuncs;

	ReAPI_Init();

	ReGameDLL_Init();

	return 1;
}

C_DLLEXPORT int Meta_Detach(PLUG_LOADTIME now, PL_UNLOAD_REASON	reason)
{
	ReAPI_Stop();

	ReGameDLL_Stop();

	return 1;
}

C_DLLEXPORT int Meta_Query(char *interfaceVersion, plugin_info_t **pPlugInfo, mutil_funcs_t *pMetaUtilFuncs)
{
	*pPlugInfo = &Plugin_info;

	gpMetaUtilFuncs = pMetaUtilFuncs;

	return 1;
}
