#include "BT_BehaviourCustoms.h"

// Forward declarations
#include "Agent.h"

// Tools
#include "raymath.h"

Status ConditionTargetClose::Update(Agent* agent, float deltaTime)
{
	// Gets the positions of the agent and its target; returns Successful if it's within the defined radius of this condition instance.
	if (Vector2Distance(agent->GetPos(), agent->GetTarget()->GetPos()) < m_radius)
		return Status::Success;
	else
		return Status::Failure;
}

Status ActionChase::Update(Agent* agent, float deltaTime)
{
    // Check if the agent has moved significantly from its last position (a whole node cell)
    // If so, recalculate a path to it.
    Agent* target = agent->GetTarget();

    if (target)
    {
        float dist = Vector2Distance(target->GetPos(), m_targetLastPos);

        // Update the last target position and path to it only if the distance from the last valid calculation was large enough
        if (dist >= agent->GetNodeMap()->CellSize())
        {
            m_targetLastPos = target->GetPos();
            agent->GoTo(m_targetLastPos);
        }
        else if (agent->PathComplete() && Vector2Distance(target->GetPos(), agent->GetPos()) > agent->GetNodeMap()->CellSize())
        {
            // Extra check for when an agent's path to its target moves away from the target, then later finds its target in the same spot.
            // Without this check, the agent does not move if the target was found twice without the target having moved.
            // Additional distance check to prevent constant path recalculation when close to the target (could path directly to target now).
            agent->GoTo(m_targetLastPos);
        }
    }

    return Status::Success; // Return Success regardless.
}

Status ActionWander::Update(Agent* agent, float deltaTime)
{
    // Pick a random node to path to. Once that path has complete, pick another random node.
    if (agent->PathComplete())
        agent->GoToRandom();

    return Status::Success; // Return Success regardless.
}
