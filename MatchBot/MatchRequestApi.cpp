#include "precompiled.h"

CMatchRequestApi gMatchRequestApi;

bool CMatchRequestApi::ClientConnected(IGameClient* Client)
{
	if (gMatchBot.m_ApiUrl->string)
	{
		if (gMatchBot.m_ApiUrl->string[0] != '\0')
		{
			if (Client)
			{
				auto pEdict = Client->GetEdict();

				if (pEdict)
				{
					auto UserID = Client->GetNetworkUserID();

					if (UserID)
					{
						if (UserID->idtype == AUTH_IDTYPE_STEAM)
						{
							if (UserID->m_SteamID)
							{
								// "https://api.pugbr.net/auth/%lld"
								//
								gMatchCurl.Get(gMatchUtil.FormatString(gMatchBot.m_ApiUrl->string, UserID->m_SteamID), (int)gMatchBot.m_ApiTimeout->value, (void*)this->RequestCallback, ENTINDEX(pEdict));
							}
						}

						return true;
					}

					gMatchUtil.DropClient(ENTINDEX(pEdict), "Failed to connect, contact: %s", g_engfuncs.pfnCVarGetString("sv_contact"));
				}
			}
		}
	}

	return false;
}

void CMatchRequestApi::RequestCallback(CURL* ch, size_t Size, const char* Memory, int EntityIndex)
{
	auto Player = UTIL_PlayerByIndexSafe(EntityIndex);

	if (Player)
	{
		if (ch)
		{
			long HttpResponseCode = 0;

			if (curl_easy_getinfo(ch, CURLINFO_RESPONSE_CODE, &HttpResponseCode) == CURLE_OK)
			{
				if (HttpResponseCode == 200)
				{
					if (Memory)
					{
						auto Result = nlohmann::json::parse(Memory);

						if (Result.contains("kick"))
						{
							auto Kick = Result["kick"].get<bool>();

							if (Kick)
							{
								if (Result.contains("reason"))
								{
									auto Reason = Result["reason"].get<std::string>();

									gMatchUtil.DropClient(EntityIndex, "%s", Reason.c_str());
								}
								else
								{
									gMatchUtil.DropClient(EntityIndex, "Failed to connect, contact: %s", g_engfuncs.pfnCVarGetString("sv_contact"));
								}
							}

							return;
						}
					}
				}
			}
		}
	}

	gMatchUtil.DropClient(EntityIndex, "Failed to connect, contact: %s", g_engfuncs.pfnCVarGetString("sv_contact"));
}
