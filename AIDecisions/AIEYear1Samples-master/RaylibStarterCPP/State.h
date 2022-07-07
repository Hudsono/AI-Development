#pragma once

// Tools
#include <vector>

class Behaviour;
class Agent;
class Condition;

class State
{
public:
	struct Transition
	{
		Condition* condition;	// The condition that needs to be met to transition to the target state. Owned by Transition.
		State* targetState;	// The target state to be transitioned to upon meeting the condition. Reference.
	};

private:
	std::vector<Behaviour*> m_behaviours;	// Polymorphic list--included behaviours may change with inheritance; varying sizes--store as pointers instead.
	std::vector<Transition> m_transitions;

public:
	State();	// Default constructor.
	State(Behaviour* behaviour);	// Constructor which takes one behaviour to add.
	State(std::initializer_list<Behaviour*> behaviours);	// Constructor which takes any number of behaviours to add.

	~State();	// Destructor.

	virtual void Enter(Agent* agent);	// Executes upon entering the state.
	virtual void Update(Agent* agent, float deltaTime);	// Updates all behaviours in the state.
	virtual void Exit(Agent* agent);	// Executes upon leaving the state.

	std::vector<Transition> GetTransitions() { return m_transitions; }	// Return list of transitions this state has.

	void AddTransition(Condition* condition, State* targetState);	// Add a new transition from this state to the target state, given the condition.
};

