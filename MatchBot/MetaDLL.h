#pragma once

#pragma region DLL_PRE
BOOL DLL_PRE_ClientConnect(edict_t* pEntity, const char* pszName, const char* pszAddress, char szRejectReason[128]);
#pragma endregion

#pragma region DLL_POST
void DLL_POST_ServerActivate(edict_t* pEdictList, int edictCount, int clientMax);
void DLL_POST_ServerDeactivate(void);
void DLL_POST_StartFrame(void);
#pragma endregion