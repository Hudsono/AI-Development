#pragma once

class Agent;

class Condition
{
public:
	// Pure virtual function set to nullptr. Abstract; uninstantiable.
	virtual bool IsTrue(Agent* agent) = 0;
};

class DistanceCondition : public Condition
{
private:
	float m_distance;
	bool m_lessThan;

public:
	DistanceCondition(float d, bool lt) : m_distance(d), m_lessThan(lt) {}
	virtual bool IsTrue(Agent* agent);
};