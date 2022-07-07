#include "Helper.h"

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