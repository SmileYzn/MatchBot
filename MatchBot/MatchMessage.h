#pragma once

// Max messages
#define MAX_REGISTER_MESSAGE 256

// Max parameter of message
#define MAX_PARAMETER_COUNT 10

// Message types
constexpr auto MESSAGE_TYPE_BYTE = 0;
constexpr auto MESSAGE_TYPE_CHAR = 1;
constexpr auto MESSAGE_TYPE_SHORT = 2;
constexpr auto MESSAGE_TYPE_LONG = 3;
constexpr auto MESSAGE_TYPE_ANGLE = 4;
constexpr auto MESSAGE_TYPE_COORD = 5;
constexpr auto MESSAGE_TYPE_STRING = 6;
constexpr auto MESSAGE_TYPE_ENTITY = 7;

//
struct dfunc_t
{
	void (*function)(void*);
};

//
union message_var_t
{
	float	fValue;
	int		iValue;
	char	sValue[256];
};

//
struct message_t
{
	union message_var_t mValue = { 0 };
	int iType = 0;
};

class CMatchMessage
{
public:
	// Register message hook
	void RegisterHook(char* MessageName, void (*Function)(void*));

	// Begin message hook
	void MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity);

	// End message hook
	void MessageEnd();

	// Write parameter byte
	void WriteByte(int iValue);

	// Write parameter character
	void WriteChar(int iValue);

	// Write parameter short
	void WriteShort(int iValue);

	// Write parameter long
	void WriteLong(int iValue);

	// Write parameter angle
	void WriteAngle(float flValue);

	// Write parameter coordinate
	void WriteCoord(float flValue);

	// Write parameter string
	void WriteString(const char* szValue);

	// Write parameter entity
	void WriteEntity(int iValue);

	// Get parameter as byte
	int GetByte(int ParamIndex);

	// Get parameter as char
	int GetChar(int ParamIndex);

	// Get parameter as short
	int GetShort(int ParamIndex);

	// Get parameter as long
	int GetLong(int ParamIndex);

	// Get parameter as Angle
	float GetAngle(int ParamIndex);

	// Get parameter as coordinate
	float GetCoord(int ParamIndex);

	// Get parameter as string
	char* GetString(int ParamIndex);

	// Get parameter as entity
	int GetEntity(int ParamIndex);

private:
	// Message Function hooks
	dfunc_t MessageHooks[MAX_REGISTER_MESSAGE] = { nullptr };
	
	// Current Message Index
	int CurrentMessageIndex = 0;

	// Current message is active (Is running)
	bool MessageActive = false;

	// Current message data
	message_t CurrentMessage[MAX_PARAMETER_COUNT] = { 0 };

	// Current message type
	int CurrentMessageType = 0;

	// Current message entity
	edict_t* pCurentEntity = nullptr;
};

extern CMatchMessage gMatchMessage;