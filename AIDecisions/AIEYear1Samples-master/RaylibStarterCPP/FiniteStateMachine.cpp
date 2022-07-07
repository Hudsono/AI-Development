#include "FiniteStateMachine.h"

#include "Condition.h"

FiniteStateMachine::~FiniteStateMachine()
{
	// The FSM owns all states assigned to it--clean them up.
	for (State* state : m_states)
		delete state;
}

void FiniteStateMachine::Update(Agent* agent, float deltaTime)
{
	//State* newState = nullptr;	// To store the new state, if applicable.

	// Check the current state's transitions.
	for (State::Transition transition : m_currentState->GetTransitions())
	{
		// For the /first/ condition that's true, transition to its target state.
		if (transition.condition->IsTrue(agent))
			m_newState = transition.targetState;
	}

	// If we've changed state, clean up the old one and initialise the new one.
	if (m_newState != nullptr && m_newState != m_currentState)
	{
		m_currentState->Exit(agent);	// Exit current state.
		m_currentState = m_newState;	// Make new state the current state.
		m_currentState->Enter(agent);	// Initialise the now-current state.
		m_newState = nullptr;	// Reset new state data.
	}

	// Update only the current state.
	m_currentState->Update(agent, deltaTime);
}

void FiniteStateMachine::AddState(State* state)
{
	m_states.push_back(state);
}

void FiniteStateMachine::Enter(Agent* agent)
{
	m_currentState->Enter(agent);
}
