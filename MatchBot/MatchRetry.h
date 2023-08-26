#pragma once

class CMatchRetry
{
public:
	bool PlayerConnect(edict_t* pEdict, const char* pszName, const char* pszAddress, char szRejectReason[128]);
	void PlayerDisconnect(edict_t* pEdict, bool crash, const char* Reason);

private:
	std::unordered_map<std::string, float> m_Data;
};

extern CMatchRetry gMatchRetry;