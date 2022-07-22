#include "GameManager.h"

// Forward declarations.
#include "Agent.h"
#include "SoundEmitter.h"

void GameManager::Update(float deltaTime)
{
	// Update all agents
	for (int i = 0; i < AllAgents.size(); i++)
	{
		AllAgents[i]->Update(deltaTime);
	}

	// Update all sounds
	for (int i = 0; i < AllSounds.size(); i++)
	{
		AllSounds[i]->Update(deltaTime);

		// If this sound is done playing, remove it.
		if (AllSounds[i]->DonePlaying())
		{
			std::swap(AllSounds[i], AllSounds.back());	// Put this sound at the back of the list...
			AllSounds.pop_back();	// ...then delete the last elemenet.
		}
	}

	// Modifying a range-based loop while it's still going produces unexpected behaviour. Below loop would delete all sounds upon one expiring, or only update the first sound exponetially.
	//for (SoundSource* sound : AllSounds)
	//{
	//	sound->Update(deltaTime);
	//
	//	// If this sound is done playing, remove it.
	//	if (sound->DonePlaying())
	//	{
	//		std::swap(sound, AllSounds.back());	// Put this sound at the back of the list...
	//		AllSounds.pop_back();	// ...then delete the last elemenet.
	//	}
	//}
}

void GameManager::Draw()
{
	// Draw all agents
	for (Agent* agent : AllAgents)
		agent->Draw();

	// Draw all sounds
	for (SoundSource* sound : AllSounds)
		sound->Draw();
}

void GameManager::AddAgent(Agent* _agent)
{
	AllAgents.push_back(_agent);	// Add the new agent.
}

void GameManager::AddAgent(std::initializer_list<Agent*> _agents)
{
	// Add all the given agents.
	for (Agent* agent : _agents)
		AddAgent(agent);
}

void GameManager::PlaySound(SoundSource* _sound)
{
	AllSounds.push_back(_sound);	// Add the given sound effect.
}
