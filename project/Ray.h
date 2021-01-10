#pragma once
#include "../framework/glmmath.h"

struct RayData
{
	glm::vec3 origin;
	glm::vec3 direction;
	float t; // intersection t
};