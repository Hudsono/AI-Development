#include "SoundEmitter.h"

void SoundSource::Update(float deltaTime)
{
	m_duration -= 1 * deltaTime;	// Count down the sound's duration.
}

void SoundSource::Draw()
{
	// Draw the full radius of the sound as a circle.
	DrawCircleLines(m_location.x, m_location.y, m_volume, WHITE);

	// Draw 3 grey circles rippling from the centre to illustrate sound waves.
	for (int i = 0; i < 3; i++)
	{
		int ripInterval = m_volume / 5;	// Space between each ring. Proportional to size of sound.

		// (current volume - max volume + offset) % max volume
		// Creates the effect of the rings starting small and expanding outwards over time, wrapping back to being small when reaching the ring's full radius.
		// The multiplications are proportional to the sound's volume/size, meaning the effect scales to its size.
		// * m_volume is the speed of the ripple, i*ripInterval is the space between each ring ripple and the end modulo expression is the max size controlling the rings wrapping back.
		int rippleRadius = ((int)((m_duration * m_volume) - (m_durationMax * m_volume)) + (i*ripInterval)) % (int)(m_volume);
		DrawCircleLines(m_location.x, m_location.y, rippleRadius, GRAY);
	}
}
