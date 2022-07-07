#pragma once

// Tools
#include "raylib.h"
#include "raymath.h"

class Helper
{
public:
	// Draws an "X" with an optional circle around it.
	static void DrawCross(Vector2 pos, float size, float thick, Color colour, bool encircle);

	// Draws a dashed line.
	static void DrawLineDash(Vector2 startPos, Vector2 endPos, float thick, Color colour);
};

