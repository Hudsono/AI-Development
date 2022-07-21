#pragma once

// Dependencies.
#include "Pathfinding.h"

// Forward declarations.
class Behaviour;
class BlackBoard;

// Structure to hold data about looking at things.
struct ViewTarget
{
	Vector2 target;	// Where to look.
	int timer;	// How long to look at it.
	int importance;	// How important is this location to look at? Controlled by guard suspicion.
};

// AI Agent dedicated to making logical decisions.
class Agent
{
public:
	Agent() {}	// Base constructor.

	// Constructor to assign a node map, starting behaviour and blackboard.
	Agent(NodeMap* _nodeMap, Behaviour* _behaviour, BlackBoard* _blackBoard);// : m_current(_behaviour), m_nodeMap(_nodeMap), m_colour({ 255, 255, 0, 255 }) { m_pathAgent.SetMap(_nodeMap); if (_behaviour2) {} m_current->Enter(this); }
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
	void SetGuardPoints(std::vector<Node*> _guardPoints) { m_patrolPoints = _guardPoints; }	// Sets this agent's guard points to the given nodes.
	std::vector<Node*> GetGuardPoints() { return m_patrolPoints; }	// Returns the agent's guard points.
	Node* IncPatrolPath();	// Returns the next node of the agent's patrol path and increases/resets the patrol progress.

	// Metrics for Guards. 0-100 //
	// Personality
	int m_experience;	// Controls how well the guard responds to duty-related actions.						Calling for backup
	int m_familiarity;	// Controls how well the guard responds to location-related actions.					Searching "known" hiding spots in an area, quicker to spot smaller changes in environment
	int m_confidence;	// Controls how well the guard responds to investigative and combat-related actions.	Calling for backup/sounding alarms
	// Context
	int m_suspicion;	// How suspicious the guard is currently. Controls movement speed and length of searching. Is controlled by guard personality.

	/*
	* A Guard with high Experience EXP will make more accurate guesses in searches and is able to ...
	* A Guard with high Familiarity FAM will make better judgements in what areas to investigate and what actions are considered suspicious.
	* A Guard with high Confidence CON will pursue their target for longer and venture further and longer to search for potential threats.
	* 
	* Example metric combinations:
	* Guard A: low EXP, FAM and CON
	*  - Makes horrible guesses for searching, often misses queues to begin searching/become suspicious (seeing things move in the distance/hearing quick noises). 
	*  - Is not aware of what is or isn't normal for the area they're patrolling.
	*  - Does not chase for very long and is reluctant to search long distances or long times.
	* 
	* Guard B: low EXP, FAM but high CON
	*  - Confidence is a slow and insideous killer. Bone-headed.
	*  - 
	*/
	void SetSearchPoint(Vector2 _searchPoint) { m_searchPoint = _searchPoint; }	// Sets the agent's search point.
	Vector2 GetSearchPoint() { return m_searchPoint; }	// Gets the agent's search point.


	void AddViewTarget(Vector2 _viewTarget);	// Add somewhere to look at.

private:
	PathAgent m_pathAgent;	// The current path agent this agent is assigned to.
	Behaviour* m_current;	// Points to the current behaviour this agent is in.
	NodeMap* m_nodeMap;	// Points to the node graph this agent is assigned to.
	Color m_colour;	// Colour of this agent.

	float m_rotation;	// The angle this agent is facing.
	Vector2 m_viewTarget;	// Where this agent is looking.

	Agent* m_target;	// Multi-purpose pointer target to another agent.

	std::vector<Node*> m_patrolPoints;	// Sets this agent to patrol the given points when idling. One defined point is guarded; several are patrolled through.
	int m_patrolProgress;	// At what index of patrol points we're currently at.
	Vector2 m_searchPoint;	// The last point of suspicioun for the agent: either the last time they saw their target, or the source of a sound.

	BlackBoard* m_blackBoard;	// The blackboard this agent will review.
};

