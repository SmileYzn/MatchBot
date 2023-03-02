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

class CMatchMessage
{
public:
	//
	void RegisterHook(char* MessageName, void (*Function)(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity));

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

	// Write parameter entity index
	void WriteEntity(int iValue);

private:
};

extern CMatchMessage gMatchMessage;