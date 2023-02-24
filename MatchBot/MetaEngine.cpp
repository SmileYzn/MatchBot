#include "precompiled.h"

// ENGINE POST Functions Table
enginefuncs_t gENGINE_FunctionTable_Pre;

// ENGINE POST Functions Table
enginefuncs_t gENGINE_FunctionTable_Post;

#pragma region ENGINE_PRE
C_DLLEXPORT int GetEngineFunctions(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion)
{
	memset(&gENGINE_FunctionTable_Pre, 0, sizeof(enginefuncs_t));

	// Register Functions Here

	memcpy(pengfuncsFromEngine, &gENGINE_FunctionTable_Pre, sizeof(enginefuncs_t));

	return 1;
}
#pragma endregion

#pragma region ENGINE_POST
C_DLLEXPORT int GetEngineFunctions_Post(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion)
{
	memset(&gENGINE_FunctionTable_Post, 0, sizeof(enginefuncs_t));

	// Register Functions Here
	gENGINE_FunctionTable_Post.pfnRegUserMsg = ENGINE_POST_RegUserMsg;
	gENGINE_FunctionTable_Post.pfnMessageBegin = ENGINE_POST_MessageBegin;
	gENGINE_FunctionTable_Post.pfnMessageEnd = ENGINE_POST_MessageEnd;
	gENGINE_FunctionTable_Post.pfnWriteByte = ENGINE_POST_WriteByte;
	gENGINE_FunctionTable_Post.pfnWriteChar = ENGINE_POST_WriteChar;
	gENGINE_FunctionTable_Post.pfnWriteShort = ENGINE_POST_WriteShort;
	gENGINE_FunctionTable_Post.pfnWriteLong = ENGINE_POST_WriteLong;
	gENGINE_FunctionTable_Post.pfnWriteAngle = ENGINE_POST_WriteAngle;
	gENGINE_FunctionTable_Post.pfnWriteCoord = ENGINE_POST_WriteCoord;
	gENGINE_FunctionTable_Post.pfnWriteString = ENGINE_POST_WriteString;
	gENGINE_FunctionTable_Post.pfnWriteEntity = ENGINE_POST_WriteEntity;

	memcpy(pengfuncsFromEngine, &gENGINE_FunctionTable_Post, sizeof(enginefuncs_t));

	return 1;
}

int ENGINE_POST_RegUserMsg(const char* pszName, int iSize)
{
	auto MetaResult = gMatchMessage.RegUserMsg(pszName, iSize);

	RETURN_META_VALUE(MetaResult, 0);
}

void ENGINE_POST_MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed)
{
	auto MetaResult = gMatchMessage.MessageBegin(msg_dest, msg_type, pOrigin, ed);

	RETURN_META(MetaResult);
}

void ENGINE_POST_WriteByte(int iValue)
{
	auto MetaResult = gMatchMessage.WriteByte(iValue);

	RETURN_META(MetaResult);
}

void ENGINE_POST_WriteChar(int iValue)
{
	auto MetaResult = gMatchMessage.WriteChar(iValue);

	RETURN_META(MetaResult);
}

void ENGINE_POST_WriteShort(int iValue)
{
	auto MetaResult = gMatchMessage.WriteShort(iValue);

	RETURN_META(MetaResult);
}

void ENGINE_POST_WriteLong(int iValue)
{
	auto MetaResult = gMatchMessage.WriteLong(iValue);

	RETURN_META(MetaResult);
}

void ENGINE_POST_WriteAngle(float flValue)
{
	auto MetaResult = gMatchMessage.WriteAngle(flValue);

	RETURN_META(MetaResult);
}

void ENGINE_POST_WriteCoord(float flValue)
{
	auto MetaResult = gMatchMessage.WriteCoord(flValue);

	RETURN_META(MetaResult);
}

void ENGINE_POST_WriteString(const char* sz)
{
	auto MetaResult = gMatchMessage.WriteString(sz);

	RETURN_META(MetaResult);
}

void ENGINE_POST_WriteEntity(int iValue)
{
	auto MetaResult = gMatchMessage.WriteByte(iValue);

	RETURN_META(MetaResult);
}

void ENGINE_POST_MessageEnd(void)
{
	auto MetaResult = gMatchMessage.MessageEnd();

	RETURN_META(MetaResult);
}
#pragma endregion
