#pragma once
#include "Agent.h"
class Guard : public Agent
{
private:
	std::vector<Node*> m_patrolPoints;	// Nodes this guard is assigned to patrol. One patrol point is a single point to guard/wander near. Multiple is a patrol path.

public:

};

