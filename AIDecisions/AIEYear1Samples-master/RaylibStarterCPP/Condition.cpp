#include "Condition.h"

#include "raymath.h"

#include "Agent.h"

bool DistanceCondition::IsTrue(Agent* agent)
{
    return (Vector2Distance(agent->GetPos(), agent->GetTarget()->GetPos()) < m_distance) == m_lessThan;
}
