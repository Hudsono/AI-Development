#pragma once

// Tools.
#include "raylib.h"
#include <vector>

// Forward declarations.
class Agent;
class SoundSource;

// Functions and data for the entire game.
class GameManager
{
private:
	std::vector<Agent*> AllAgents;
	std::vector<SoundSource*> AllSounds;

public:
	GameManager() {}	// Base constructor.
	~GameManager() {}	// Destructor.

	void Update(float deltaTime);	// Update all entities.
	void Draw();	// Draw all entities.

	void AddAgent(Agent* _agent);	// Add an agent to the game.
	void AddAgent(std::initializer_list<Agent*> _agents);	// Add a list of agents to the game.

	void PlaySound(SoundSource* _sound);	// Plays a sound in the game.

	std::vector<SoundSource*>* GetSounds() { return &AllSounds; }	// Gets the list of sounds currently playing.
};

