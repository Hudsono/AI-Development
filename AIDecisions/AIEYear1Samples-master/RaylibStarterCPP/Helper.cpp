#include "Helper.h"
#include <random>

// Draws an "X" with an optional circle around it.
void Helper::DrawCross(Vector2 pos, float size, float thick, Color colour, bool encircle)
{
	// Draw two lines crossing each other at the defined position, offset by the given size.
	DrawLineEx({ pos.x + size, pos.y + size }, { pos.x - size, pos.y - size }, thick, colour);
	DrawLineEx({ pos.x - size, pos.y + size }, { pos.x + size, pos.y - size }, thick, colour);
	// Draw a circle around the X if defined.
	if (encircle)
		DrawCircleLines(pos.x, pos.y, size * 1.5f, colour);
}

// Draws a dashed line.
void Helper::DrawLineDash(Vector2 startPos, Vector2 endPos, float thick, Color colour)
{

}

int Helper::GetRouletteIndex(std::vector<float> weights)
{
	float totalWeight = 0;

	for (float weight : weights)
		totalWeight += weight;

	float rng = rand() % (int)totalWeight;

	float accumilator = 0;

	for (int i = 0; i < weights.size(); i++)
	{
		accumilator += weights[i];
		if (accumilator > rng)
			return i;
	}
}

bool Helper::SameVector(Vector2 _one, Vector2 _two)
{
	return (_one.x == _two.x && _one.y == _two.y);	// Return true if relative values are identical.
}
