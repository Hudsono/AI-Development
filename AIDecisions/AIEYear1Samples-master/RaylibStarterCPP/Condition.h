#pragma once

// Tools
#include <vector>

// Forward declarations.
class Agent;
class SoundSource;
enum class SoundTypes;
class GameManager;



// Base abstract Condition class; to be overridden.
class Condition
{
public:
	virtual bool IsTrue(Agent* agent, float deltaTime) = 0;		// Pure virtual function set to nullptr. Abstract; uninstantiable.
	virtual void Enter(Agent* agent) {};		// Empty optional function to execute logic on first checking the condition, like resetting a timer.
};

// Condition checks whether the distance of this agent's target is inside/outside the given radius.
class DistanceCondition : public Condition
{
private:
	float m_distance;	// The distance to check.
	bool m_inside;	// If true, target must be inside the radius for the condition to return true; if false, target must be outside the radius.

public:
	DistanceCondition(float _distance, bool _inside) : m_distance(_distance), m_inside(_inside) {}
	virtual bool IsTrue(Agent* agent, float deltaTime);
};

// Condition counts down from the given time and reports true upon reaching/preceding 0.
class TimeCondition : public Condition
{
private:
	float m_maxTime;	// Records the time this condition is set to start counting down from.
	float m_time;		// How far this time condition has counted down to so far.
	bool m_startedSearch;	// If the agent has reached its first search point to begin searching. This controls starting timer countdown. Otherwise, timer can run out before agent starts searching.

public:
	TimeCondition(float _time) : m_time(_time), m_maxTime(_time), m_startedSearch(false) {}
	virtual bool IsTrue(Agent* agent, float deltaTime);
	virtual void Enter(Agent* agent);	// Resets condition for beginning the search to start the timer.
};

// Condition checks if this agent is within a SoundSource's radius/volume; whether the agent can "hear" it.
class SoundCondition : public Condition
{
private:
	std::vector<SoundSource*>* m_AllSoundsPtr;	// Points to the vector containing all sounds to check for.

public:
	SoundCondition(std::vector<SoundSource*>* _AllSoundsPtr) : m_AllSoundsPtr(_AllSoundsPtr) {}
	virtual bool IsTrue(Agent* agent, float deltaTime);
};