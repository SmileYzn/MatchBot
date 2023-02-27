#pragma once

#pragma region ENGINE_POST
void ENGINE_PRE_MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed);
void ENGINE_PRE_WriteByte(int iValue);
void ENGINE_PRE_WriteChar(int iValue);
void ENGINE_PRE_WriteShort(int iValue);
void ENGINE_PRE_WriteLong(int iValue);
void ENGINE_PRE_WriteAngle(float flValue);
void ENGINE_PRE_WriteCoord(float flValue);
void ENGINE_PRE_WriteString(const char* sz);
void ENGINE_PRE_WriteEntity(int iValue);
void ENGINE_PRE_MessageEnd(void);
#pragma endregion

#pragma region ENGINE_POST
//void ENGINE_POST_MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed);
//void ENGINE_POST_WriteByte(int iValue);
//void ENGINE_POST_WriteChar(int iValue);
//void ENGINE_POST_WriteShort(int iValue);
//void ENGINE_POST_WriteLong(int iValue);
//void ENGINE_POST_WriteAngle(float flValue);
//void ENGINE_POST_WriteCoord(float flValue);
//void ENGINE_POST_WriteString(const char* sz);
//void ENGINE_POST_WriteEntity(int iValue);
//void ENGINE_POST_MessageEnd(void);
#pragma endregion