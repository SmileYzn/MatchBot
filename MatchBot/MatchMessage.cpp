#include "precompiled.h"

CMatchMessage gMatchMessage;

// On Server Activate
void CMatchMessage::ServerActivate()
{
	/**/
}

// On Server Deactivate
void CMatchMessage::ServerDeactivate()
{
	// Clear messages
	this->Reset();
}

// Reset message poll
void CMatchMessage::Reset()
{
	// Clear message poll
	this->m_Messages.clear();

	// Clear Message PRE poll
	this->m_MessageBegin.clear();

	// Clear Message POST poll
	this->m_MessageEnd.clear();

	// Clear message states
	this->m_MsgState = 0;

	// Clear current message index
	this->m_MsgCurrent = 0;

	// Clear current message origin
	this->m_MsgOrigin = nullptr;

	// Clear current message edict_t
	this->m_MsgEdict = nullptr;

	// Clear current message param index
	this->m_MsgParamId = 0;

	// Clear current message parameters
	this->m_MsgData = { 0 };
}

void CMatchMessage::RegisterMessage(std::string Name, MESSAGE_EVENT_CALL FunctionPre, MESSAGE_EVENT_CALL_END FunctionPost)
{
	// If name is not empty
	if (!Name.empty())
	{
		// If has functions to register (PRE) AND (POST)
		if (FunctionPre != nullptr || FunctionPost != nullptr)
		{
			// Find Message data
			auto Msg = this->m_Messages.find(Name);
			//
			// If found
			if (Msg != this->m_Messages.end())
			{
				// Get message index
				auto MsgId = Msg->second.Index;
				//
				// Update message data
				this->m_Messages[Name] =
				{
					Name,
					Msg->second.Index,
					Msg->second.Size,
					FunctionPre,
					FunctionPost
				};
				//
				// Message (PRE) register
				this->m_MessageBegin[MsgId] = FunctionPre;
				//
				// Message (POST) register
				this->m_MessageEnd[MsgId] = FunctionPost;
			}
		}
	}
}

void* CMatchMessage::GetParam(int Param)
{
	for (auto This = 0; This <= this->m_MsgCurrent; This++)
	{
		if (this->m_MsgData[This])
		{
			if (Param == This)
			{
				return this->m_MsgData[This];
			}
		}
	}

	return nullptr;
}

// Register user message
META_RES CMatchMessage::RegUserMsg(const char* pszName, int iSize)
{
	if (pszName)
	{
		this->m_Messages[pszName] = { pszName, META_RESULT_ORIG_RET(int), iSize, nullptr, nullptr };
	}

	return MRES_IGNORED;
}

META_RES CMatchMessage::MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity)
{
	this->m_MsgOrigin = pOrigin;

	this->m_MsgEdict = pEntity;

	this->m_MsgState = 0;

	this->m_MsgCurrent = (msg_type <= 0 || msg_type >= MAX_USER_REG_MESSAGE) ? 0 : msg_type;

	this->m_MsgParamId = 0;

	this->m_MsgData = { nullptr };

	this->FunctionBegin = this->m_MessageBegin[this->m_MsgCurrent];

	this->FunctionEnd = this->m_MessageEnd[this->m_MsgCurrent];

	return MRES_IGNORED;
}

META_RES CMatchMessage::MessageEnd(void)
{
	if (this->FunctionEnd)
	{
		(*this->FunctionEnd)();
	}

	return MRES_IGNORED;
}

META_RES CMatchMessage::WriteByte(int iValue)
{
	if (this->FunctionBegin)
	{
		(*this->FunctionBegin)((void*)&iValue);
	}

	this->m_MsgData[this->m_MsgParamId++] = (void*)iValue;

	return MRES_IGNORED;
}

META_RES CMatchMessage::WriteChar(int iValue)
{
	if (this->FunctionBegin)
	{
		(*this->FunctionBegin)((void*)&iValue);
	}

	this->m_MsgData[this->m_MsgParamId++] = (void*)iValue;

	return MRES_IGNORED;
}

META_RES CMatchMessage::WriteShort(int iValue)
{
	if (this->FunctionBegin)
	{
		(*this->FunctionBegin)((void*)&iValue);
	}

	this->m_MsgData[this->m_MsgParamId++] = (void*)iValue;

	return MRES_IGNORED;
}

META_RES CMatchMessage::WriteLong(int iValue)
{
	if (this->FunctionBegin)
	{
		(*this->FunctionBegin)((void*)&iValue);
	}

	this->m_MsgData[this->m_MsgParamId++] = (void*)iValue;

	return MRES_IGNORED;
}

META_RES CMatchMessage::WriteAngle(float flValue)
{
	if (this->FunctionBegin)
	{
		(*this->FunctionBegin)((void*)&flValue);
	}

	this->m_MsgData[this->m_MsgParamId++] = (void*)&flValue;

	return MRES_IGNORED;
}

META_RES CMatchMessage::WriteCoord(float flValue)
{
	if (this->FunctionBegin)
	{
		(*this->FunctionBegin)((void*)&flValue);
	}

	this->m_MsgData[this->m_MsgParamId++] = (void*)&flValue;

	return MRES_IGNORED;
}

META_RES CMatchMessage::WriteString(const char* sz)
{
	if (this->FunctionBegin)
	{
		(*this->FunctionBegin)((void*)sz);
	}

	this->m_MsgData[this->m_MsgParamId++] = (void*)sz;

	return MRES_IGNORED;
}

META_RES CMatchMessage::WriteEntity(int iValue)
{
	if (this->FunctionBegin)
	{
		(*this->FunctionBegin)((void*)&iValue);
	}

	this->m_MsgData[this->m_MsgParamId++] = (void*)iValue;

	return MRES_IGNORED;
}
