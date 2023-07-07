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
            try
            {
                // File stream
                std::ifstream fp(MB_LANGUAGE_FILE);

                // Reset pointer
                fp.clear();

                // Go to begin of file
                fp.seekg(0, std::ios::beg);

                // Read data from json file
                auto json = nlohmann::json::parse(fp, nullptr, true, true);

                // Loop each item of array
                for (auto const& row : json.items())
                {
					auto Value = row.value().at(Language);
					
					if (!Value.empty())
					{
						auto Text = Value.get<std::string>();

						if (!Text.empty())
						{
							gMatchUtil.ReplaceAll(Text, "^1", "\1");
							gMatchUtil.ReplaceAll(Text, "^3", "\3");
							gMatchUtil.ReplaceAll(Text, "^4", "\4");
							gMatchUtil.ReplaceAll(Text, "^n", "\n");

							gMatchUtil.ReplaceAll(Text, "^w", "\\w");
							gMatchUtil.ReplaceAll(Text, "^y", "\\y");
							gMatchUtil.ReplaceAll(Text, "^r", "\\r");
							gMatchUtil.ReplaceAll(Text, "^R", "\\R");
							
							this->m_Data.insert(std::make_pair(row.key(), Text));
						}
					}
                }
            }
            catch (nlohmann::json::parse_error& e)
            {
                LOG_CONSOLE(PLID, "[%s] %s", __func__, e.what());
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
