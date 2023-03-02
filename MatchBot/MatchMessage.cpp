#include "precompiled.h"

CMatchMessage gMatchMessage;

void CMatchMessage::RegisterHook(char* MsgName, void (*Function)(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity))
{
	auto MsgId = GET_USER_MSG_ID(PLID, MsgName, 0);
	
	if (MsgId)
	{

	}
}

void CMatchMessage::MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity)
{

}

void CMatchMessage::MessageEnd()
{
	/**/
}

void CMatchMessage::WriteByte(int iValue)
{

}

void CMatchMessage::WriteChar(int iValue)
{

}

void CMatchMessage::WriteShort(int iValue)
{

}

void CMatchMessage::WriteLong(int iValue)
{

}

void CMatchMessage::WriteAngle(float flValue)
{

}

void CMatchMessage::WriteCoord(float flValue)
{

}

void CMatchMessage::WriteString(const char* szValue)
{

}

void CMatchMessage::WriteEntity(int iValue)
{

}