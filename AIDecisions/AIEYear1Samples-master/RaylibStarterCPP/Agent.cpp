#include "Agent.h"

// Forward declarations.
#include "Behaviour.h"

Agent::Agent(NodeMap* _nodeMap, Behaviour* _behaviour, Behaviour* _behaviour2)
{
	// Initialise variables.
	m_current = _behaviour;
	m_nodeMap = _nodeMap;
	m_colour = YELLOW;

	m_pathAgent.SetMap(_nodeMap);
	m_current->Enter(this);
}

void Agent::Update(float deltaTime)
{
	// Update the current behaviour, if there is one.
	if (m_current)
		m_current->Update(this, deltaTime);

	// Update the assigned path agent.
	m_pathAgent.Update(deltaTime);
}

void Agent::Draw()
{
	m_pathAgent.Draw();	// Draw the path agent.
	m_nodeMap->DrawPath(m_pathAgent.GetPath(), m_colour);	// Draw the path of this agent.
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
	m_pathAgent.SetSpeed(100);
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
