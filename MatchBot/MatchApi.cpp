#include "precompiled.h"

CMatchApi gMatchApi;

bool CMatchApi::ClientConnected(IGameClient* Client)
{
	if (gMatchBot.m_ApiUrl->string)
	{
		if (gMatchBot.m_ApiUrl->string[0] != '\0')
		{
			if (gMatchBot.m_ApiAuthType->value > 0.0f)
			{
				if (Client)
				{
					auto pEdict = Client->GetEdict();

					if (pEdict)
					{
						auto UserID = Client->GetNetworkUserID();

						if (UserID)
						{
							nlohmann::ordered_json PostData;

							PostData["PlayerConnectAuthenticate"] =
							{
								{"IdType", UserID->idtype},
								{"SteamID", UserID->m_SteamID},
								{"ClientIp", UserID->clientip},
							};

							if (PostData.size())
							{
								gMatchCurl.PostJSON(gMatchBot.m_ApiUrl->string, (long)gMatchBot.m_ApiTimeout->value, PostData.dump(), (void*)this->ClientConnectCallback, ENTINDEX(pEdict));
							}

							return true;
						}

						if (gMatchBot.m_ApiAuthType->value > 1.0f)
						{
							gMatchUtil.DropClient(ENTINDEX(pEdict), _T("Failed to authenticate, contact us: %s"), g_engfuncs.pfnCVarGetString("sv_contact"));
						}
					}
				}
			}
		}
	}

	return false;
}

void CMatchApi::ClientConnectCallback(CURL* ch, size_t Size, const char* Memory, int EntityIndex)
{
	if (EntityIndex)
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
									gMatchUtil.DropClient(EntityIndex, _T("Failed to authenticate, contact us: %s"), g_engfuncs.pfnCVarGetString("sv_contact"));
								}
							}

							return;
						}
					}
				}
			}
		}
	}

	if (gMatchBot.m_ApiAuthType->value > 1.0f)
	{
		gMatchUtil.DropClient(EntityIndex, _T("Failed to authenticate, contact us: %s"), g_engfuncs.pfnCVarGetString("sv_contact"));
	}
}
