#pragma once

// Message types
constexpr auto MESSAGE_TYPE_BYTE = 0;
constexpr auto MESSAGE_TYPE_CHAR = 1;
constexpr auto MESSAGE_TYPE_SHORT = 2;
constexpr auto MESSAGE_TYPE_LONG = 3;
constexpr auto MESSAGE_TYPE_ANGLE = 4;
constexpr auto MESSAGE_TYPE_COORD = 5;
constexpr auto MESSAGE_TYPE_STRING = 6;
constexpr auto MESSAGE_TYPE_ENTITY = 7;

// MessageBegin function header
struct MESSAGE_BEGIN_FUNC
{
	bool (*Function)(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity);
};

// Message parameters
typedef struct MESSAGE_PARAM_S
{
	int Type;
	int iValue;
	float flValue;
	char szValue[200];
} P_MESSAGE_PARAM, * LP_MESSAGE_PARAM;

// Message data
typedef struct MESSAGE_DATA_S
{
	int msg_dest;
	int msg_type;
	const float* pOrigin;
	edict_t* pEntity;
	std::unordered_map<int, P_MESSAGE_PARAM> Param;
} P_MESSAGE_DATA, * LP_MESSAGE_DATA;

class CMatchMessage
{
public:
	// Register hook message
	void RegisterHook(const char* MsgName, bool (*Function)(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity));

	// Begin message hook
	bool MessageBegin(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity);

	// End message hook
	bool MessageEnd();

	// Write parameter byte
	bool WriteByte(int iValue);

	// Write parameter character
	bool WriteChar(int iValue);

	// Write parameter short
	bool WriteShort(int iValue);

	// Write parameter long
	bool WriteLong(int iValue);

	// Write parameter angle
	bool WriteAngle(float flValue);

	// Write parameter coordinate
	bool WriteCoord(float flValue);

	// Write parameter string
	bool WriteString(const char* szValue);

	// Write parameter entity index
	bool WriteEntity(int iValue);

	// Get parameter byte
	int GetByte(int Id);

	// Get parameter character
	int GetChar(int Id);

	// Get parameter short
	int GetShort(int Id);

	// Get parameter long
	int GetLong(int Id);

	// Get parameter angle
	float GetAngle(int Id);

	// Get parameter coordinate
	float GetCoord(int Id);

	// Get parameter string
	const char* GetString(int Id);

	// Get parameter entity index
	int GetEntity(int Id);

	// Set parameter integer
	void SetArgInt(int Id, int iValue);

	// Set parameter float
	void SetArgFloat(int Id, float flValue);

	// Set parameter string
	void SetArgString(int Id, const char* szValue);


private:
	// Message is hooked
	int m_MsgId = 0;

	// Message Data
	P_MESSAGE_DATA m_Data = { 0 };

	// Message Hooks
	std::unordered_map<int, MESSAGE_BEGIN_FUNC> m_Hook;
};

extern CMatchMessage gMatchMessage;