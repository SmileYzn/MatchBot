#pragma once

#define MAX_USER_REG_MESSAGE 256
#define MAX_USER_PARAM_COUNT 20

typedef void (*MESSAGE_EVENT_CALL)(void*);
typedef void (*MESSAGE_EVENT_CALL_END)();

typedef struct S_USER_MESSAGE
{
	std::string				Name;
	int						Index;
	int						Size;
	MESSAGE_EVENT_CALL		Function;
	MESSAGE_EVENT_CALL_END	FunctionEnd;
} P_USER_MESSAGE, * LP_USER_MESSAGE;

class CMatchMessage
{
public:
	void ServerActivate();
	void ServerDeactivate();

	void Reset();

	void RegisterMessage(std::string Name, MESSAGE_EVENT_CALL FunctionPre, MESSAGE_EVENT_CALL_END FunctionPost);
	void* GetParam(int Param);

	META_RES RegUserMsg(const char* pszName, int iSize);
	META_RES MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity);
	META_RES MessageEnd(void);
	META_RES WriteByte(int iValue);
	META_RES WriteChar(int iValue);
	META_RES WriteShort(int iValue);
	META_RES WriteLong(int iValue);
	META_RES WriteAngle(float flValue);
	META_RES WriteCoord(float flValue);
	META_RES WriteString(const char* sz);
	META_RES WriteEntity(int iValue);

private:
	// Registered messages
	std::map<std::string, P_USER_MESSAGE> m_Messages;

	// Registered message event (PRE) callback
	std::map<int, MESSAGE_EVENT_CALL> m_MessageBegin;

	// Registered message event (POST) callback
	std::map<int, MESSAGE_EVENT_CALL_END> m_MessageEnd;

	// Mesage State
	int	m_MsgState = 0;

	// Current message type
	int	m_MsgCurrent = 0;

	// Current message origin
	const float* m_MsgOrigin = nullptr;

	// Current message edict_t
	edict_t* m_MsgEdict = nullptr;

	// Current message parameter
	int m_MsgParamId = 0;

	// Current message data
	std::array<void*, MAX_USER_PARAM_COUNT> m_MsgData = { 0 };

	// Current message callback function begin
	void (*FunctionBegin)(void*) = nullptr;

	// Current message callback function end
	void (*FunctionEnd)() = nullptr;
};

extern CMatchMessage gMatchMessage;