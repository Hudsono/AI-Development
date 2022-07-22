#pragma once

// Tools
#include "raylib.h"
#include <iostream>

// Class to emulate noise which can alert AI Agents.
class SoundSource
{
public:
	// Define publicly-usable enums for different sound types.
	// These allow agents to respond to sounds in different ways, or anticipate a particular sound
	// like a fellow guard's footsteps so as to not be alerted by them.
	enum class SoundTypes
	{
		Footstep_Boot,		// Used by guards.
		Footstep_Shoe,
		Rock,
		Steam,
		Pistol_Beretta,		// Used by guards.
		Pistol_Glock,
		Pistol_Silenced
	};
private:
	float m_duration;	// How long the sound plays for in seconds. Gunshots are quick, explosions are long. Negative duration means the sound never stops.
	float m_durationMax;	// How long the sound will play for in seconds.
	float m_volume;		// How loud the sound is. Doubles as the sound's radius.
	Vector2 m_location;	// Where the sound is coming from.
	SoundTypes m_soundType;	// The type of sound this is.

	static int AllIds;

public:
	
	int m_id;
	//SoundSource() {}	// Default constructor
	// Constructor; initialises the sound's properties.
	SoundSource(float _duration, float _volume, Vector2 _location, SoundTypes _soundType) : m_duration(_duration), m_volume(_volume), m_location(_location), m_durationMax(_duration), m_soundType(_soundType) { m_id = ++AllIds; std::cout << "sound id = " << m_id << std::endl; }
	~SoundSource() {}	// Destructor.
	
	void Update(float deltaTime);	// Update the sound's duration and visual effect.
	void Draw();	// Visualise the sound's radius.

	SoundTypes GetSoundType() { return m_soundType; }	// Gets the type of this sound.
	bool DonePlaying() { return m_duration <= 0; }	// True if we've finished playing this sound.
	float GetVolume() { return m_volume; }	// Gets the volume of this sound; the radius.
	Vector2 GetLocation() { return m_location; } // Gets the location source of this sound.

	bool operator==(const SoundSource& _other);
};

