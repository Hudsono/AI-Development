#pragma once

#include "BH_Behaviour.h"

// Here be behaviours defined with custom logic.

// Con: checks if this agent's target is within the given radius.
class ConditionTargetClose : public BT_Behaviour
{
private:
	float m_radius;	// How close the agent's target must be to return as true.
	bool m_outside;	// Whether the condition returns true if outside the given radius rather than inside.

public:
	ConditionTargetClose(float _radius, bool _outside) : m_radius(_radius), m_outside(_outside) {}	// Constructor taking a radius as argument and whether inside or outside the radius returns Successful.
	virtual Status Update(Agent* agent, float deltaTime);	// Checks the distance between the agent and its target; returns true if within the radius.
};

// Act: paths towards its target.
class ActionChase : public BT_Behaviour
{
public:
	Vector2 m_targetLastPos;	// The last time we tried to path to a given target.

public:
	virtual Status Update(Agent* agent, float deltaTime);
};

// Act: paths to a random point.
class ActionWander : public BT_Behaviour
{
	virtual Status Update(Agent* agent, float deltaTime);
};