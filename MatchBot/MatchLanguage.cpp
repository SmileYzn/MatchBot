#include "precompiled.h"

CMatchLanguage gMatchLanguage;

// On server activate
void CMatchLanguage::ServerActivate()
{
	// Server Language
	this->m_Language = gMatchUtil.CvarRegister("mb_language", "en");

	// Clear Current translation system data
	this->m_Data.clear();
 
	// If string is not null
	if (this->m_Language->string)
	{
		// If string is not empty
		if (this->m_Language->string[0] != '\0')
		{
			// File stream
			std::ifstream fp;

			// Open file ready only
			fp.open(MB_LANGUAGE_FILE, std::ifstream::in);

			// If is open
			if (fp.is_open())
			{
				// Create key and line
				std::string Key, Line;

				// while reading lines
				while(std::getline(fp, Line))
				{
					// If is not empty
					if (!Line.empty())
					{
						// Escape all special chracters
						this->ReplaceAll(Line, "\\1", "\1");
						this->ReplaceAll(Line, "\\3", "\3");
						this->ReplaceAll(Line, "\\4", "\4");
						this->ReplaceAll(Line, "\\n", "\n");

						// If has quotes
						if (Line[0] == '"')
						{
							// Remove quote from begining of string
							Line.erase(std::remove(Line.begin(), Line.end(), '\"'), Line.end());

							// This line is a key
							Key = Line;
						}
						// Compare language setting with key
						else if (Line.substr(0, 2).compare(this->m_Language->string) == 0)
						{
							// Get line after third character
							Line = Line.substr(3);

							// Remove quotes from string
							Line.erase(std::remove(Line.begin(), Line.end(), '\"'), Line.end());

							// Insert to language system
							this->m_Data[Key] = Line;
						}
					}
				}

				// Close file pointer
				fp.close();

				// Test
				for (auto const& row : this->m_Data)
				{
					SERVER_PRINT(gMatchUtil.FormatString("[%s] %s %s\n", __func__, row.first.c_str(), row.second.c_str()));
				}
			}
		}
	}
}

// Replace all in string
void CMatchLanguage::ReplaceAll(std::string& String, const std::string& From, const std::string& To)
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

// Get language by key, return same key if not found
const char* CMatchLanguage::Get(const char* Key)
{
	// If is not null
	if (Key)
	{
		// Find key
		if (this->m_Data.find(Key) != this->m_Data.end())
		{
			// Return string of thi skey
			return this->m_Data[Key].c_str();
		}
	}

	// Return key
	return Key;
}
