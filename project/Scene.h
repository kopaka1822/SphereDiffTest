#pragma once
#include "Sphere.h"
#include "Ray.h"
#include <optional>
#include <vector>

class Scene
{
public:
	Scene();
	
	// returns hit point
	std::optional<Sphere> TraceRay(RayData& ray);

private:
	std::vector<Sphere> m_spheres;
};
