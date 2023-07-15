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

			//// Loop each item of array
			//for (auto const& row : json.items())
			//{
			//	// If value is not empty
			//	if (!row.value().empty())
			//	{
			//		// Loop objects of value
			//		for (auto const& el : row.value().items())
			//		{
			//			// If is equal to defined language
			//			if (el.key().compare(Language) == 0)
			//			{
			//				// If text value is not empty
			//				if (!el.value().empty())
			//				{
			//					// If text value is an string
			//					if (el.value().is_string())
			//					{
			//						// Insert
			//						this->m_Data.insert(std::make_pair(row.key(), std::string(el.value())));
			//					}
			//				}
			//			}
			//		}
			//	}
			//}

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
