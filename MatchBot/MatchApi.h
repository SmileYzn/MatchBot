#pragma once

constexpr auto MB_REQUEST_API_TIMEOUT = 10;
constexpr auto MB_REQUEST_API_URL = "https://api.pugbr.net/auth/%lld";

class CMatchApi
{
public:
	bool ClientConnected(IGameClient* Client);
	static void ClientConnectCallback(CURL* ch, size_t Size, const char* Memory, int EntityIndex);
};

extern CMatchApi gMatchApi;