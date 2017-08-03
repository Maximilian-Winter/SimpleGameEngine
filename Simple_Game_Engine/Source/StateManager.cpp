#include "StateManager.h"


StateManager::StateManager()
{
	m_IsPaused.State = IsPaused;
	m_IsPaused.IsTrue = false;

	m_IsDrawingMouseCursor.State = IsDrawingMouseCursor;
	m_IsDrawingMouseCursor.IsTrue = false;

	m_IsStarted.State = IsStarted;
	m_IsStarted.IsTrue = false;
	
}

StateManager::~StateManager()
{
}

void StateManager::SetApplicationState(ApplicationState state, bool condition)
{	
	if (state == IsStarted)
	{
		m_IsStarted.IsTrue = condition;
	}
	else if (state == IsPaused)
	{
		m_IsPaused.IsTrue = condition;
	}
	else if (state == IsDrawingMouseCursor)
	{
		m_IsDrawingMouseCursor.IsTrue = condition;
	}
}

bool StateManager::GetApplicationState(ApplicationState state)
{
	if (state == IsStarted)
	{
		return m_IsStarted.IsTrue;
	}
	else if (state == IsPaused)
	{
		return m_IsPaused.IsTrue;
	}
	else if (state == IsDrawingMouseCursor)
	{
		return m_IsDrawingMouseCursor.IsTrue;
	}
	else
	{
		return false;
	}
}