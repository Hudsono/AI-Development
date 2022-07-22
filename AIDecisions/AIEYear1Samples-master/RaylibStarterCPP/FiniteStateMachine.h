#pragma once

// Inheritance
#include "Behaviour.h"

// Tools
#include <vector>

#include "State.h"

class FiniteStateMachine : public Behaviour
{
private:
	std::vector<State*> m_states;

	State* m_currentState;
	State* m_newState;

public:
	FiniteStateMachine(State* _state) : m_currentState(_state), m_newState(nullptr) {}	// Constructor with initial state set.
	virtual ~FiniteStateMachine();	// Cleans up states as the FSM owns them.

	void Update(Agent* agent, float deltaTime);	// Checks current state's transition conditions and changes accordingly; updates current state.

	void AddState(State* state);	// Add a state for the FSM to be responsible for.
	void Enter(Agent* agent);	// Initialises the current state; used for the first-added state.

	// Deep copy
	FiniteStateMachine& operator= (const FiniteStateMachine& other) { *m_currentState = *other.m_currentState; };
	FiniteStateMachine(const FiniteStateMachine& other) { m_currentState = new State(*other.m_currentState); };
};

