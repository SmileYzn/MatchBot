#pragma once

#pragma region ENGINE_POST
#pragma endregion

#pragma region ENGINE_POST
int ENGINE_POST_RegUserMsg(const char* pszName, int iSize);
void ENGINE_POST_MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed);
void ENGINE_POST_WriteByte(int iValue);
void ENGINE_POST_WriteChar(int iValue);
void ENGINE_POST_WriteShort(int iValue);
void ENGINE_POST_WriteLong(int iValue);
void ENGINE_POST_WriteAngle(float flValue);
void ENGINE_POST_WriteCoord(float flValue);
void ENGINE_POST_WriteString(const char* sz);
void ENGINE_POST_WriteEntity(int iValue);
void ENGINE_POST_MessageEnd(void);
#pragma endregion