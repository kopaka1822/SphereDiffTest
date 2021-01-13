#include "RayDiffRenderer.h"

glm::vec3 OriginalTransfer(glm::vec3 dPdx, glm::vec3 dDdx, float rayT, glm::vec3 rayDir, glm::vec3 triangleNormal)
{
	glm::vec3 new_dPdx;
	const float invDdotNg = 1.0 / dot(rayDir, triangleNormal);

	float dtdx = -(dot(dPdx + rayT * dDdx, triangleNormal)) * invDdotNg;
	new_dPdx = dPdx + (rayT * dDdx) + dtdx * rayDir;
	new_dPdx = glm::clamp(new_dPdx, -1e8f, 1e8f);

	return new_dPdx;
}

void RayDiffRenderer::update(const Pixels<GBuffer>& gbuffer, const Pixels<glm::vec3>& src, Pixels<glm::vec3>& dst, Scissor s, Attribute attrib)
{
	for (int y = s.ystart; y < s.yend; ++y) for (int x = s.xstart; x < s.xend; ++x)
	{
		// calc dx
		auto rightBuf = gbuffer.get(x + 1, y);
		auto midBuff = gbuffer.get(x, y);
		
		bool rightOk = rightBuf.hit == midBuff.hit;
		glm::vec3 refDiff = glm::vec3(0.0f); // for debugging
		
		if (rightOk)
		{
			auto mid = src.get(x, y);
			auto right = src.get(x + 1, y);
			refDiff = right - mid;
		}
		if(!rightOk || midBuff.hit.index() != 1) // skip this for now
		{
			dst.set(glm::vec3(0.0f), x, y);
			continue;
		}

		auto ray = midBuff.ray;
		auto xRay = rightBuf.ray;

		Sphere sphere = std::get<1>(midBuff.hit);
		auto dDdx = xRay.direction - ray.direction;
		auto p = ray.origin + ray.t * ray.direction;
		auto n = (p - sphere.C) / sphere.r;
		
		// first test: transfer ray for dOdX
		auto dPdx = OriginalTransfer(glm::vec3(0.0f), dDdx, ray.t, ray.direction, n);

		if(attrib == Attribute::Position)
		{
			dst.set(dPdx, x, y);
			continue;
		}
	}
}
