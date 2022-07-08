#pragma once

#include "Behaviour.h"
#include "Condition.h"

// Tools
#include <vector>

class Agent;

// Base class for decisions.
class Decision
{
public:
	// Pure virtaul function to make a decision.
	virtual void makeDecision(Agent* agent, float deltaTime) = 0;
};

// A decision tree branch node.
class ABDecision : Decision
{
private:
	Decision* treeA;	// If the condition is true.
	Decision* treeB;	// If the condition is false.
	Condition* m_condition;	// Condition that must be met to decide on tree A; otherwise, decide on tree B.

public:
	// Constructor taking options to define the A and B decisions. Can also be defined as final actions.
	ABDecision(Decision* _treeA, Decision* _treeB, Condition* _condition) : treeA(_treeA), treeB(_treeB), m_condition(_condition) {}
	virtual void makeDecision(Agent* agent, float deltaTime);
};

// An action node--Decision Tree leaf node.
class ActionDecision : Decision
{
public:
	virtual void makeDecision(Agent* agent, float deltaTime);	// The action function we want.
};
