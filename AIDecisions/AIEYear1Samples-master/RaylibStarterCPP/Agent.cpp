#include "Agent.h"

// Forward declarations.
#include "Behaviour.h"
#include "Blackboard.h"

Agent::Agent(NodeMap* _nodeMap, Behaviour* _behaviour, BlackBoard* _blackBoard)
{
	// Initialise variables.
	m_current = _behaviour;
	m_nodeMap = _nodeMap;
	m_colour = YELLOW;

	m_pathAgent.SetMap(_nodeMap);
	m_current->Enter(this);

	m_patrolProgress = 0;	// Initialise at the start of the patrol path.
	m_suspicion = 0;	// Start suspecting nothing.
	m_blackBoard = _blackBoard;
}

void Agent::Update(float deltaTime)
{
	// Update the current behaviour, if there is one.
	if (m_current)
		m_current->Update(this, deltaTime);

	// Update the assigned path agent.
	m_pathAgent.Update(deltaTime);

	// Turn to face the view target smoothly. Less smooth/snappier if more suspicious.

}

void Agent::Draw()
{
	//m_pathAgent.Draw();	// Draw the path agent.
	m_nodeMap->DrawPath(m_pathAgent.GetPath(), m_colour);	// Draw the path of this agent.

	// The agent's FOV. Used to visually detect objects, like the player.
	DrawCircleSectorLines(m_pathAgent.GetPosition(), 200, 10, 170, 1, RED);

	// Draw a triangle to represent the agent
	DrawPoly(m_pathAgent.GetPosition(), 3, 10, m_rotation + 33, m_colour);
}

void Agent::GoTo(Vector2 point)
{
	m_pathAgent.SetNode(m_nodeMap->GetClosestNode(m_pathAgent.GetPosition()));	// Sets start point as the node closest to the agent's current location.
	Node* end = m_nodeMap->GetClosestNode(point);	// Sets end point as the node closest to the given location.
	m_pathAgent.GoToNode(end);	// Tell the agent to path to the end node, from the start node.
}

void Agent::GoToRandom()
{
	m_pathAgent.SetNode(m_nodeMap->GetClosestNode(m_pathAgent.GetPosition()));	// Sets start point as the node closest to the agent's current location.
	Node* end = m_nodeMap->GetRandomNode();	// Sets end point to a random valid node.
	m_pathAgent.GoToNode(end);	// Tell the agent to path to the end node, from the start node.
	
}

void Agent::SetNode(Node* node)
{
	m_pathAgent.SetNode(node);
	//.SetSpeed(100);
}

bool Agent::PathComplete()
{
	return m_pathAgent.GetPath().empty();	// True when there's nowhere to path to anymore; completed.
}

void Agent::SetSpeed(float speed)
{
	m_pathAgent.SetSpeed(speed);
}

NodeMap* Agent::GetNodeMap()
{
	return m_nodeMap;
}

void Agent::SetTarget(Agent* agent)
{
	m_target = agent;
}

Agent* Agent::GetTarget()
{
	return m_target;
}

Vector2 Agent::GetPos()
{
	return m_pathAgent.GetPosition();
}

void Agent::SetPos(Vector2 pos)
{
	m_pathAgent.SetPosition(pos);
}

void Agent::SetNodePos(Vector2 pos)
{
	m_pathAgent.SetPosition({ pos.x * m_nodeMap->CellSize(), pos.y * m_nodeMap->CellSize() });
}

void Agent::Reset()
{
	m_pathAgent.ClearPath();
}

void Agent::SetColour(Color colour)
{
	m_colour = colour;
	m_pathAgent.SetColour(colour);
}

Node* Agent::IncPatrolPath()
{
	// Increment the patrol progress and check it hasn't overflowed available patrol points.
	if (++m_patrolProgress > m_patrolPoints.size() - 1)
		m_patrolProgress = 0;	// If so, reset the progress.

	// Return the current patrol point.
	return m_patrolPoints[m_patrolProgress];
}
