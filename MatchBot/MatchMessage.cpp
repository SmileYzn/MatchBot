#include "precompiled.h"

CMatchMessage gMatchMessage;

// Register message hook
void CMatchMessage::RegisterHook(const char* MsgName, bool (*Function)(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity))
{
	// Get message index from message name
	auto MsgId = GET_USER_MSG_ID(PLID, MsgName, 0);

	if (MsgId)
	{
		this->m_Hook[MsgId].Function = Function;
	}
}

bool CMatchMessage::MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity)
{
	// If message is hooked
	if (this->m_Hook.find(msg_type) != this->m_Hook.end())
	{
		// Current mesage index
		this->m_MsgId = msg_type;

		// Set message dest entity
		this->m_Data.msg_dest = msg_dest;

		// Set message type (Message index)
		this->m_Data.msg_type = msg_type;

		// Set Message origin coordinates
		this->m_Data.pOrigin = pOrigin;

		// Set message entity
		this->m_Data.pEntity = pEntity;

		// Reset parameters
		this->m_Data.Param.clear();

		// Return true
		return true;
	}

	// Reset current message index
	this->m_MsgId = 0;

	// Return false
	return false;
}

bool CMatchMessage::MessageEnd()
{
	if (this->m_MsgId)
	{
		auto Hook = this->m_Hook.find(this->m_MsgId);

		if (Hook != this->m_Hook.end())
		{
			if (Hook->second.Function)
			{
				auto BlockMessage = ((bool(*)(int msg_dest, int msg_type, const float* pOrigin, edict_t * pEntity))Hook->second.Function)(this->m_Data.msg_dest, this->m_Data.msg_type, this->m_Data.pOrigin, this->m_Data.pEntity);

				if (!BlockMessage)
				{
					MESSAGE_BEGIN(this->m_Data.msg_dest, this->m_Data.msg_type, this->m_Data.pOrigin, this->m_Data.pEntity);

					for (auto const& Param : this->m_Data.Param)
					{
						switch (Param.second.Type)
						{
							case MESSAGE_TYPE_BYTE:
							{
								WRITE_BYTE(Param.second.iValue);
								break;
							}
							case MESSAGE_TYPE_CHAR:
							{
								WRITE_CHAR(Param.second.iValue);
								break;
							}
							case MESSAGE_TYPE_SHORT:
							{
								WRITE_SHORT(Param.second.iValue);
								break;
							}
							case MESSAGE_TYPE_LONG:
							{
								WRITE_LONG(Param.second.iValue);
								break;
							}
							case MESSAGE_TYPE_ANGLE:
							{
								WRITE_ANGLE(Param.second.flValue);
								break;
							}
							case MESSAGE_TYPE_COORD:
							{
								WRITE_COORD(Param.second.flValue);
								break;
							}
							case MESSAGE_TYPE_STRING:
							{
								WRITE_STRING(Param.second.szValue);
								break;
							}
							case MESSAGE_TYPE_ENTITY:
							{
								WRITE_ENTITY(Param.second.iValue);
								break;
							}
						}
					}

					MESSAGE_END();
				}
			}
		}

		// Reset current message index
		this->m_MsgId = 0;

		// Return true
		return true;
	}

	// Return false
	return false;
}

bool CMatchMessage::WriteByte(int iValue)
{
	if (this->m_MsgId)
	{
		P_MESSAGE_PARAM Param = { 0 };

		Param.Type = MESSAGE_TYPE_BYTE;
		Param.iValue = iValue;

		this->m_Data.Param[this->m_Data.Param.size()] = Param;

		return true;
	}

	return false;
}

bool CMatchMessage::WriteChar(int iValue)
{
	if (this->m_MsgId)
	{
		P_MESSAGE_PARAM Param = { 0 };

		Param.Type = MESSAGE_TYPE_CHAR;
		Param.iValue = iValue;

		this->m_Data.Param[this->m_Data.Param.size()] = Param;

		return true;
	}

	return false;
}

bool CMatchMessage::WriteShort(int iValue)
{
	if (this->m_MsgId)
	{
		P_MESSAGE_PARAM Param = { 0 };

		Param.Type = MESSAGE_TYPE_SHORT;
		Param.iValue = iValue;

		this->m_Data.Param[this->m_Data.Param.size()] = Param;

		return true;
	}

	return false;
}

bool CMatchMessage::WriteLong(int iValue)
{
	if (this->m_MsgId)
	{
		P_MESSAGE_PARAM Param = { 0 };

		Param.Type = MESSAGE_TYPE_LONG;
		Param.iValue = iValue;

		this->m_Data.Param[this->m_Data.Param.size()] = Param;

		return true;
	}

	return false;
}

bool CMatchMessage::WriteAngle(float flValue)
{
	if (this->m_MsgId)
	{
		P_MESSAGE_PARAM Param = { 0 };

		Param.Type = MESSAGE_TYPE_ANGLE;
		Param.flValue = flValue;

		this->m_Data.Param[this->m_Data.Param.size()] = Param;

		return true;
	}

	return false;
}

bool CMatchMessage::WriteCoord(float flValue)
{
	if (this->m_MsgId)
	{
		P_MESSAGE_PARAM Param = { 0 };

		Param.Type = MESSAGE_TYPE_COORD;
		Param.flValue = flValue;

		this->m_Data.Param[this->m_Data.Param.size()] = Param;

		return true;
	}

	return false;
}

bool CMatchMessage::WriteString(const char* szValue)
{
	if (this->m_MsgId)
	{
		P_MESSAGE_PARAM Param = { 0 };

		Param.Type = MESSAGE_TYPE_STRING;

		Q_strncpy(Param.szValue, szValue, sizeof(Param.szValue));

		this->m_Data.Param[this->m_Data.Param.size()] = Param;

		return true;
	}

	return false;
}

bool CMatchMessage::WriteEntity(int iValue)
{
	if (this->m_MsgId)
	{
		P_MESSAGE_PARAM Param = { 0 };

		Param.Type = MESSAGE_TYPE_ENTITY;
		Param.iValue = iValue;

		this->m_Data.Param[this->m_Data.Param.size()] = Param;

		return true;
	}

	return false;
}

// Get parameter byte
int CMatchMessage::GetByte(int Id)
{
	auto Param = this->m_Data.Param.find(Id);

	if (Param != this->m_Data.Param.end())
	{
		if (Param->second.Type == MESSAGE_TYPE_BYTE)
		{
			return Param->second.iValue;
		}
	}

	return  0;
}

// Get parameter character
int CMatchMessage::GetChar(int Id)
{
	auto Param = this->m_Data.Param.find(Id);

	if (Param != this->m_Data.Param.end())
	{
		if (Param->second.Type == MESSAGE_TYPE_CHAR)
		{
			return Param->second.iValue;
		}
	}

	return  0;
}

// Get parameter short
int CMatchMessage::GetShort(int Id)
{
	auto Param = this->m_Data.Param.find(Id);

	if (Param != this->m_Data.Param.end())
	{
		if (Param->second.Type == MESSAGE_TYPE_SHORT)
		{
			return Param->second.iValue;
		}
	}

	return  0;
}

// Get parameter long
int CMatchMessage::GetLong(int Id)
{
	auto Param = this->m_Data.Param.find(Id);

	if (Param != this->m_Data.Param.end())
	{
		if (Param->second.Type == MESSAGE_TYPE_LONG)
		{
			return Param->second.iValue;
		}
	}

	return  0;
}

// Get parameter angle
float CMatchMessage::GetAngle(int Id)
{
	auto Param = this->m_Data.Param.find(Id);

	if (Param != this->m_Data.Param.end())
	{
		if (Param->second.Type == MESSAGE_TYPE_ANGLE)
		{
			return Param->second.flValue;
		}
	}

	return  0.0f;
}

// Get parameter coordinate
float CMatchMessage::GetCoord(int Id)
{
	auto Param = this->m_Data.Param.find(Id);

	if (Param != this->m_Data.Param.end())
	{
		if (Param->second.Type == MESSAGE_TYPE_COORD)
		{
			return Param->second.flValue;
		}
	}

	return  0.0f;
}

// Get parameter string
const char* CMatchMessage::GetString(int Id)
{
	auto Param = this->m_Data.Param.find(Id);

	if (Param != this->m_Data.Param.end())
	{
		if (Param->second.Type == MESSAGE_TYPE_STRING)
		{
			return Param->second.szValue;
		}
	}

	return  nullptr;
}

// Get parameter entity index
int CMatchMessage::GetEntity(int Id)
{
	auto Param = this->m_Data.Param.find(Id);

	if (Param != this->m_Data.Param.end())
	{
		if (Param->second.Type == MESSAGE_TYPE_ENTITY)
		{
			return Param->second.iValue;
		}
	}

	return  0;
}

// Set parameter integer
void CMatchMessage::SetArgInt(int Id, int iValue)
{
	// Find parameter
	if (this->m_Data.Param.find(Id) != this->m_Data.Param.end())
	{
		// Set integer value
		this->m_Data.Param[Id].iValue = iValue;
	}
}

// Set parameter float
void CMatchMessage::SetArgFloat(int Id, float flValue)
{
	// Find parameter
	if (this->m_Data.Param.find(Id) != this->m_Data.Param.end())
	{
		// Set integer value
		this->m_Data.Param[Id].flValue = flValue;
	}
}

// Set parameter string
void CMatchMessage::SetArgString(int Id, const char* szValue)
{
	// Find parameter
	if (this->m_Data.Param.find(Id) != this->m_Data.Param.end())
	{
		// Set string value
		Q_strncpy(this->m_Data.Param[Id].szValue, szValue, sizeof(this->m_Data.Param[Id].szValue));
	}
}