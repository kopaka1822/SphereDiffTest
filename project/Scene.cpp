#include "Scene.h"

static float sign(float num)
{
	return num <= 0 ? 1 : -1.0f;
}

Scene::Scene()
{
	Sphere s;
	s.C = glm::vec3(0.0f, 0.0f, 3.0f);
	s.r = 1.0f;
	m_spheres.push_back(s);

	s.C = glm::vec3(0.0f, -3.0f, 3.0f);
	s.r = 2.0f;
	m_spheres.push_back(s);
}

std::optional<Sphere> Scene::TraceRay(RayData& ray)
{
	std::optional<Sphere> closest;
	
	for(const auto& s : m_spheres)
	{
		// ray tracing gems ray sphere intersection:
		glm::vec3 f = ray.origin - s.C;
		float b = -glm::dot(f, ray.direction);
		glm::vec3 l = (f + b * ray.direction);
		float tri = s.r * s.r - glm::dot(l, l);
		if (tri < 0) continue; // miss
		float c = glm::dot(f, f) - s.r * s.r;
		float q = b + sign(b) * glm::sqrt(tri);
		float t0 = c / q;
		float t1 = q;

		if(t0 > 0.0f && t0 < ray.t)
		{
			ray.t = t0;
			closest = s;
		}
		if(t1 > 0.0f && t1 < ray.t)
		{
			ray.t = t1;
			closest = s;
		}
	}

	return closest;
}
