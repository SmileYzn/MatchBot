#pragma once

class CMatchScore
{
public:
    // On server actiave event
    void ServerActivate();

	// TeamScore HL1 Message
	static bool TeamScore(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity);

	// ScoreInfo HL1 Message
	static bool ScoreInfo(int msg_dest, int msg_type, const float* pOrigin, edict_t* pEntity);
};

extern CMatchScore gMatchScore;