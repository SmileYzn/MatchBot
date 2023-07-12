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
			//try
			//{
			//	// File stream
			//	std::ifstream fp(MB_LANGUAGE_FILE, std::ios::in);

			//	// If file is open
			//	if (fp)
			//	{
			//		// Reset pointer
			//		fp.clear();

			//		// Go to begin of file
			//		fp.seekg(0, std::ios::beg);

			//		// Read data from json file
			//		auto json = nlohmann::ordered_json::parse(fp, nullptr, true, true);

			//		// Loop each item of array
			//		for (auto const& row : json.items())
			//		{
			//			// If value is not empty
			//			if (!row.value().empty())
			//			{
			//				// Loop objects of value
			//				for (auto const& el : row.value().items())
			//				{
			//					// If is equal to defined language
			//					if (el.key().compare(Language) == 0)
			//					{
			//						// If text value is not empty
			//						if (!el.value().empty())
			//						{
			//							// Get line
			//							auto Text = std::string(el.value());

			//							// For chat and hudmessages
			//							gMatchUtil.ReplaceAll(Text, "^1", "\1");
			//							gMatchUtil.ReplaceAll(Text, "^3", "\3");
			//							gMatchUtil.ReplaceAll(Text, "^4", "\4");
			//							gMatchUtil.ReplaceAll(Text, "^n", "\n");

			//							// For Menus
			//							gMatchUtil.ReplaceAll(Text, "^w", "\\w");
			//							gMatchUtil.ReplaceAll(Text, "^y", "\\y");
			//							gMatchUtil.ReplaceAll(Text, "^r", "\\r");
			//							gMatchUtil.ReplaceAll(Text, "^R", "\\R");

			//							// Insert on final container
			//							this->m_Data.insert(std::make_pair(row.key(), Text));
			//						}
			//					}
			//				}
			//			}
			//		}

			//		// Close file
			//		fp.close();
			//	}
			//	else
			//	{
			//		LOG_CONSOLE(PLID, "[%s] Failed to open file: %s", __func__, MB_LANGUAGE_FILE);
			//	}
			//}
			//catch (nlohmann::json::parse_error& e)
			//{
			//	LOG_CONSOLE(PLID, "[%s] %s", __func__, e.what());
			//}
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
