#include "precompiled.h"

#include <regex>

CMatchLanguage gMatchLanguage;

void CMatchLanguage::ServerActivate()
{
	// Server Language
	this->m_Language = gMatchUtil.CvarRegister("mb_language", "bp");

	// Clear Current translation system data
	this->m_Data.clear();

	if (this->m_Language->string)
	{
		if (this->m_Language->string[0] != '\0')
		{
			std::ifstream fp;

			fp.open(MB_LANGUAGE_FILE, std::ifstream::in);

			if (fp.is_open())
			{
				std::string Key, Line;

				while(std::getline(fp, Line))
				{
					if (!Line.empty())
					{
						this->ReplaceAll(Line, "\\1", "\1");
						this->ReplaceAll(Line, "\\3", "\3");
						this->ReplaceAll(Line, "\\4", "\4");
						this->ReplaceAll(Line, "\\n", "\n");

						if (Line[0] == '"')
						{
							Line.erase(std::remove(Line.begin(), Line.end(), '\"'), Line.end());

							Key = Line;
						}
						else if (Line.substr(0, 2).compare(this->m_Language->string) == 0)
						{
							Line = Line.substr(3);

							Line.erase(std::remove(Line.begin(), Line.end(), '\"'), Line.end());

							this->m_Data[Key] = Line;
						}
					}
				}

				fp.close();
			}
		}
	}
}


void CMatchLanguage::ReplaceAll(std::string& String, const std::string& From, const std::string& To)
{
	size_t StartPos = 0;

	while ((StartPos = String.find(From, StartPos)) != std::string::npos)
	{
		String.replace(StartPos, From.length(), To);

		StartPos += To.length();
	}
}

const char* CMatchLanguage::Get(const char* Key)
{
	if (Key)
	{
		if (this->m_Data.find(Key) != this->m_Data.end())
		{
			return this->m_Data[Key].c_str();
		}
		
		LOG_CONSOLE(PLID, "[%s][KEY_NOT_FOUND] %s", __func__, Key);
	}

	return Key;
}
