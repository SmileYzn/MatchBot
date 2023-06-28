#pragma once

class CMatchApi
{
public:
	bool ClientConnected(IGameClient* Client);
	static void ClientConnectCallback(CURL* ch, size_t Size, const char* Memory, int EntityIndex);
};

extern CMatchApi gMatchApi;