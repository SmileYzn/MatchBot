#pragma once

// Translate Macro, put in new translation system
#define _T(TextString) gMatchLanguage.Get(TextString)

// Default language file
constexpr auto MB_LANGUAGE_FILE = "cstrike/addons/matchbot/language.txt";

class CMatchLanguage
{
public:
	//
	void ServerActivate();

	//
	void ReplaceAll(std::string& String, const std::string& From, const std::string& To);

	//
	const char* Get(const char* Key);

private:
	cvar_t m_Language;

	std::map<std::string, std::string> m_Data;
};

extern CMatchLanguage gMatchLanguage;

