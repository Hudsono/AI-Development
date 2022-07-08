#include "BH_Behaviour.h"

Status BT_Behaviour::Check(Agent* agent, float deltaTime)
{
    // Execute logic for first checking this node ONLY if it isn't already running.
    if (m_status != Status::Running)
        StartCheck();

    m_status = Update(agent, deltaTime);    // Execute this behaviour's logic, get its status from it.

    return m_status;    // Return the status given back to the parent node.
}

BT_Composite::BT_Composite(std::initializer_list<BT_Behaviour*> _behaviours)
{
    // Add all given behaviours to this composite's children.
    for (BT_Behaviour* BTBehaviour : _behaviours)
        m_children.push_back(BTBehaviour);
}

void BT_Composite::AddNode(BT_Behaviour* _node)
{
    // Add one child to this composite node.
    m_children.push_back(_node);
}

Status BT_Sequence::Update(Agent* agent, float deltaTime)
{
    // First child to not report successful has its status returned to the parent node. (Failure or Running)
    for (BT_Behaviour* child : m_children)
    {
        Status status = child->Check(agent, deltaTime);
        if (status != Status::Success)
            return status;
    }

    return Status::Success; // All children were successful; report back as successful.
}

Status BT_Selector::Update(Agent* agent, float deltaTime)
{
    // First child to not report failure has its status returned to the parent node. (Success or Running)
    for (BT_Behaviour* child : m_children)
    {
        Status status = child->Check(agent, deltaTime);
        if (status != Status::Failure)
            return status;
    }

    return Status::Failure; // All children failed; report back as failed.
}
