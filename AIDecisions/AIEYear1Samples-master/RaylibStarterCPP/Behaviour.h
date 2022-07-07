#pragma once

// Tools
#include "raylib.h"

class Agent;	// Forward declaration--just need to know what an Agent is for the Agent pointer.

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
	virtual void Update(Agent* agent, float deltaTime);
};

class WanderBehaviour : public Behaviour
{
public:
	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime);

	virtual float Evaluate(Agent* agent);

	virtual float ResetTimer() { return 1.0f; }	// Minimum 1 second acting this behaviour.
};

class FollowerBehaviour : public Behaviour
{
public:
	virtual void Enter(Agent* agent);
	virtual void Update(Agent* agent, float deltaTime);

	virtual float Evaluate(Agent* agent);

	virtual float ResetTimer() { return 3.0f; }	// Minimum 3 seconds acting this behaviour.
private:
	Vector2 lastTargetPosition;	// The last time we tried to path to a given target.
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