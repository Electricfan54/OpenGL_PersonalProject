#pragma once
#include <vector>
#include "../Objects/Cube.h"

struct Chunk
{
	Cube cube;
	std::vector<PhysicsObject> colliderVector;

	void DebugChangeColor()
	{
		cube.Color(Math::vector4(1.0f, 0.0f, 0.0f, 1.0f));
	}
};