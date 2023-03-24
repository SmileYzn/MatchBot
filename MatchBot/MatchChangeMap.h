#pragma once

class CMatchChangeMap
{
public:
	void ChangeMap(std::string MapName, float Delay, bool IntermissionMsg);
	static void ChangeLevel(int MapIndex);
	const char* GetNextMap();

private:
	std::string m_NextMap = "";
};

extern CMatchChangeMap gMatchChangeMap;