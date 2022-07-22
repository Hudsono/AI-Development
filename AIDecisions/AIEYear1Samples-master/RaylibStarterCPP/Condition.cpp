#include "Condition.h"

// Forward declarations.
#include "Agent.h"
#include "GameManager.h"
#include "SoundEmitter.h"

// Tools
#include "raymath.h"
#include <algorithm>

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
        // Also check that the agent hasn't already heard this sound before and considered it suspicious...

        std::vector<SoundSource*> allsounds = (*m_AllSoundsPtr);

        //std::vector<SoundSource*>::iterator it = std::find((allsounds.begin(), allsounds.end(), sound));

        bool foundSameSound = false;

        for (int i = 0; i < allsounds.size(); i++)
        {
            if (sound->m_id == allsounds[i]->m_id)
            {
                foundSameSound = true;
                //std::cout << "FOUND " << std::endl;
            }
            else
            {
                std::cout << "-----> NOT FOUND" << std::endl;
                foundSameSound = false;
                FAIL HERE
            }
        }

        if ((Vector2Distance(agent->GetPos(), sound->GetLocation()) < sound->GetVolume()) && !foundSameSound)
        {
            agent->Reset(); // Stop the agent's current path.
            agent->SetSearchPoint(sound->GetLocation());    // Once the guard transitions to searching, they will start at the sound's source.
            // TODO: based on the guard's EXP and FAM and the guard's distance from the sound's source, the guard will make a more-or-less accurate guess on where to start searching.
           
            // Add this sound source to the guard's list of "heard" sounds so they don't check it again.
            agent->AddSusSound(sound);

            return true;
        }
    }

    // Otherwise, the agent heard nothing.
    return false;
}
