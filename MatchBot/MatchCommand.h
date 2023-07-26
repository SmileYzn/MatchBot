#pragma once

class CMatchCommand
{
public:
	// On Client Command
	bool ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1);
	// Prefix command comparison
	bool CompareCommand(const char* pcmd, const char* prefix, const char* mbCommand);
};

extern CMatchCommand gMatchCommand;
