#pragma once

// Flags for stats commands
constexpr auto CMD_ALL	= 0;		/* All Options */
constexpr auto CMD_HP	= BIT(0);	/* flag "a" */
constexpr auto CMD_DMG	= BIT(1);	/* flag "b" */
constexpr auto CMD_RDMG = BIT(2);	/* flag "c" */
constexpr auto CMD_SUM	= BIT(3);	/* flag "d" */

class CMatchStats
{
public:
	// On Server activate
	void ServerActivate();

	// Match State
	void SetState(int State, bool KnifeRound);

	// Round Start
	void RoundStart();

	// Round End
	void RoundEnd(int winStatus, ScenarioEventEndRound eventScenario, float tmDelay);

	// Player Damage
	void PlayerDamage(CBasePlayer* Victim, entvars_t* pevInflictor, entvars_t* pevAttacker, float& flDamage, int bitsDamageType);

	// Show Enemy HP
	bool ShowHP(CBasePlayer* Player, bool Command, bool InConsole);

	// Show Damage
	bool ShowDamage(CBasePlayer* Player, bool Command, bool InConsole);

	// Show Received Damage
	bool ShowReceivedDamage(CBasePlayer* Player, bool Command, bool InConsole);

	// Show Round Summary
	bool ShowSummary(CBasePlayer* Player, bool Command, bool InConsole);

	// Round End Stats
	static void RoundEndStats(int State);

private:
	// Match State
	int m_State = STATE_DEAD;

	// Round Stats Damage
	std::array<std::array<int, MAX_CLIENTS + 1>, MAX_CLIENTS + 1> m_RoundDmg;

	// Round Stats Hits
	std::array<std::array<int, MAX_CLIENTS + 1>, MAX_CLIENTS + 1> m_RoundHit;

	// Match stats commands
	int m_StatsCommandFlags = CMD_ALL;
};

extern CMatchStats gMatchStats;