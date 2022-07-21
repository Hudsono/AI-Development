#pragma once

// Tools.
#include "raylib.h"
#include <vector>

// Forward declarations.
class Agent;
class SoundSource;
class GameManager;

// Simple Blackboard listing important information for Guard agents.
// For now, also acts to collate manager-like information.
class BlackBoard
{
public:
	BlackBoard(GameManager* _gameManager) : m_gameManager(_gameManager) {}	// Base constructor.
	~BlackBoard() {}	// Destructor.

	std::vector<Agent*> m_agents;
	std::vector<SoundSource*> m_soundSources;

	GameManager* m_gameManager;	// Points to the game manager in use. For playing sounds + other uses.
};

