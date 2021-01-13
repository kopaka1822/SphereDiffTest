#pragma once
#include "../framework/glmmath.h"
#include "Ray.h"

struct Sphere
{
	glm::vec3 C; // center
	float r; // radius

	glm::vec2 calcUV(glm::vec3 n)
	{
		glm::vec2 uv;
		uv[0] = (n.x == 0.0f ? 0.25f * glm::sign(n.z) : glm::atan(n.z, n.x) / (2.0f * glm::pi<float>())) + 0.5f;
		uv[1] = glm::acos(n.y) / glm::pi<float>();
		return uv;
	}

	bool operator==(const Sphere& right) const
	{
		return (C == right.C) && r == right.r;
	}
};