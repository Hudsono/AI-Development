#include "UtilityAI.h"

// Tools
#include "Helper.h"
#include <iostream>

void UtilityAI::Update(Agent* agent, float deltaTime)
{
	// First, count down the timer. Reset it to 0 if necessary.
	if (m_timer > 0)
		m_timer -= deltaTime;
	else if (m_timer < 0)
		m_timer = 0;

	//std::cout << m_timer << std::endl;

	float bestEval = 0;	// The highest-scoring evaluation.
	Behaviour* newBehaviour = nullptr;	// The behaviour to transition to (if not current).

	std::vector<float> allEvals;	// Store evaluation results for roulette.

	// Evaluate all behaviours to either choose the best option or use them in a roulette.
	for (Behaviour* behaviour : m_behaviours)
	{
		float eval = behaviour->Evaluate(agent);	// Evaluate the current behaviour.

		// If we aren't using a roulette, choose only the best-evaluated option.
		// Otherwise, accumilate all evaluations and later run the roulette with them.
		if (!m_roulette)
		{
			// Found a better evaluaiton--this behaviour becomes the new behaviour.
			if (eval > bestEval)
			{
				bestEval = eval;
				newBehaviour = behaviour;
			}
		}
		else
		{
			allEvals.push_back(eval);	// Record all evaluation results for the roulette.
		}
	}

	// If we're using a roulette, decide on the behaviour based on the output evaluations as weights
	// Semi-random: still leans towards the best option (but doesn't always pick the best option).
	if (m_roulette)
		newBehaviour = m_behaviours[Helper::GetRouletteIndex(allEvals)];

	// If we have a candidate, different behaviour, then exit the current behaviour and initialise the new one.
	if (newBehaviour != nullptr && newBehaviour != currentBehaviour && m_timer == 0)
	{
		if (currentBehaviour)
			currentBehaviour->Exit(agent);
		currentBehaviour = newBehaviour;
		currentBehaviour->Enter(agent);
		m_timer = currentBehaviour->ResetTimer();	// Reset the minimum-change timer as per the now-current behaviour.
	}

	// Update only the current behaviour.
	currentBehaviour->Update(agent, deltaTime);
}

UtilityAI::~UtilityAI()
{
	// All behaviours belong to this class--clean them up upon removal.
	for (Behaviour* behaviour : m_behaviours)
		delete behaviour;
}

void UtilityAI::AddBehaviour(Behaviour* behaviour)
{
	m_behaviours.push_back(behaviour);	// Add the given behaviour to list of possible behaviours that will be evaluated.
}
