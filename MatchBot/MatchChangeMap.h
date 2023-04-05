#pragma once

class CMatchChangeMap
{
public:
	void ChangeMap(std::string Map, float Delay, bool IntermissionMsg);
	static void ChangeLevel(int MapIndex);
	std::string GetNextMap();

private:
	std::string m_NextMap;
};

extern CMatchChangeMap gMatchChangeMap;