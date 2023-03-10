#include "precompiled.h"

CMatchUtil gMatchUtil;

cvar_t* CMatchUtil::CvarRegister(const char* Name, const char* Value)
{
	// Get Pointer by variable name
	cvar_t* Pointer = CVAR_GET_POINTER(Name);

	// If not exists
	if (!Pointer)
	{
		// Variable Data
		static cvar_t VariableData;

		// Set name
		VariableData.name = Name;

		// Set value
		VariableData.string = (char*)Value;

		// Set flags
		VariableData.flags = (FCVAR_SERVER | FCVAR_EXTDLL | FCVAR_PROTECTED | FCVAR_SPONLY | FCVAR_UNLOGGED);

		// Register the variable
		CVAR_REGISTER(&VariableData);

		// Return created pointer
		return CVAR_GET_POINTER(Name);
	}

	// Return existing pointer to variable
	return Pointer;
}

void CMatchUtil::TeamInfo(edict_t* pEntity, int playerIndex, const char* pszTeamName)
{
	static int iMsgTeamInfo;

	if (iMsgTeamInfo || (iMsgTeamInfo = GET_USER_MSG_ID(PLID, "TeamInfo", NULL)))
	{
		MESSAGE_BEGIN(MSG_ONE, iMsgTeamInfo, nullptr, pEntity);
		WRITE_BYTE(playerIndex);
		WRITE_STRING(pszTeamName);
		MESSAGE_END();
	}
}

void CMatchUtil::SayText(edict_t* pEntity, int Sender, const char* Format, ...)
{
	static int iMsgSayText;

	if (iMsgSayText || (iMsgSayText = GET_USER_MSG_ID(PLID, "SayText", NULL)))
	{
		va_list argList;

		va_start(argList, Format);

		char Buffer[191] = { 0 };

		vsnprintf(Buffer, sizeof(Buffer), Format, argList);

		va_end(argList);

		char SayText[191] = { 0 };

		snprintf(SayText, sizeof(SayText), "\4[%s]\1 %s", gMatchBot.GetTag(), Buffer);

		if (Sender < PRINT_TEAM_BLUE || Sender > gpGlobals->maxClients)
		{
			Sender = PRINT_TEAM_DEFAULT;
		}
		else if (Sender < PRINT_TEAM_DEFAULT)
		{
			Sender = abs(Sender) + MAX_CLIENTS;
		}

		if (!FNullEnt(pEntity))
		{
			if (!(pEntity->v.flags & FL_FAKECLIENT))
			{
				MESSAGE_BEGIN(MSG_ONE, iMsgSayText, nullptr, pEntity);
				WRITE_BYTE(Sender ? Sender : ENTINDEX(pEntity));
				WRITE_STRING("%s");
				WRITE_STRING(SayText);
				MESSAGE_END();
			}
		}
		else
		{
			for (int i = 1; i <= gpGlobals->maxClients; ++i)
			{
				edict_t* pTempEntity = INDEXENT(i);

				if (!FNullEnt(pTempEntity))
				{
					if (!(pTempEntity->v.flags & FL_FAKECLIENT))
					{
						MESSAGE_BEGIN(MSG_ONE, iMsgSayText, nullptr, pTempEntity);
						WRITE_BYTE(Sender ? Sender : i);
						WRITE_STRING("%s");
						WRITE_STRING(SayText);
						MESSAGE_END();
					}
				}
			}
		}
	}
}

void CMatchUtil::ClientPrint(edict_t* pEntity, int msg_dest, const char* Format, ...)
{
	va_list argList;

	va_start(argList, Format);

	char Buffer[188] = { 0 };

	int Length = vsnprintf(Buffer, sizeof(Buffer), Format, argList);

	va_end(argList);

	if (msg_dest == PRINT_CONSOLE)
	{
		if (Length > 125)
		{
			Length = 125;
		}

		Buffer[Length++] = '\n';
		Buffer[Length++] = '\n';
		Buffer[Length] = 0;
	}

	static int iMsgTextMsg;

	if (iMsgTextMsg || (iMsgTextMsg = GET_USER_MSG_ID(PLID, "TextMsg", NULL)))
	{
		if (pEntity)
		{
			MESSAGE_BEGIN(MSG_ONE, iMsgTextMsg, NULL, pEntity);
		}
		else
		{
			MESSAGE_BEGIN(MSG_BROADCAST, iMsgTextMsg);
		}

		WRITE_BYTE(msg_dest);
		WRITE_STRING("%s");
		WRITE_STRING(Buffer);
		MESSAGE_END();
	}
}

int CMatchUtil::GetCount(TeamName Team)
{
	int Result = 0;

	for (int i = 1; i <= gpGlobals->maxClients; ++i)
	{
		auto Player = UTIL_PlayerByIndexSafe(i);

		if (Player)
		{
			if (!Player->IsDormant())
			{
				if (Player->m_iTeam == Team)
				{
					Result++;
				}
			}
		}
	}

	return Result;
}

int CMatchUtil::GetPlayers(CBasePlayer* Players[MAX_CLIENTS], bool InGameOnly)
{
	int Num = 0;

	memset(&Players, 0, MAX_CLIENTS);

	for (int i = 1; i <= gpGlobals->maxClients; ++i)
	{
		auto Player = UTIL_PlayerByIndexSafe(i);

		if (Player)
		{
			if (!Player->IsDormant())
			{
				if (InGameOnly)
				{
					if (Player->m_iTeam == TERRORIST || Player->m_iTeam == CT)
					{
						Players[Num++] = Player;
					}
				}
				else
				{
					Players[Num++] = Player;
				}
			}
		}
	}

	return Num;
}

std::vector<CBasePlayer*> CMatchUtil::GetPlayers(bool InGameOnly, bool ReturnBots)
{
	std::vector<CBasePlayer*> Players;

	for (int i = 1; i <= gpGlobals->maxClients; ++i)
	{
		auto Player = UTIL_PlayerByIndexSafe(i);

		if (Player)
		{
			if (!Player->IsDormant())
			{
				if (InGameOnly)
				{
					if (Player->m_iTeam != TERRORIST && Player->m_iTeam != CT)
					{
						continue;
					}
				}

				if (!ReturnBots && Player->IsBot())
				{
					continue;
				}

				Players.push_back(Player);
			}
		}
	}

	return Players;
}

std::vector<CBasePlayer*> CMatchUtil::GetPlayers(TeamName Team, bool ReturnBots)
{
	std::vector<CBasePlayer*> Players;

	for (int i = 1; i <= gpGlobals->maxClients; ++i)
	{
		auto Player = UTIL_PlayerByIndexSafe(i);

		if (Player)
		{
			if (!Player->IsDormant())
			{
				if (Team != Player->m_iTeam)
				{
					continue;
				}

				if (!ReturnBots && Player->IsBot())
				{
					continue;
				}

				Players.push_back(Player);
			}
		}
	}

	return Players;
}

void CMatchUtil::ServerCommand(const char* Format, ...)
{
	va_list argList;

	va_start(argList, Format);

	char Buffer[255] = { 0 };

	int Length = vsnprintf(Buffer, sizeof(Buffer), Format, argList);

	va_end(argList);

	Buffer[Length++] = '\n';
	Buffer[Length] = 0;

	SERVER_COMMAND(Buffer);
	SERVER_EXECUTE();
}


unsigned short CMatchUtil::FixedUnsigned16(float value, float scale)
{
	int output = value * scale;

	if (output < 0)
	{
		output = 0;
	}

	if (output > USHRT_MAX)
	{
		output = USHRT_MAX;
	}

	return (unsigned short)output;
}

short CMatchUtil::FixedSigned16(float value, float scale)
{
	int output = value * scale;

	if (output > SHRT_MAX)
	{
		output = SHRT_MAX;
	}

	if (output < SHRT_MIN)
	{
		output = SHRT_MIN;
	}

	return (short)output;
}

hudtextparms_t CMatchUtil::HudParam(int red, int green, int blue, float x, float y, int effects, float fxtime, float holdtime, float fadeintime, float fadeouttime, int channel)
{
	hudtextparms_t hud = { 0 };

	hud.r1 = red;
	hud.g1 = green;
	hud.b1 = blue;
	hud.a1 = 255;
	hud.r2 = 255;
	hud.g2 = 255;
	hud.b2 = 255;
	hud.a2 = 255;
	hud.x = x;
	hud.y = y;
	hud.effect = effects;
	hud.fxTime = fxtime;
	hud.holdTime = holdtime;
	hud.fadeinTime = fadeintime;
	hud.fadeoutTime = fadeouttime;
	hud.channel = channel;

	return hud;
}

void CMatchUtil::HudMessage(edict_t* pEntity, hudtextparms_t textparms, const char* Format, ...)
{
	va_list argList;

	va_start(argList, Format);

	char Buffer[511] = { 0 };

	vsnprintf(Buffer, sizeof(Buffer), Format, argList);

	va_end(argList);

	if (pEntity)
	{
		MESSAGE_BEGIN(MSG_ONE_UNRELIABLE, SVC_TEMPENTITY, NULL, pEntity);
	}
	else
	{
		MESSAGE_BEGIN(MSG_BROADCAST, SVC_TEMPENTITY);
	}

	WRITE_BYTE(TE_TEXTMESSAGE);
	WRITE_BYTE(textparms.channel & 0xFF);
	
	WRITE_SHORT(this->FixedSigned16(textparms.x, BIT(13)));
	WRITE_SHORT(this->FixedSigned16(textparms.y, BIT(13)));

	WRITE_BYTE(textparms.effect);

	WRITE_BYTE(textparms.r1);
	WRITE_BYTE(textparms.g1);
	WRITE_BYTE(textparms.b1);
	WRITE_BYTE(textparms.a1);

	WRITE_BYTE(textparms.r2);
	WRITE_BYTE(textparms.g2);
	WRITE_BYTE(textparms.b2);
	WRITE_BYTE(textparms.a2);

	WRITE_SHORT(this->FixedUnsigned16(textparms.fadeinTime, BIT(8)));
	WRITE_SHORT(this->FixedUnsigned16(textparms.fadeoutTime, BIT(8)));
	WRITE_SHORT(this->FixedUnsigned16(textparms.holdTime, BIT(8)));

	if (textparms.effect == 2)
	{
		WRITE_SHORT(this->FixedUnsigned16(textparms.fxTime, BIT(8)));
	}

	WRITE_STRING(Buffer);
	MESSAGE_END();
}
/*
void CMatchUtil::ClientCommand(edict_t* pEntity, const char* Format, ...)
{
	if (pEntity)
	{
		va_list argList;

		va_start(argList, Format);

		char Buffer[255] = { 0 };

		int Length = vsnprintf(Buffer, sizeof(Buffer), Format, argList);

		va_end(argList);

		if (Length > 254)
		{
			Length = 254;
		}

		Buffer[Length++] = '\n';
		Buffer[Length] = 0;

		CLIENT_COMMAND(pEntity, Buffer);
	}
}*/

const char* CMatchUtil::FormatString(const char* Format, ...)
{
	va_list argList;

	static char VarArgs[256];

	va_start(argList, Format);

	vsnprintf(VarArgs, sizeof(VarArgs), Format, argList);

	va_end(argList);

	return VarArgs;
}

std::map<int, std::string> CMatchUtil::GetMapList(bool CurrentMap)
{
	std::map<int, std::string> MapList;

	std::ifstream fp;

	fp.open(MAP_LIST_FILE, std::ifstream::in);

	if (fp.is_open())
	{
		std::string Map = "";

		int MapIndex = 0;

		while(std::getline(fp, Map))
		{
			if (IS_MAP_VALID(Map.data()))
			{
				if (!CurrentMap)
				{
					if (Map.compare(STRING(gpGlobals->mapname)) == 0)
					{
						continue;
					}
				}

				MapList[MapIndex] = Map;

				MapIndex++;
			}
		}

		fp.close();
	}

	return MapList;
}

void CMatchUtil::ShowMotd(edict_t* pEntity, char* Motd, int MotdLength)
{
	static int iMsgMOTD;

	if (iMsgMOTD || (iMsgMOTD = GET_USER_MSG_ID(PLID, "MOTD", NULL)))
	{
		if (MotdLength < 128)
		{
			struct stat FileBuffer;

			if (stat(Motd, &FileBuffer) == 0)
			{
				int FileLength = 0;

				char* FileContent = reinterpret_cast<char*>(LOAD_FILE_FOR_ME(Motd, &FileLength));

				if (FileLength)
				{
					this->ShowMotd(pEntity, FileContent, FileLength);
				}

				FREE_FILE(FileContent);
			}

			return;
		}

		char* Buffer = Motd;

		char Character = 0;

		int Size = 0;

		while (*Buffer)
		{
			Size = MotdLength;

			if (Size > 175)
			{
				Size = 175;
			}

			MotdLength -= Size;

			Character = *(Buffer += Size);

			*Buffer = 0;

			MESSAGE_BEGIN(MSG_ONE, iMsgMOTD, NULL, pEntity);
			WRITE_BYTE(Character ? FALSE : TRUE);
			WRITE_STRING(Motd);
			MESSAGE_END();

			*Buffer = Character;

			Motd = Buffer;
		}
	}
}
