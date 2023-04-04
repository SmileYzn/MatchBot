#pragma once

class CMatchChangeMap
{
public:
	void ChangeMap(const char* MapName, float Delay, bool IntermissionMsg);
	static void ChangeLevel(int MapIndex);
	char* GetNextMap();

private:
	char m_NextMap[32] = { 0 };
};

extern CMatchChangeMap gMatchChangeMap;