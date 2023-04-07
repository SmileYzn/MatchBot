#include "precompiled.h"

CMatchLanguage gMatchLanguage;

// On server activate
void CMatchLanguage::Load(const char* Language)
{
	// Clear Current translation system data
	this->m_Data.clear();
 
	// If string is not null
	if (Language)
	{
		// If string is not empty
		if (Language[0] != '\0')
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
						gMatchUtil.ReplaceAll(Line, "\\1", "\1");
						gMatchUtil.ReplaceAll(Line, "\\3", "\3");
						gMatchUtil.ReplaceAll(Line, "\\4", "\4");
						gMatchUtil.ReplaceAll(Line, "\\n", "\n");

						// If has quotes
						if (Line[0] == '"')
						{
							// Remove quote from begining of string
							Line.erase(std::remove(Line.begin(), Line.end(), '\"'), Line.end());

							// This line is a key
							Key = Line;
						}
						// Compare language setting with key
						else if (Line.substr(0, 2).compare(Language) == 0)
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
			}
		}
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
			// Return result
			return this->m_Data[Key].c_str();
		}
	}

	// Return key
	return Key;
}
