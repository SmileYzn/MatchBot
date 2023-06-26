#pragma once

struct P_CURL_MOD_MEMORY
{
	char* Memory;
	size_t Size;
	void* Callback;
	int CallbackData;
};

class CMatchCurl
{
public:
	void ServerActivate();
	void ServerDeactivate();

	void ServerFrame();

	void Get(const char* url, long Timeout, void* FunctionCallback, int CallbackData);
	void PostJSON(const char* url, long Timeout, std::string PostData, void* FunctionCallback, int CallbackData);

	static size_t WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp);

private:
	CURLM* m_MultiHandle = NULL;

	long m_RequestIndex = 0;

	std::map<int, P_CURL_MOD_MEMORY> m_Data;
};

extern CMatchCurl gMatchCurl;
