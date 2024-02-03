#pragma once

class CMatchReady
{
public:
	void Init();
	void Stop(bool ChangeState);

	static void Task(int PlayersMin);
	static void TaskMinimal(int PlayersMin);

	void PlayerGetIntoGame(CBasePlayer* Player);

	void SetReady(int EntityIndex, bool Ready);
	bool GetReady(int EntityIndex);

	bool Ready(CBasePlayer* Player);
	bool NotReady(CBasePlayer* Player);

private:
	bool m_Running = false;
	std::array<int, MAX_CLIENTS + 1> m_Ready;
};

extern CMatchReady gMatchReady;