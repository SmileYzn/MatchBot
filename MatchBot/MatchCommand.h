#pragma once

class CMatchCommand
{
public:
	// On Client Command
	bool ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1);
};

extern CMatchCommand gMatchCommand;
