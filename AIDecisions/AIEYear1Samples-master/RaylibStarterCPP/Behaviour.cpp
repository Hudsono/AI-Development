#include "Behaviour.h"
#include "Agent.h"

#include "raymath.h"    // Distance calculation

void GotoPointBehaviour::Update(Agent* agent, float deltaTime)
{
    // Left-clicking an empty node will create an end point to path to.
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
    {
        agent->GoTo(GetMousePosition()); // Tell agent to pathfind to the given end point (mouse position).
    }
}

void WanderBehaviour::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete())
        agent->GoToRandom();
}

void FollowerBehaviour::Update(Agent* agent, float deltaTime)
{
    // Check if the agent has moved significantly from its last position (a whole node cell)
    // If so, recalculate a path to it.
    Agent* target = agent->GetTarget();

    if (target)
    {
        float dist = Vector2Distance(target->GetPos(), lastTargetPosition);

        // Update the last target position and path to it only if the distance from the last valid calculation was large enough
        if (dist >= agent->GetNodeMap()->CellSize())
        {
            lastTargetPosition = target->GetPos();
            agent->GoTo(lastTargetPosition);
        }
        //else if (Vector2Distance(agent->GetPos(), agent->GetTarget()->GetPos()) <= agent->GetNodeMap()->CellSize() * 5)
        //{
        //    agent->GoTo(target->GetPos());
        //}
    }
}

void SelectorBehaviour::Update(Agent* agent, float deltaTime)
{
    if (Vector2Distance(agent->GetPos(), agent->GetTarget()->GetPos()) <= agent->GetNodeMap()->CellSize() * 100)
    {
        SetBehaviour(m_b1, agent);
        agent->SetColour(RED);
    }
    else
    {
        // Wander
        
        // Record whether we were just following a point.
        Vector2 lastFollowing = { 0, 0 };
        bool wasFollowing = false;

        // If we're about to change states but we didn't complete the last path to the target, record that target's current position for later so we can search it out.
        if (m_selected != m_b2 && !agent->PathComplete())
        {
            wasFollowing = true;
            lastFollowing = agent->GetTarget()->GetPos();
        }

        SetBehaviour(m_b2, agent);

        // If we did have an incomplete target search, make sure we finish that before wandering randomly.
        // This is good when the agent's path to the target takes it /away/ from the target, causing the distance check to change to wander w/o finishing searching.
        if (wasFollowing)
        {
            agent->GoTo(lastFollowing);
        }

        agent->SetColour(BLUE);
    }
    m_selected->Update(agent, deltaTime);
}

void SelectorBehaviour::SetBehaviour(Behaviour* b, Agent* agent)
{
    if (m_selected != b)
    {
        m_selected = b;
        agent->Reset();
    }
}
