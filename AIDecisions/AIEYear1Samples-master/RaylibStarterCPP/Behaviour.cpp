#include "Behaviour.h"

// Forward declarations
#include "Agent.h"
#include "BH_Behaviour.h"

// Tools
#include "raymath.h"    // Distance calculation

void GotoPointBehaviour::Enter(Agent* agent)
{
    agent->SetSpeed(100);
}

void GotoPointBehaviour::Update(Agent* agent, float deltaTime)
{
    // Left-clicking an empty node will create an end point to path to.
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !IsKeyDown(KEY_LEFT_SHIFT))
    {
        agent->GoTo(GetMousePosition()); // Tell agent to pathfind to the given end point (mouse position).
    }
    else if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && IsKeyDown(KEY_LEFT_SHIFT))
    {
        agent->SetPos(GetMousePosition());  // Teleport the player to the mouse position.
    }

    if (IsKeyDown(KEY_A))
        agent->SetPos({ agent->GetPos().x - 1, agent->GetPos().y });

    if (IsKeyDown(KEY_D))
        agent->SetPos({ agent->GetPos().x + 1, agent->GetPos().y });

    if (IsKeyDown(KEY_W))
        agent->SetPos({ agent->GetPos().x, agent->GetPos().y - 1 });

    if (IsKeyDown(KEY_S))
        agent->SetPos({ agent->GetPos().x, agent->GetPos().y + 1 });

    if (IsMouseButtonDown(MOUSE_MIDDLE_BUTTON))
    {
        Vector2 mouse = GetMousePosition();
        Vector2 direction = Vector2Subtract(mouse, agent->GetPos());

        Vector2 acc = direction;

        acc = (Vector2Scale(Vector2Normalize(direction), 0.1f));

        agent->SetPos(Vector2Add(agent->GetPos(), acc));
    }
}

void WanderBehaviour::Enter(Agent* agent)
{
    // Blue when wandering
    agent->SetColour(GREEN);
    agent->SetSpeed(20);
}

void WanderBehaviour::Update(Agent* agent, float deltaTime)
{
    if (agent->PathComplete())
        agent->GoToRandom();
}

float WanderBehaviour::Evaluate(Agent* agent)
{
    Agent* target = agent->GetTarget();
    float dist = Vector2Distance(agent->GetPos(), target->GetPos());

    float eval = dist;
    if (eval < 0)
        eval = 0;
    return eval;
}

void FollowerBehaviour::Enter(Agent* agent)
{
    // Red when following
    agent->SetColour(RED);
    agent->Reset();
    agent->SetSpeed(50);
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
        else if (agent->PathComplete() && Vector2Distance(target->GetPos(), agent->GetPos()) > agent->GetNodeMap()->CellSize())
        {
            // Extra check for when an agent's path to its target moves away from the target, then later finds its target in the same spot.
            // Without this check, the agent does not move if the target was found twice without the target having moved.
            // Additional distance check to prevent constant path recalculation when close to the target (could path directly to target now).
            agent->GoTo(lastTargetPosition);
        }
    }
}

void FollowerBehaviour::Exit(Agent* agent)
{
    // If a target exists as we just exited following, record its position as a point to search.

    Agent* target = agent->GetTarget();

    if (target)
    {
        agent->SetSearchPoint(target->GetPos());
    }
}

float FollowerBehaviour::Evaluate(Agent* agent)
{
    Agent* target = agent->GetTarget();
    float dist = Vector2Distance(target->GetPos(), agent->GetPos());

    float eval = 10 * agent->GetNodeMap()->CellSize() - dist;
    if (eval < 0)
        eval = 0;
    return eval;
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

void DecisionBehaviour::Update(Agent* agent, float deltaTime)
{
    //m_decision->makeDecision(agent, deltaTime);
}

BehaviourTreeBehaviour::BehaviourTreeBehaviour(std::initializer_list<BT_Behaviour*> _BT_behaviours)
{
    // Add all the given BT Behvaiours to the root.
    for (BT_Behaviour* BTBehaviour : _BT_behaviours)
        m_BTbehaviours.push_back(BTBehaviour);
}

void BehaviourTreeBehaviour::Update(Agent* agent, float deltaTime)
{
    // Check/Update all the BT behaviours.
    for (BT_Behaviour* BTBehaviour : m_BTbehaviours)
        BTBehaviour->Check(agent, deltaTime);
}

void GuardBehaviour::Enter(Agent* agent)
{
    // Guard turns blue and walks slowly when guarding...
    agent->SetColour(BLUE);
    agent->SetSpeed(25);

    // Make sure we have guard points to use. If not, automatically save agent's current location as its sole guard point.
    if (agent->GetGuardPoints().empty())
    {
        agent->SetGuardPoints({ agent->GetNodeMap()->GetClosestNode(agent->GetPos()) });    // Adds the node closest to the agent's position as its single guard point for now.
    }

    // Clear any existing path so that it may immediately return to whatever was its last guard destination.
    agent->Reset();
}

void GuardBehaviour::Update(Agent* agent, float deltaTime)
{
    // Sets agent to guard its given guard points. Patrols if multiple guard points are defined.
    // First, check there are any points /to/ guard.
    if (agent->GetGuardPoints().empty())
        return; // If so, break out now.

    // Check if we're done pathing yet...
    if (agent->PathComplete())
        agent->GoTo(agent->GetNodeMap()->NodeWPos(agent->IncPatrolPath()));    // Path to the next available patrol destination.
}

void SearchBehaviour::Enter(Agent* agent)
{
    // Guard turns orange when searching, but retains the speed it was running at before...
    agent->SetColour(ORANGE);
}

void SearchBehaviour::Update(Agent* agent, float deltaTime)
{
    // Guard picks random points to walk to, based around the search point.
    
    // Check if we're done pathing yet...
    if (agent->PathComplete())
    {
        // Randomised X and Y numbers in world coordinates...
        int randPointX = rand() % 200 + (-100);    // Random int between -100 and 100.
        int randPointY = rand() % 200 + (-100);    // Random int between -100 and 100.


        // Pick a random coordinate based on the search point, offset by the random amount.
        Node* searchNode = agent->GetNodeMap()->GetClosestNode({ agent->GetSearchPoint().x + randPointX, agent->GetSearchPoint().y + randPointY });

        // If that node was valid, go to it.
        if (searchNode)
            agent->GoTo(agent->GetNodeMap()->NodeWPos(searchNode));
    }
}

void AttackBehaviour::Enter(Agent* agent)
{
}

void AttackBehaviour::Update(Agent* agent, float deltaTime)
{
}
