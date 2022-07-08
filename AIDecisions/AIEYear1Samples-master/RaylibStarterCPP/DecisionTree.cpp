#include "DecisionTree.h"

void ABDecision::makeDecision(Agent* agent, float deltaTime)
{
	// If the condition is met, go left (tree A); otherwise, go right (tree B)
	// Also checks that either tree exists.
	if (m_condition && treeA)
		treeA->makeDecision(agent, deltaTime);
	else if (treeB)
		treeB->makeDecision(agent, deltaTime);
}

void ActionDecision::makeDecision(Agent* agent, float deltaTime)
{
}

//void Sequence::Update(Agent* agent, float deltaTime)
//{
//	// Search each child behaviour.
//	// First successful child returns success overall; otherwise, fail.
//	//for (Behaviour* child : children)
//	//{
//	//	if (!child->Update(agent, deltaTime))
//	//		return false;
//	//	
//	//	return true;
//	//}
//}
//
//void Selector::Update(Agent* agent, float deltaTime)
//{
//	// Search each child behaviour.
//	// First successful child returns success overall; otherwise, fail.
//	//for (Behaviour* child : children)
//	//{
//	//	if (child->Update(agent, deltaTime))
//	//		return true;
//	//
//	//	return false;
//	//}
//}
