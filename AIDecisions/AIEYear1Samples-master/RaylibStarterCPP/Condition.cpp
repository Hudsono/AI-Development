#include "Condition.h"

// Forward declarations.
#include "Agent.h"
#include "GameManager.h"
#include "SoundEmitter.h"

// Tools
#include "raymath.h"

bool DistanceCondition::IsTrue(Agent* agent, float deltaTime)
{
    return (Vector2Distance(agent->GetPos(), agent->GetTarget()->GetPos()) < m_distance) == m_inside;
}

bool TimeCondition::IsTrue(Agent* agent, float deltaTime)
{
    // If we've started searching, count down. Otherwise, wait until we have reached our initial search point.
    if (m_startedSearch)
    {
        m_time -= 1 * deltaTime;
        return m_time <= 0;   // Decrements timer--checks if it's 
    }
    else if (!m_startedSearch && agent->PathComplete())
    {
        m_startedSearch = true;
    }
    return false;
}

void TimeCondition::Enter(Agent* agent)
{
    m_time = m_maxTime; // Resets timer.
    m_startedSearch = false;    // Let it know it has yet to start its search.
}

bool SoundCondition::IsTrue(Agent* agent, float deltaTime)
{
    // Loop through all sounds in the given container pointer to check for.
    for (SoundSource* sound : *m_AllSoundsPtr)
    {
        // If the agent is within the radius of the sound, return true and set it as the agent's search point.
        if ((Vector2Distance(agent->GetPos(), sound->GetLocation()) < sound->GetVolume()))
        {
            agent->Reset(); // Stop the agent's current path.
            agent->SetSearchPoint(sound->GetLocation());    // Once the guard transitions to searching, they will start at the sound's source.
            // TODO: based on the guard's EXP and FAM and the guard's distance from the sound's source, the guard will make a more-or-less accurate guess on where to start searching.
            return true;
        }
    }

    // Otherwise, the agent heard nothing.
    return false;
}
