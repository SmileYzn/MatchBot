#pragma once

constexpr auto MB_REQUEST_API_TIMEOUT = 10;
constexpr auto MB_REQUEST_API_URL = "https://api.pugbr.net/auth/%lld";

class CMatchRequestApi
{
public:
	bool ClientConnected(IGameClient* Client);
	static void RequestCallback(CURL* ch, size_t Size, const char* Memory, int EntityIndex);
};

extern CMatchRequestApi gMatchRequestApi;