#pragma once

// Tools.
#include <vector>
#include <raylib.h>

// Forward declarations.
class Agent;

// Status reporting for BT nodes.
enum class Status
{
	Ready,		// Ready to be checked.
	Success,	// Either Composite logic resulted true, a Condition resulted true, or an Action completed.
	Failure,	// Either Composite logic resulted false or a Condition resulted false.
	Running		// An Action/Condition is running this frame; won't be done running this frame.
};

// Negation unary operator. Success returns Failure, vice-versa. Running and Ready are unaffected.
//Status operator!(Status& _status)
//{
//	// Flip Success and Failure states, based on which is given.
//	if (_status == Status::Success)
//		_status = Status::Failure;
//	else if (_status == Status::Failure)
//		_status = Status::Success;
//
//	return _status;		// Return the new (or unchanged Running/Ready) state.
//}

// Base class for Behaviour Tree behaviours, needing special status updates. Separate from regular Behaviours for this purpose.
class BT_Behaviour
{
private:
	Status m_status;	// Current status of this behaviour.

public:
	BT_Behaviour() : m_status(Status::Ready) {};	// Default Constructor. Starts status to Ready.

	virtual Status Update(Agent* agent, float deltaTime) = 0;	// Updates logic--to be overridden with custom logic.
	virtual void StartCheck() {}		// Logic to be executed when we begin checking this behaviour--can be overridden with custom logic.
	
	Status Check(Agent* agent, float deltaTime);	// Checks the status of children, updates the behaviour.
	Status GetStatus() { return m_status; }	// Returns the status of this behaviour; has no logic.
};

   //////////////////
  // BRANCH NODES //
 //////////////////

// Abstract base class for branch nodes.
class BT_Composite : public BT_Behaviour
{
private:
	BT_Behaviour* m_currentRun;	// The currently-running behaviour of this composite, if there is one.

protected:
	std::vector<BT_Behaviour*> m_children;	// Behaviours to select/sequence. Needs to be accessed by sequences, selectors, etc.

public:
	BT_Composite(std::initializer_list<BT_Behaviour*> _behaviours);	// Constructor taking any number of behaviours to add.

	void AddNode(BT_Behaviour* _node);	// Add a node to this composite's children.
};

// AND node for running a list of behaviours consecutively until one fails.
class BT_Sequence : public BT_Composite
{
public:
	using BT_Composite::BT_Composite;
	virtual Status Update(Agent* agent, float deltaTime);	// Checks children until one doesn't report success.
};

// OR node for running a list of behaviours consecutively until one succeeds.
class BT_Selector : public BT_Composite
{
public:
	using BT_Composite::BT_Composite;
	virtual Status Update(Agent* agent, float deltaTime);	// Checks children until one doesn't report failure.
};