#pragma once

struct Vector2
{
	float x;
	float y;
};


class Math {

public:

	static float ToDegrees(float radians) { return radians * 180.0f / 3.14159f; }
};