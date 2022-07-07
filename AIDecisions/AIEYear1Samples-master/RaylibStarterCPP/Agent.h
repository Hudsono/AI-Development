#pragma once

#include "Pathfinding.h"

class Behaviour;

// AI Agent dedicated to making logical decisions.
class Agent
{
public:
	Agent() {}	// Base constructor.

	// Constructor to assign a node map and a starting behaviour. Yellow.
	Agent(NodeMap* _nodeMap, Behaviour* _behaviour, Behaviour* _behaviour2 = nullptr);// : m_current(_behaviour), m_nodeMap(_nodeMap), m_colour({ 255, 255, 0, 255 }) { m_pathAgent.SetMap(_nodeMap); if (_behaviour2) {} m_current->Enter(this); }
	~Agent() { delete m_current; }	// Destructor.

	void Update(float deltaTime);	// Update decision-making process and behaviour execution.
	void Draw();	// Draw the agent.

	void GoTo(Vector2 point);	// Path to the given world coordinate.
	void GoToRandom();	// Path to a random valid node.
	void SetNode(Node* node);	// Set the path agent's starting node.
	bool PathComplete();	// Whether the agent has done travelling to its path.
	void SetSpeed(float speed);	// Sets the movement speed of the agent.
	NodeMap* GetNodeMap();	// Gets the node map this agent is assigned to.

	void SetTarget(Agent* agent);
	Agent* GetTarget();
	Vector2 GetPos();
	void SetPos(Vector2 pos);
	void SetNodePos(Vector2 pos);
	void Reset();	// Resets the path.
	void SetColour(Color colour);

private:
	PathAgent m_pathAgent;	// The current path agent this agent is assigned to.
	Behaviour* m_current;	// Points to the current behaviour this agent is in.
	NodeMap* m_nodeMap;	// Points to the node graph this agent is assigned to.
	Color m_colour;	// Colour of this agent.

	Agent* m_target;	// Multi-purpose pointer target to another agent.
};

