#include "precompiled.h"

CMatchLanguage gMatchLanguage;

// On server activate
void CMatchLanguage::Load()
{
	// Clear Current translation system data
	this->m_Data.clear();

    // Memory Script instance
    CMemScript* lpMemScript = new CMemScript;

    // If is not null
    if (lpMemScript)
    {
        // Try to load file
        if (lpMemScript->SetBuffer(MB_LANGUAGE_FILE))
        {
            try
            {
                // Loop lines
                while (true)
                {
                    // If file content ended
                    if (lpMemScript->GetToken() == eTokenResult::TOKEN_END)
                    {
                        // Break loop
                        break;
                    }

                    // Sentence Key
                    auto Key = lpMemScript->GetString();

                    // If is not empty
                    if (!Key.empty())
                    {
                        // Loop lines
                        while (true)
                        {
                            // If is end of section
                            if (lpMemScript->GetAsString().compare("end") == 0)
                            {
                                // Break loop
                                break;
                            }

                            // Get language
                            auto Lang = lpMemScript->GetString();

                            // If is not empty
                            if (!Lang.empty())
                            {
                                // Load Sentence
                                this->m_Data[Key][Lang] = lpMemScript->GetAsString();
                            }
                        }
                    }
                }
            }
            catch (...)
            {
                // Catch for erros
                LOG_CONSOLE(PLID, "[%s] %s", __func__, lpMemScript->GetError().c_str());
            }
        }

        // Delete Memory Script instance
        delete lpMemScript;
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
				if (this->m_Data.find(Key) != this->m_Data.end())
				{
					// If the language key contains the translation of language string
					if (this->m_Data[Key].find(gMatchBot.m_Language->string) != this->m_Data[Key].end())
					{
						// Return pointer
                        return this->m_Data[Key][gMatchBot.m_Language->string].c_str();
					}
				}
			}
		}
	}

	// Return key
	return Key;
}
