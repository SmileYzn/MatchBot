#include "precompiled.h"

CMatchMessage gMatchMessage;

void CMatchMessage::RegisterHook(char* MessageName, void (*Function)(void*))
{
	auto MessgeIndex = GET_USER_MSG_ID(PLID, MessageName, 0);

	if (MessgeIndex)
	{
		this->MessageHooks[MessgeIndex].function = Function;
	}
}

void CMatchMessage::MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity)
{
	this->CurrentMessageIndex = 0;

	this->CurrentMessageType = msg_type;

	if (this->MessageHooks[this->CurrentMessageType].function)
	{
		this->MessageActive = true;

		this->pCurentEntity = pEntity;
	}
	else
	{
		this->MessageActive = false;
	}
}

void CMatchMessage::MessageEnd()
{
	if (this->MessageHooks[this->CurrentMessageType].function)
	{
		(*this->MessageHooks[this->CurrentMessageType].function)(nullptr);
	}
}

void CMatchMessage::WriteByte(int iValue)
{
	if (this->MessageActive)
	{
		message_var_t message_var = { 0 };
		message_t message = { 0 };

		message_var.iValue = iValue;

		//SERVER_PRINT(temp->sValue);

		message.iType = MESSAGE_TYPE_BYTE;

		message.mValue = message_var;

		//CLEAR_VAR(CurrentMessageIndex);

		this->CurrentMessage[this->CurrentMessageIndex] = message;

		this->CurrentMessageIndex++;
	}
}

void CMatchMessage::WriteChar(int iValue)
{
	if (this->MessageActive)
	{
		message_var_t message_var = { 0 };
		message_t message = { 0 };

		message_var.iValue = iValue;

		//SERVER_PRINT(temp->sValue);

		message.iType = MESSAGE_TYPE_CHAR;

		message.mValue = message_var;

		//CLEAR_VAR(CurrentMessageIndex);

		this->CurrentMessage[this->CurrentMessageIndex] = message;

		this->CurrentMessageIndex++;
	}
}

void CMatchMessage::WriteShort(int iValue)
{
	if (this->MessageActive)
	{
		message_var_t message_var = { 0 };
		message_t message = { 0 };

		message_var.iValue = iValue;

		//SERVER_PRINT(temp->sValue);

		message.iType = MESSAGE_TYPE_SHORT;

		message.mValue = message_var;

		//CLEAR_VAR(CurrentMessageIndex);

		this->CurrentMessage[this->CurrentMessageIndex] = message;

		this->CurrentMessageIndex++;
	}
}

void CMatchMessage::WriteLong(int iValue)
{
	if (this->MessageActive)
	{
		message_var_t message_var = { 0 };
		message_t message = { 0 };

		message_var.iValue = iValue;

		//SERVER_PRINT(temp->sValue);

		message.iType = MESSAGE_TYPE_LONG;

		message.mValue = message_var;

		//CLEAR_VAR(CurrentMessageIndex);

		this->CurrentMessage[this->CurrentMessageIndex] = message;

		this->CurrentMessageIndex++;
	}
}

void CMatchMessage::WriteAngle(float flValue)
{
	if (this->MessageActive)
	{
		message_var_t message_var = { 0 };
		message_t message = { 0 };

		message_var.fValue = flValue;

		//SERVER_PRINT(temp->sValue);

		message.iType = MESSAGE_TYPE_ANGLE;
		message.mValue = message_var;

		//CLEAR_VAR(CurrentMessageIndex);

		this->CurrentMessage[this->CurrentMessageIndex] = message;

		this->CurrentMessageIndex++;
	}
}

void CMatchMessage::WriteCoord(float flValue)
{
	if (this->MessageActive)
	{
		message_var_t message_var = { 0 };
		message_t message = { 0 };

		message_var.fValue = flValue;

		//SERVER_PRINT(temp->sValue);

		message.iType = MESSAGE_TYPE_COORD;
		message.mValue = message_var;

		//CLEAR_VAR(CurrentMessageIndex);

		this->CurrentMessage[this->CurrentMessageIndex] = message;

		this->CurrentMessageIndex++;
	}
}

void CMatchMessage::WriteString(const char* szValue)
{
	if (this->MessageActive)
	{
		message_var_t message_var = { 0 };
		message_t message = { 0 };

		Q_strncpy(message_var.sValue, szValue, sizeof(message_var.sValue));

		//SERVER_PRINT(temp->sValue);

		message.iType = MESSAGE_TYPE_STRING;
		message.mValue = message_var;

		//CLEAR_VAR(CurrentMessageIndex);

		this->CurrentMessage[this->CurrentMessageIndex] = message;

		this->CurrentMessageIndex++;
	}
}

void CMatchMessage::WriteEntity(int iValue)
{
	if (this->MessageActive)
	{
		message_var_t message_var = { 0 };
		message_t message = { 0 };

		message_var.iValue = iValue;

		//SERVER_PRINT(temp->sValue);

		message.iType = MESSAGE_TYPE_ENTITY;

		message.mValue = message_var;

		//CLEAR_VAR(CurrentMessageIndex);

		this->CurrentMessage[this->CurrentMessageIndex] = message;

		this->CurrentMessageIndex++;
	}
}

int CMatchMessage::GetByte(int ParamIndex)
{
	if (ParamIndex >= this->CurrentMessageIndex)
	{
		return -1;
	}

	if (this->CurrentMessage[ParamIndex].iType != MESSAGE_TYPE_BYTE)
	{
		return -2;
	}

	return this->CurrentMessage[ParamIndex].mValue.iValue;
}

int CMatchMessage::GetChar(int ParamIndex)
{
	if (ParamIndex >= this->CurrentMessageIndex)
	{
		return -1;
	}

	if (this->CurrentMessage[ParamIndex].iType != MESSAGE_TYPE_CHAR)
	{
		return -2;
	}

	return this->CurrentMessage[ParamIndex].mValue.iValue;
}

int CMatchMessage::GetShort(int ParamIndex)
{
	if (ParamIndex >= this->CurrentMessageIndex)
	{
		return -1;
	}

	if (this->CurrentMessage[ParamIndex].iType != MESSAGE_TYPE_SHORT)
	{
		return -2;
	}

	return this->CurrentMessage[ParamIndex].mValue.iValue;
}

int CMatchMessage::GetLong(int ParamIndex)
{
	if (ParamIndex >= this->CurrentMessageIndex)
	{
		return -1;
	}

	if (this->CurrentMessage[ParamIndex].iType != MESSAGE_TYPE_LONG)
	{
		return -2;
	}

	return this->CurrentMessage[ParamIndex].mValue.iValue;
}

float CMatchMessage::GetAngle(int ParamIndex)
{
	if (ParamIndex >= this->CurrentMessageIndex)
	{
		return -1;
	}

	if (this->CurrentMessage[ParamIndex].iType != MESSAGE_TYPE_ANGLE)
	{
		return -2;
	}

	return this->CurrentMessage[ParamIndex].mValue.fValue;
}

float CMatchMessage::GetCoord(int ParamIndex)
{
	if (ParamIndex >= this->CurrentMessageIndex)
	{
		return -1;
	}

	if (this->CurrentMessage[ParamIndex].iType != MESSAGE_TYPE_COORD)
	{
		return -2;
	}

	return this->CurrentMessage[ParamIndex].mValue.fValue;
}

char* CMatchMessage::GetString(int ParamIndex)
{
	if (ParamIndex >= this->CurrentMessageIndex)
	{
		return nullptr;
	}

	if (this->CurrentMessage[ParamIndex].iType != MESSAGE_TYPE_STRING)
	{
		return nullptr;
	}

	return this->CurrentMessage[ParamIndex].mValue.sValue;
}

int CMatchMessage::GetEntity(int ParamIndex)
{
	if (ParamIndex >= this->CurrentMessageIndex)
	{
		return -1;
	}

	if (this->CurrentMessage[ParamIndex].iType != MESSAGE_TYPE_ENTITY)
	{
		return -2;
	}

	return this->CurrentMessage[ParamIndex].mValue.iValue;
}