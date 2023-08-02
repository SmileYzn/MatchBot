#include "precompiled.h"

CMatchLanguage gMatchLanguage;

// On server activate
void CMatchLanguage::Load()
{
	// Clear Current translation system data
	this->m_Data.clear();

	try
	{
		// File stream
		std::ifstream fp(MB_LANGUAGE_FILE, std::ios::in);

		// If file is open
		if (fp)
		{
			// Reset pointer
			fp.clear();

			// Go to begin of file
			fp.seekg(0, std::ios::beg);

			// Read data from json file
			this->m_Data = nlohmann::ordered_json::parse(fp, nullptr, true, true);

			// Close file
			fp.close();
		}
		else
		{
			LOG_CONSOLE(PLID, "[%s] Failed to open file: %s", __func__, MB_LANGUAGE_FILE);
		}
	}
	catch (nlohmann::json::parse_error& e)
	{
		LOG_CONSOLE(PLID, "[%s] %s", __func__, e.what());
	}
}

// Get language by key, return same key if not found
const char* CMatchLanguage::Get(const char* Key)
{
	// If is not null
	if (Key)
	{
		// If has language variable
		if (gMatchBot.m_Language)
		{
			// If language variable has value
			if (gMatchBot.m_Language->string)
			{
				// If contain the given key
				if (this->m_Data.contains(Key))
				{
					// If the language key contains the translation of language string
					if (this->m_Data[Key].contains(gMatchBot.m_Language->string))
					{
						// Return pointer
						return this->m_Data[Key][gMatchBot.m_Language->string].get_ptr<nlohmann::json::string_t*>()->c_str();
					}
				}
			}
		}
	}

	// Return key
	return Key;
}
