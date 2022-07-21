#pragma once

// Tools
#include "raylib.h"
#include <vector>


class Agent;	// Forward declaration--just need to know what an Agent is for the Agent pointer.
class Decision;
class BT_Behaviour;

// Abstract base class for all actions.
class Behaviour
{
public:
	virtual void Enter(Agent* agent) {}	// Logic executing upon entry. Empty stub function--not every behaviour needs entry logic.
	virtual void Update(Agent* agent, float deltaTime) = 0;	// Pure virtual function; abstract. Child classes need this implemented.
	virtual void Exit(Agent* agent) {}	// Logic executing upon exit. Empty stub function--not every behaviour needs exit logic.

	// Used by UtilityAI to determine what behaviour to execute.
	virtual float Evaluate(Agent* agent) { return 0.0f; }

	// Resets the minimum-acting timer for this behaviour (cannot transition until timer is up!). Defaults to having no timer.
	virtual float ResetTimer() { return 0.0f; }
};

class GotoPointBehaviour : public Behaviour
{
public:
	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime);
};

class WanderBehaviour : public Behaviour
{
public:
	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime);

	virtual float Evaluate(Agent* agent);

	virtual float ResetTimer() { return 0.2f; }	// Minimum 200ms acting this behaviour.
};

class FollowerBehaviour : public Behaviour
{
public:
	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime);
	virtual void Exit(Agent* agent);

	virtual float Evaluate(Agent* agent);

	virtual float ResetTimer() { return 3.0f; }	// Minimum 3 seconds acting this behaviour.
private:
	Vector2 lastTargetPosition;	// The last time we tried to path to a given target.
};

// The agent slowly follows a set path until disturbed.
class GuardBehaviour : public Behaviour
{
public:
	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime);
};

// The guard stops in place for a bit, looks around.
class PostBehaviour : public Behaviour
{
public:
	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime);
};

// The agent senses a threat but cannot see it; not confident where the threat is. Does not want to "alert" the threat--slow/no movement.
class SusBehaviour : public Behaviour
{
public:
	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime);
};

// The agent is somewhat confident or interested in a potential threat/just lost sight of their target.
class SearchBehaviour : public Behaviour
{
public:
	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime);
};

// The agent's target is within range to deal damage to it.
class AttackBehaviour : public Behaviour
{
public:
	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime);
};

class SelectorBehaviour : public Behaviour
{
private:
	Behaviour* m_b1;
	Behaviour* m_b2;
	Behaviour* m_selected;
	bool m_b1Finish;

public:
	SelectorBehaviour(Behaviour* b1, Behaviour* b2) : m_b1(b1), m_b2(b2) {}
	~SelectorBehaviour() { delete m_b1; delete m_b2; }
	virtual void Update(Agent* agent, float deltaTime);

	void SetBehaviour(Behaviour* b, Agent* agent);
};

class DecisionBehaviour : public Behaviour
{
private:
	Decision* m_decision;

public:
	virtual void Update(Agent* agent, float deltaTime);
};

// Root of a Behaviour Tree. Inherits from base Behaviour for Agent compatability/flexibility.
// .. this name is a bit long, but it's descriptive among all the other things happeneing here..
class BehaviourTreeBehaviour : public Behaviour
{
private:
	std::vector<BT_Behaviour*> m_BTbehaviours;	// List of tree behaviours to traverse through initially.

public:
	// Constructor to add varying number of BT behaviours.
	BehaviourTreeBehaviour(std::initializer_list<BT_Behaviour*> _BT_behaviours);

	// Checks each of its behaviours
	virtual void Update(Agent* agent, float deltaTime);
};