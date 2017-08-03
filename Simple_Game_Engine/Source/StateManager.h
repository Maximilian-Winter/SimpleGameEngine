//StateManager to handle different application states and give information about it
#ifndef _STATEMANAGER_H_
#define _STATEMANAGER_H_


enum ApplicationState
{
	IsStarted,
	IsPaused,
	IsDrawingMouseCursor
};

class StateManager
{
	struct ApplicationStates
	{
		ApplicationState State;
		bool IsTrue;
	};

public:

	StateManager();
	~StateManager();
	void SetApplicationState(ApplicationState, bool);
	bool GetApplicationState(ApplicationState);

private:
	ApplicationStates m_IsStarted;
	ApplicationStates m_IsPaused;
	ApplicationStates m_IsDrawingMouseCursor;

};
#endif
