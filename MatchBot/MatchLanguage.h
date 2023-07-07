#pragma once

// Translate Macro, put in new translation system
#define _T(TextString) gMatchLanguage.Get(TextString)

// Default language file
constexpr auto MB_LANGUAGE_FILE = "cstrike/addons/matchbot/language.json";

class CMatchLanguage
{
public:
	// Load Language file
	void Load(const char* Language);

	// Get language by key, return same key if not found
	const char* Get(const char* Key);

private:
	std::unordered_map<std::string, std::string> m_Data;
};

extern CMatchLanguage gMatchLanguage;

