#pragma once
#include "raylib.h"

class Agent;	// Forward declaration--just need to know what an Agent is for the Agent pointer.

// Abstract base class for all actions.
class Behaviour
{
public:
	virtual void Update(Agent* agent, float deltaTime) = 0;
};

class GotoPointBehaviour : public Behaviour
{
public:
	virtual void Update(Agent* agent, float deltaTime);
};

class WanderBehaviour : public Behaviour
{
public:
	virtual void Update(Agent* agent, float deltaTime);
};

class FollowerBehaviour : public Behaviour
{
public:
	virtual void Update(Agent* agent, float deltaTime);
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