#include "precompiled.h"

// DLL PRE Functions Table
DLL_FUNCTIONS gDLL_FunctionTable_Pre;

// DLL POST Functions Table
DLL_FUNCTIONS gDLL_FunctionTable_Post;

#pragma region DLL_PRE
C_DLLEXPORT int GetEntityAPI2(DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion)
{
	memset(&gDLL_FunctionTable_Pre, 0, sizeof(DLL_FUNCTIONS));

	gDLL_FunctionTable_Pre.pfnClientConnect = DLL_PRE_ClientConnect;

	memcpy(pFunctionTable, &gDLL_FunctionTable_Pre, sizeof(DLL_FUNCTIONS));

	return 1;
}

BOOL DLL_PRE_ClientConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128])
{
	if (!gMatchAdmin.PlayerConnect(pEntity, pszName, pszAddress, szRejectReason))
	{
		RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);
		return FALSE;
	}

	if (!gMatchBot.PlayerConnect(pEntity, pszName, pszAddress, szRejectReason))
	{
		RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);
		return FALSE;
	}

	if (!gMatchStats.PlayerConnect(pEntity, pszName, pszAddress, szRejectReason))
	{
		RETURN_META_VALUE(MRES_SUPERCEDE, FALSE);
		return FALSE;
	}

	RETURN_META_VALUE(MRES_IGNORED, TRUE);
	return TRUE;
}
#pragma endregion

#pragma region DLL_POST
C_DLLEXPORT int GetEntityAPI2_Post(DLL_FUNCTIONS* pFunctionTable, int* interfaceVersion)
{
	memset(&gDLL_FunctionTable_Post, 0, sizeof(DLL_FUNCTIONS));

	gDLL_FunctionTable_Post.pfnServerActivate = DLL_POST_ServerActivate;

	gDLL_FunctionTable_Post.pfnServerDeactivate = DLL_POST_ServerDeactivate;

	gDLL_FunctionTable_Post.pfnStartFrame = DLL_POST_StartFrame;

	memcpy(pFunctionTable, &gDLL_FunctionTable_Post, sizeof(DLL_FUNCTIONS));

	return 1;
}

void DLL_POST_ServerActivate(edict_t* pEdictList, int edictCount, int clientMax)
{
	gMatchTask.ServerActivate();

	gMatchLanguage.ServerActivate();

	gMatchBot.ServerActivate();

	//gMatchScore.ServerActivate();

	//gMatchAdmin.ServerActivate();

	//gMatchVoteMenu.ServerActivate();

	RETURN_META(MRES_IGNORED);
}

void DLL_POST_ServerDeactivate(void)
{
	gMatchTask.ServerDeactivate();

	gMatchBot.ServerDeactivate();

	RETURN_META(MRES_IGNORED);
}

void DLL_POST_StartFrame(void)
{
	gMatchTask.Frame();

	RETURN_META(MRES_IGNORED);
}
#pragma endregion