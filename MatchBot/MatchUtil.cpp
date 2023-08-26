#include "precompiled.h"

CMatchUtil gMatchUtil;

// Make Directory
int CMatchUtil::MakeDirectory(const char* Path)
{
	struct stat st = { 0 };

	if (stat(Path, &st) == -1)
	{
#if defined(_WIN32)
		return _mkdir(Path);
#else
		return mkdir(Path, 0755);
#endif
	}

	return 0;
}

// Register console variable
cvar_t* CMatchUtil::CvarRegister(const char* Name, const char* Value)
{
	return this->CvarRegister(Name, Value, (FCVAR_SERVER | FCVAR_SPONLY));
}

// Register console variable
cvar_t* CMatchUtil::CvarRegister(const char* Name, const char* Value, int Flags)
{
	// Get cvar pointer
	cvar_t* Pointer = g_engfuncs.pfnCVarGetPointer(Name);

	// If not exists
	if (!Pointer)
	{
		// Variable for Cvar Helper
		this->m_CvarData[Name].name = Name;

		// Set name
		this->m_CvarData[Name].string = (char*)(Value);
		
		// Set flags
		this->m_CvarData[Name].flags = Flags ? Flags : (FCVAR_SERVER | FCVAR_SPONLY);

		// Register the variable
		g_engfuncs.pfnCVarRegister(&this->m_CvarData[Name]);

		// Get created pointer
		Pointer = g_engfuncs.pfnCVarGetPointer(this->m_CvarData[Name].name);

		// If is not null
		if(Pointer)
		{
			// We can set values
			g_engfuncs.pfnCvar_DirectSet(Pointer, Value);
		}
	}

	return Pointer;
}

void CMatchUtil::TeamInfo(edict_t* pEntity, int playerIndex, const char* pszTeamName)
{
	static int iMsgTeamInfo;

	if (iMsgTeamInfo || (iMsgTeamInfo = gpMetaUtilFuncs->pfnGetUserMsgID(PLID, "TeamInfo", NULL)))
	{
		g_engfuncs.pfnMessageBegin(MSG_ONE, iMsgTeamInfo, nullptr, pEntity);
		g_engfuncs.pfnWriteByte(playerIndex);
		g_engfuncs.pfnWriteString(pszTeamName);
		g_engfuncs.pfnMessageEnd();
	}
}

void CMatchUtil::MakeDeathMessage(edict_t* pKiller, edict_t* pVictim, bool Headshot, const char* Weapon)
{
	static int iMsgDeathMsg;

	if (iMsgDeathMsg || (iMsgDeathMsg = gpMetaUtilFuncs->pfnGetUserMsgID(PLID, "DeathMsg", NULL)))
	{
		g_engfuncs.pfnMessageBegin(MSG_ALL, iMsgDeathMsg, NULL, NULL);

		g_engfuncs.pfnWriteByte(!FNullEnt(pKiller) ? ENTINDEX(pKiller) : 0);

		g_engfuncs.pfnWriteByte(!FNullEnt(pVictim) ? ENTINDEX(pVictim) : 0);

		g_engfuncs.pfnWriteByte(Headshot ? 1 : 0);

		g_engfuncs.pfnWriteString(Weapon);

		g_engfuncs.pfnMessageEnd();
	}
}

void CMatchUtil::SayText(edict_t* pEntity, int Sender, const char* Format, ...)
{
	static int iMsgSayText;

	if (iMsgSayText || (iMsgSayText = gpMetaUtilFuncs->pfnGetUserMsgID(PLID, "SayText", NULL)))
	{
		char Buffer[191] = { 0 };

		va_list ArgList;
		va_start(ArgList, Format);
		Q_vsnprintf(Buffer, sizeof(Buffer), Format, ArgList);
		va_end(ArgList);

		if (gMatchBot.m_MatchTag)
		{
			if (gMatchBot.m_MatchTag->string)
			{
				char Temp[191] = { 0 };

				Q_snprintf(Temp, sizeof(Temp), "^4[%s]^1 %s", gMatchBot.m_MatchTag->string, Buffer);

				Q_strcpy_s(Buffer, Temp);
			}
		}

		if (Sender < PRINT_TEAM_BLUE || Sender > gpGlobals->maxClients)
		{
			Sender = PRINT_TEAM_DEFAULT;
		}
		else if (Sender < PRINT_TEAM_DEFAULT)
		{
			Sender = abs(Sender) + MAX_CLIENTS;
		}

		this->ParseLinesAndColors(Buffer);

		if (!FNullEnt(pEntity))
		{
			if (!(pEntity->v.flags & FL_FAKECLIENT))
			{
				g_engfuncs.pfnMessageBegin(MSG_ONE, iMsgSayText, nullptr, pEntity);
				g_engfuncs.pfnWriteByte(Sender ? Sender : ENTINDEX(pEntity));
				g_engfuncs.pfnWriteString("%s");
				g_engfuncs.pfnWriteString(Buffer);
				g_engfuncs.pfnMessageEnd();
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
						g_engfuncs.pfnMessageBegin(MSG_ONE, iMsgSayText, nullptr, pTempEntity);
						g_engfuncs.pfnWriteByte(Sender ? Sender : i);
						g_engfuncs.pfnWriteString("%s");
						g_engfuncs.pfnWriteString(Buffer);
						g_engfuncs.pfnMessageEnd();
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

	if (iMsgTextMsg || (iMsgTextMsg = gpMetaUtilFuncs->pfnGetUserMsgID(PLID, "TextMsg", NULL)))
	{
		if (pEntity)
		{
			g_engfuncs.pfnMessageBegin(MSG_ONE, iMsgTextMsg, NULL, pEntity);
		}
		else
		{
			g_engfuncs.pfnMessageBegin(MSG_BROADCAST, iMsgTextMsg, NULL, NULL);
		}

		g_engfuncs.pfnWriteByte(msg_dest);
		g_engfuncs.pfnWriteString("%s");
		g_engfuncs.pfnWriteString(Buffer);
		g_engfuncs.pfnMessageEnd();
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

std::array<int, SPECTATOR + 1> CMatchUtil::GetCount()
{
	std::array<int, SPECTATOR + 1> TeamCount = { 0 };

	for (int i = 1; i <= gpGlobals->maxClients; ++i)
	{
		auto Player = UTIL_PlayerByIndexSafe(i);

		if (Player)
		{
			if (!Player->IsDormant())
			{
				TeamCount[Player->m_iTeam]++;
			}
		}
	}

	return TeamCount;
}

int CMatchUtil::GetPlayers(CBasePlayer* Players[MAX_CLIENTS], bool InGameOnly)
{
	int Num = 0;

	Q_memset(Players, 0, MAX_CLIENTS);

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

// Get player by user index
CBasePlayer* CMatchUtil::GetPlayerByUserId(int UserIndex)
{
	for (int i = 1; i <= gpGlobals->maxClients; ++i)
	{
		auto Player = UTIL_PlayerByIndexSafe(i);

		if (Player)
		{
			if (!Player->IsDormant())
			{
				if (UserIndex == g_engfuncs.pfnGetPlayerUserId(Player->edict()))
				{
					return Player;
				}
			}
		}
	}

	return nullptr;
}

void CMatchUtil::ServerCommand(const char* Format, ...)
{
	char cmd[255] = { 0 };

	va_list	argptr;

	va_start(argptr, Format);

	vsnprintf(cmd, sizeof(cmd), Format, argptr);

	va_end(argptr);

	Q_strncat(cmd, "\n", 1);

	g_engfuncs.pfnServerCommand(cmd);
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
	hudtextparms_t hud ;

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

	char Buffer[511];

	vsnprintf(Buffer, sizeof(Buffer), Format, argList);

	va_end(argList);

	this->ParseLinesAndColors(Buffer);

	if (pEntity)
	{
		g_engfuncs.pfnMessageBegin(MSG_ONE_UNRELIABLE, SVC_TEMPENTITY, NULL, pEntity);
	}
	else
	{
		g_engfuncs.pfnMessageBegin(MSG_BROADCAST, SVC_TEMPENTITY, NULL, NULL);
	}

	g_engfuncs.pfnWriteByte(TE_TEXTMESSAGE);
	g_engfuncs.pfnWriteByte(textparms.channel & 0xFF);
	
	g_engfuncs.pfnWriteShort(this->FixedSigned16(textparms.x, BIT(13)));
	g_engfuncs.pfnWriteShort(this->FixedSigned16(textparms.y, BIT(13)));

	g_engfuncs.pfnWriteByte(textparms.effect);

	g_engfuncs.pfnWriteByte(textparms.r1);
	g_engfuncs.pfnWriteByte(textparms.g1);
	g_engfuncs.pfnWriteByte(textparms.b1);
	g_engfuncs.pfnWriteByte(textparms.a1);

	g_engfuncs.pfnWriteByte(textparms.r2);
	g_engfuncs.pfnWriteByte(textparms.g2);
	g_engfuncs.pfnWriteByte(textparms.b2);
	g_engfuncs.pfnWriteByte(textparms.a2);

	g_engfuncs.pfnWriteShort(this->FixedUnsigned16(textparms.fadeinTime, BIT(8)));
	g_engfuncs.pfnWriteShort(this->FixedUnsigned16(textparms.fadeoutTime, BIT(8)));
	g_engfuncs.pfnWriteShort(this->FixedUnsigned16(textparms.holdTime, BIT(8)));

	if (textparms.effect == 2)
	{
		g_engfuncs.pfnWriteShort(this->FixedUnsigned16(textparms.fxTime, BIT(8)));
	}

	g_engfuncs.pfnWriteString(Buffer);
	g_engfuncs.pfnMessageEnd();
}

const char* CMatchUtil::FormatString(const char* Format, ...)
{
	va_list argList;

	static char VarArgs[512];

	va_start(argList, Format);

	vsnprintf(VarArgs, sizeof(VarArgs), Format, argList);

	va_end(argList);

	return VarArgs;
}

std::map<int, std::string> CMatchUtil::GetMapList(bool CurrentMap)
{
	// Map List container
	std::map<int, std::string> MapList;

	// Map Index
	auto MapIndex = 0;

	// Memory Script instance
	CMemScript* lpMemScript = new CMemScript;

	// If is not null
	if (lpMemScript)
	{
		// Try to load file
		if (lpMemScript->SetBuffer(MB_MAP_LIST_FILE))
		{
			try
			{
				// Loop lines
				while (true)
				{
					// If file content ended
					if (lpMemScript->GetToken() == eTokenResult::TOKEN_END)
					{
						// Break loop
						break;
					}

					// Get Map name as string
					auto Map = lpMemScript->GetString();

					// If is not empty
					if (!Map.empty())
					{
						// If has to skip corrent map
						if (!CurrentMap)
						{
							// Compare with current map
							if (Map.compare(STRING(gpGlobals->mapname)) == 0)
							{
								// Skip map
								continue;
							}
						}

						// Copy to map name
						auto MapName = Q_strdup(Map.c_str());

						// If is not null
						if (MapName)
						{
							// If is not empty
							if (MapName[0u] != '\0')
							{
								// Check if is valid map name
								if (g_engfuncs.pfnIsMapValid(MapName))
								{
									// Insert into map list container
									MapList[MapIndex++] = Map;
								}
							}
						}
					}
				}
			}
			catch (...)
			{
				// Catch for erros
				LOG_CONSOLE(PLID, "[%s] %s", __func__, lpMemScript->GetError().c_str());
			}
		}

		// Delete Memory Script instance
		delete lpMemScript;
	}

	return MapList;
}

void CMatchUtil::ShowMotd(edict_t* pEntity, char* Motd, int MotdLength)
{
	static int iMsgMOTD;

	if (iMsgMOTD || (iMsgMOTD = gpMetaUtilFuncs->pfnGetUserMsgID(PLID, "MOTD", NULL)))
	{
		if (MotdLength < 128)
		{
			struct stat FileBuffer;

			if (stat(Motd, &FileBuffer) == 0)
			{
				int FileLength = 0;

				char* FileContent = reinterpret_cast<char*>(g_engfuncs.pfnLoadFileForMe(Motd, &FileLength));

				if (FileLength)
				{
					this->ShowMotd(pEntity, FileContent, FileLength);
				}

				g_engfuncs.pfnFreeFile(FileContent);

				return;
			}
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

			g_engfuncs.pfnMessageBegin(MSG_ONE, iMsgMOTD, NULL, pEntity);
			g_engfuncs.pfnWriteByte(Character ? FALSE : TRUE);
			g_engfuncs.pfnWriteString(Motd);
			g_engfuncs.pfnMessageEnd();

			*Buffer = Character;

			Motd = Buffer;
		}
	}
}

// Replace all in string
void CMatchUtil::ReplaceAll(std::string& String, const std::string& From, const std::string& To)
{
	if (!From.empty())
	{
		// Starting position
		size_t StartPos = 0;

		// While string is found on that position
		while ((StartPos = String.find(From, StartPos)) != std::string::npos)
		{
			// Replace with string
			String.replace(StartPos, From.length(), To);

			// Increment starting position
			StartPos += To.length();
		}
	}
}

void CMatchUtil::DropClient(int EntityIndex, const char* Format, ...)
{
	auto Gameclient = g_RehldsSvs->GetClient(EntityIndex - 1);

	if (Gameclient)
	{
		va_list argList;

		va_start(argList, Format);

		char Buffer[255] = { 0 };

		vsnprintf(Buffer, sizeof(Buffer), Format, argList);

		va_end(argList);

		if (g_RehldsFuncs)
		{
			g_RehldsFuncs->DropClient(Gameclient, false, "%s", Buffer);
		}
		else
		{
			auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

			if (Player)
			{
				int UserIndex = g_engfuncs.pfnGetPlayerUserId(Player->edict());

				if (!FNullEnt(Player->edict()) && UserIndex > 0)
				{
					if (strlen(Buffer) > 0)
					{
						this->ServerCommand("kick #%d %s", UserIndex, Buffer);
					}
					else
					{
						this->ServerCommand("kick #%d", UserIndex);
					}
				}

			}
		}
	}
}

int CMatchUtil::ParseLinesAndColors(char* Buffer)
{
	size_t len = strlen(Buffer);

	int offs = 0;

	if (len > 0)
	{
		int c;

		for (size_t i = 0; i < len; i++)
		{
			c = Buffer[i];

			if (c == '^' && (i != len - 1))
			{
				c = Buffer[++i];

				if (c == 'n' || c == 't' || (c >= '1' && c <= '4'))
				{
					switch (c)
					{
						case '1': c = '\x01'; break;
						case '2': c = '\x02'; break;
						case '3': c = '\x03'; break;
						case '4': c = '\x04'; break;
						case 'n': c = '\n'; break;
						case 't': c = '\t'; break;
					}

					offs++;
				}
			}

			Buffer[i - offs] = c;
		}

		Buffer[len - offs] = '\0';
	}

	return offs;
}

CBasePlayer* CMatchUtil::FindPlayer(std::string Target)
{
	if (!Target.empty())
	{
		if (Target.length() > 1)
		{
			if (Target[0u] == '#')
			{
				if (!Target.substr(1).empty())
				{
					auto Find = Q_atoi(Target.substr(1).c_str());

					if (Find > 0)
					{
						for (auto i = 1; i <= gpGlobals->maxClients; i++)
						{
							auto Player = UTIL_PlayerByIndexSafe(i);

							if (Player)
							{
								if (!Player->IsDormant())
								{
									if (g_engfuncs.pfnGetPlayerUserId(Player->edict()) == Find)
									{
										return Player;
									}
								}
							}
						}
					}
				}
			}
			else
			{
				std::transform(Target.begin(), Target.end(), Target.begin(), [](unsigned char character)
				{
					return std::tolower(character);
				});

				for (auto i = 1; i <= gpGlobals->maxClients; i++)
				{
					auto Player = UTIL_PlayerByIndexSafe(i);

					if (Player)
					{
						if (!Player->IsDormant())
						{
							std::string Name = STRING(Player->edict()->v.netname);

							if (!Name.empty())
							{
								std::transform(Name.begin(), Name.end(), Name.begin(), [](unsigned char character)
								{
									return std::tolower(character);
								});

								if (Name.find(Target) != std::string::npos)
								{
									return Player;
								}
							}
						}
					}
				}
			}
		}
	}

	return nullptr;
}