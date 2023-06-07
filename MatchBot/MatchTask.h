#pragma once

constexpr auto TASK_CHANGE_STATE = 1337;
constexpr auto TASK_READY_SYSTEM = 1338;
constexpr auto TASK_TIMER_SYSTEM = 1339;
constexpr auto TASK_VOTE_TIMER = 1440;
constexpr auto TASK_VOTE_LIST = 1441;
constexpr auto TASK_TIMER_LO3 = 1442;
constexpr auto TASK_CHANGE_MAP = 1443;
constexpr auto TASK_PAUSE_MATCH = 1444;
constexpr auto TASK_ROUND_END_STATS = 1445;
constexpr auto TASK_SAVE_STATS = 1446;

typedef struct S_TASK_INFO
{
	int				Index;
	float			Time;
	float			EndTime;
	bool			Loop;
	void*			FunctionCallback;
	int				FunctionParameter;
} P_TASK_INFO, *LP_TASK_INFO;

class CMatchTask
{
public:
	void ServerActivate();
	void ServerDeactivate();

	void Clear();
	void Create(int Index, float Time, bool Loop, void* FunctionCallback, int FunctionParameter);
	void EndTime(int Index, float EndTime);
	void Remove(int Index);
	void Execute(int Index);
	void Frame();

private:
	std::unordered_map<int, P_TASK_INFO> m_Data;
};

extern CMatchTask gMatchTask;
