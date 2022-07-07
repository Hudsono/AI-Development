#pragma once

// Inheritance
#include "Behaviour.h"

// Tools
#include <vector>

class UtilityAI : public Behaviour
{
private:
	std::vector<Behaviour*> m_behaviours;	// Belongs to UtilityAI.
	Behaviour* currentBehaviour;
	bool m_roulette;	// Whether to use a roulette to spice up behaviour choosing a little.
	float m_timer;	// Timer which determines whether the current behaviour can be changed. Reset to the new behaviour's set time variable.

public:
	virtual void Update(Agent* agent, float deltaTime);
	UtilityAI(bool doRoulette) : m_roulette(doRoulette) {}	// Constructor with choice to use roulette in decisions.
	~UtilityAI();	// Destructor.

	void AddBehaviour(Behaviour* behaviour);	// Add a behaviour to be evaluated by the Utility AI.
};

