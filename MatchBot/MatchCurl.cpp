#include "precompiled.h"

CMatchCurl gMatchCurl;

void CMatchCurl::ServerActivate()
{
	this->m_MultiHandle = NULL;

	this->m_RequestIndex = 0;

	this->m_Data.clear();

	curl_global_init(CURL_GLOBAL_ALL);

	this->m_MultiHandle = curl_multi_init();
}

void CMatchCurl::ServerDeactivate()
{
	curl_multi_cleanup(this->m_MultiHandle);

	curl_global_cleanup();
}

void CMatchCurl::ServerFrame()
{
	if (this->m_MultiHandle)
	{
		int HandleCount = 0;

		CURLMsg* MsgInfo = NULL;

		do
		{
			curl_multi_perform(this->m_MultiHandle, &HandleCount);

			while (MsgInfo = curl_multi_info_read(this->m_MultiHandle, &HandleCount))
			{
				if (MsgInfo->msg == CURLMSG_DONE)
				{
					int Index = 0;

					curl_easy_getinfo(MsgInfo->easy_handle, CURLINFO_PRIVATE, &Index);

					if (this->m_Data.find(Index) != this->m_Data.end())
					{
						if (this->m_Data[Index].Callback)
						{
							((void(*)(CURL*, size_t, const char*, int))this->m_Data[Index].Callback)(MsgInfo->easy_handle, this->m_Data[Index].Size, this->m_Data[Index].Memory, this->m_Data[Index].CallbackData);
						}

						this->m_Data.erase(Index);
					}

					curl_multi_remove_handle(this->m_MultiHandle, MsgInfo->easy_handle);

					curl_easy_cleanup(MsgInfo->easy_handle);
				}
			}
		}
		while (HandleCount);
	}
}

void CMatchCurl::Get(const char* url, long Timeout, void* FunctionCallback, int CallbackData)
{
	if (this->m_MultiHandle)
	{
		if (url)
		{
			CURL* ch = curl_easy_init();

			if (ch)
			{
				this->m_Data[this->m_RequestIndex] = { 0 };

				this->m_Data[this->m_RequestIndex].Callback = FunctionCallback;

				this->m_Data[this->m_RequestIndex].CallbackData = CallbackData;

				curl_easy_setopt(ch, CURLOPT_URL, url);

				curl_easy_setopt(ch, CURLOPT_TIMEOUT, (Timeout) > 0 ? Timeout : 5);

				curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, this->WriteMemoryCallback);

				curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void*)&this->m_Data[this->m_RequestIndex]);

				curl_easy_setopt(ch, CURLOPT_PRIVATE, this->m_RequestIndex);

				curl_multi_add_handle(this->m_MultiHandle, ch);

				this->m_RequestIndex++;
			}
		}
	}
}

void CMatchCurl::PostJSON(const char* url, long Timeout, std::string PostData, void* FunctionCallback, int CallbackData)
{
	if (this->m_MultiHandle)
	{
		if (url)
		{
			CURL* ch = curl_easy_init();

			if (ch)
			{
				this->m_Data[this->m_RequestIndex] = { 0 };

				this->m_Data[this->m_RequestIndex].Callback = FunctionCallback;

				this->m_Data[this->m_RequestIndex].CallbackData = CallbackData;

				curl_easy_setopt(ch, CURLOPT_URL, url);

				curl_easy_setopt(ch, CURLOPT_TIMEOUT, Timeout);

				curl_easy_setopt(ch, CURLOPT_NOPROGRESS, 1L);

				curl_easy_setopt(ch, CURLOPT_POST, 1L);

				curl_easy_setopt(ch, CURLOPT_POSTFIELDSIZE, (long)PostData.size());

				curl_easy_setopt(ch, CURLOPT_COPYPOSTFIELDS, PostData.c_str());

				curl_easy_setopt(ch, CURLOPT_WRITEFUNCTION, this->WriteMemoryCallback);

				curl_easy_setopt(ch, CURLOPT_WRITEDATA, (void*)&this->m_Data[this->m_RequestIndex]);

				curl_easy_setopt(ch, CURLOPT_PRIVATE, this->m_RequestIndex);

				curl_multi_add_handle(this->m_MultiHandle, ch);

				this->m_RequestIndex++;
			}
		}
	}
}

size_t CMatchCurl::WriteMemoryCallback(void* contents, size_t size, size_t nmemb, void* userp)
{
	size_t realsize = size * nmemb;

	P_CURL_MOD_MEMORY* mem = (P_CURL_MOD_MEMORY*)userp;

	char* ptr = (char*)realloc(mem->Memory, mem->Size + realsize + 1);

	if (ptr)
	{
		mem->Memory = ptr;

		memcpy(&(mem->Memory[mem->Size]), contents, realsize);

		mem->Size += realsize;

		mem->Memory[mem->Size] = 0;

		return realsize;
	}

	return 0;
}
