#pragma once

class CMatchCommand
{
public:
	bool ClientCommand(CBasePlayer* Player, const char* pcmd, const char* parg1);
};

extern CMatchCommand gMatchCommand;
