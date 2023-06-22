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
	gENGINE_FunctionTable_Pre.pfnMessageBegin = ENGINE_PRE_MessageBegin;
	gENGINE_FunctionTable_Pre.pfnMessageEnd = ENGINE_PRE_MessageEnd;
	gENGINE_FunctionTable_Pre.pfnWriteByte = ENGINE_PRE_WriteByte;
	gENGINE_FunctionTable_Pre.pfnWriteChar = ENGINE_PRE_WriteChar;
	gENGINE_FunctionTable_Pre.pfnWriteShort = ENGINE_PRE_WriteShort;
	gENGINE_FunctionTable_Pre.pfnWriteLong = ENGINE_PRE_WriteLong;
	gENGINE_FunctionTable_Pre.pfnWriteAngle = ENGINE_PRE_WriteAngle;
	gENGINE_FunctionTable_Pre.pfnWriteCoord = ENGINE_PRE_WriteCoord;
	gENGINE_FunctionTable_Pre.pfnWriteString = ENGINE_PRE_WriteString;
	gENGINE_FunctionTable_Pre.pfnWriteEntity = ENGINE_PRE_WriteEntity;

	memcpy(pengfuncsFromEngine, &gENGINE_FunctionTable_Pre, sizeof(enginefuncs_t));

	return 1;
}

void ENGINE_PRE_MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed)
{
	if (gMatchMessage.MessageBegin(msg_dest, msg_type, pOrigin, ed))
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	else
	{
		RETURN_META(MRES_IGNORED);
	}
}

void ENGINE_PRE_MessageEnd(void)
{
	if (gMatchMessage.MessageEnd())
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	else
	{
		RETURN_META(MRES_IGNORED);
	}
}

void ENGINE_PRE_WriteByte(int iValue)
{
	if (gMatchMessage.WriteByte(iValue))
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	else
	{
		RETURN_META(MRES_IGNORED);
	}
}

void ENGINE_PRE_WriteChar(int iValue)
{
	if (gMatchMessage.WriteChar(iValue))
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	else
	{
		RETURN_META(MRES_IGNORED);
	}
}

void ENGINE_PRE_WriteShort(int iValue)
{
	if (gMatchMessage.WriteShort(iValue))
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	else
	{
		RETURN_META(MRES_IGNORED);
	}
}

void ENGINE_PRE_WriteLong(int iValue)
{
	if (gMatchMessage.WriteLong(iValue))
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	else
	{
		RETURN_META(MRES_IGNORED);
	}
}

void ENGINE_PRE_WriteAngle(float flValue)
{
	if (gMatchMessage.WriteAngle(flValue))
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	else
	{
		RETURN_META(MRES_IGNORED);
	}
}

void ENGINE_PRE_WriteCoord(float flValue)
{
	if (gMatchMessage.WriteCoord(flValue))
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	else
	{
		RETURN_META(MRES_IGNORED);
	}
}

void ENGINE_PRE_WriteString(const char* sz)
{
	if (gMatchMessage.WriteString(sz))
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	else
	{
		RETURN_META(MRES_IGNORED);
	}
}

void ENGINE_PRE_WriteEntity(int iValue)
{
	if (gMatchMessage.WriteEntity(iValue))
	{
		RETURN_META(MRES_SUPERCEDE);
	}
	else
	{
		RETURN_META(MRES_IGNORED);
	}
}
#pragma endregion

#pragma region ENGINE_POST
C_DLLEXPORT int GetEngineFunctions_Post(enginefuncs_t *pengfuncsFromEngine, int *interfaceVersion)
{
	memset(&gENGINE_FunctionTable_Post, 0, sizeof(enginefuncs_t));

	// Register Functions Here
	
	//gENGINE_FunctionTable_Post.pfnMessageBegin = ENGINE_POST_MessageBegin;
	//gENGINE_FunctionTable_Post.pfnMessageEnd = ENGINE_POST_MessageEnd;
	//gENGINE_FunctionTable_Post.pfnWriteByte = ENGINE_POST_WriteByte;
	//gENGINE_FunctionTable_Post.pfnWriteChar = ENGINE_POST_WriteChar;
	//gENGINE_FunctionTable_Post.pfnWriteShort = ENGINE_POST_WriteShort;
	//gENGINE_FunctionTable_Post.pfnWriteLong = ENGINE_POST_WriteLong;
	//gENGINE_FunctionTable_Post.pfnWriteAngle = ENGINE_POST_WriteAngle;
	//gENGINE_FunctionTable_Post.pfnWriteCoord = ENGINE_POST_WriteCoord;
	//gENGINE_FunctionTable_Post.pfnWriteString = ENGINE_POST_WriteString;
	//gENGINE_FunctionTable_Post.pfnWriteEntity = ENGINE_POST_WriteEntity;

	memcpy(pengfuncsFromEngine, &gENGINE_FunctionTable_Post, sizeof(enginefuncs_t));

	return 1;
}

//void ENGINE_POST_MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* ed)
//{
//	gMatchMessage.MessageBegin(msg_dest, msg_type, pOrigin, ed);
//
//	RETURN_META(MRES_IGNORED);
//}
//
//void ENGINE_POST_MessageEnd(void)
//{
//	gMatchMessage.MessageEnd();
//
//	RETURN_META(MRES_IGNORED);
//}
//
//void ENGINE_POST_WriteByte(int iValue)
//{
//	gMatchMessage.WriteByte(iValue);
//
//	RETURN_META(MRES_IGNORED);
//}
//
//void ENGINE_POST_WriteChar(int iValue)
//{
//	gMatchMessage.WriteChar(iValue);
//
//	RETURN_META(MRES_IGNORED);
//}
//
//void ENGINE_POST_WriteShort(int iValue)
//{
//	gMatchMessage.WriteShort(iValue);
//
//	RETURN_META(MRES_IGNORED);
//}
//
//void ENGINE_POST_WriteLong(int iValue)
//{
//	gMatchMessage.WriteLong(iValue);
//
//	RETURN_META(MRES_IGNORED);
//}
//
//void ENGINE_POST_WriteAngle(float flValue)
//{
//	gMatchMessage.WriteAngle(flValue);
//
//	RETURN_META(MRES_IGNORED);
//}
//
//void ENGINE_POST_WriteCoord(float flValue)
//{
//	gMatchMessage.WriteCoord(flValue);
//
//	RETURN_META(MRES_IGNORED);
//}
//
//void ENGINE_POST_WriteString(const char* sz)
//{
//	gMatchMessage.WriteString(sz);
//
//	RETURN_META(MRES_IGNORED);
//}
//
//void ENGINE_POST_WriteEntity(int iValue)
//{
//	gMatchMessage.WriteEntity(iValue);
//
//	RETURN_META(MRES_IGNORED);
//}
#pragma endregion
