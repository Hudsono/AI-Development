#include "State.h"

// Forward declarations.
#include "Behaviour.h"

struct Transition;

State::State()
{
}

State::State(Behaviour* behaviour)
{
	m_behaviours.push_back(behaviour);	// Add the given behaviour to the state's list of behaviours.
}

State::State(std::initializer_list<Behaviour*> behaviours)
{
	// Add all given behaviour pointers to the state's list of behaviours.
	for (Behaviour* b : behaviours)
		m_behaviours.push_back(b);
}

State::~State()
{
	// States own the behaviours assigned to it, so we need to clean them up.
	// I.e., a Behaviour can only be assigned to one state in this system.
	// Loop through and delete each member behaviour.
	for (Behaviour* behaviour : m_behaviours)
		delete behaviour;

	// States also own all Conditions in each of its Transitions--clean them up too.
	for (Transition transition : m_transitions)
		delete transition.condition;
}

void State::Enter(Agent* agent)
{
	// Call Enter functions for all behaviours in this state.
	for (Behaviour* behaviour : m_behaviours)
		behaviour->Enter(agent);
}

void State::Update(Agent* agent, float deltaTime)
{
	// Update all behaviours in this state. Updated by the FSM owning this state, if this is its current state.
	for (Behaviour* behaviour : m_behaviours)
		behaviour->Update(agent, deltaTime);
}

void State::Exit(Agent* agent)
{
	// Call Exit functions for all behaviours in this state.
	for (Behaviour* behaviour : m_behaviours)
		behaviour->Exit(agent);
}

void State::AddTransition(Condition* condition, State* targetState)
{
	Transition transition{ condition, targetState };	// Create a new transition with the given condition and target state.
	m_transitions.push_back(transition);	// Add this transition to the state's list of them.
}

