#include "RayDiffRenderer.h"

float OriginalDTDX(glm::vec3 dPdx, glm::vec3 dDdx, float rayT, glm::vec3 rayDir, glm::vec3 triangleNormal)
{
	const float invDdotNg = 1.0f / dot(rayDir, triangleNormal);
	float dtdx = -(dot(dPdx + rayT * dDdx, triangleNormal)) * invDdotNg;
	return dtdx;
}

glm::vec3 OriginalTransfer(glm::vec3 dPdx, glm::vec3 dDdx, float rayT, glm::vec3 rayDir, glm::vec3 triangleNormal)
{
	glm::vec3 new_dPdx;

	float dtdx = OriginalDTDX(dPdx, dDdx, rayT, rayDir, triangleNormal);
	new_dPdx = dPdx + (rayT * dDdx) + dtdx * rayDir;
	new_dPdx = glm::clamp(new_dPdx, -1e8f, 1e8f);

	return new_dPdx;
}

float NewDTDX(glm::vec3 P /*ray origin*/, glm::vec3 D, float t, glm::vec3 C, float r,
	glm::vec3 dPdx, glm::vec3 dDdx)
{
	glm::vec3 A = P + D * dot(C - P, D) - C;
	//float t = dot(C - P, D) - sqrt(r * r - dot(A, A));

	// point on sphere and normal
	glm::vec3 p = P + D * t;
	glm::vec3 n = (p - C) / r;
	// reconstructed differential ray
	glm::vec3 rxOrigin = P + dPdx;
	glm::vec3 rxDirection = D + dDdx;

	float d = -dot(n, p);
	float tx = (-dot(n, rxOrigin) - d) / dot(n, rxDirection);

	return t - tx;
}

glm::vec3 NewTransfer(glm::vec3 P /*ray origin*/, glm::vec3 D, float t, glm::vec3 C, float r,
	glm::vec3 dPdx, glm::vec3 dDdx)
{
	glm::vec3 A = P + D * dot(C - P, D) - C;
	//float t = dot(C - P, D) - sqrt(r * r - dot(A, A));

	// point on sphere and normal
	glm::vec3 p = P + D * t;
	glm::vec3 n = (p - C) / r;
	// reconstructed differential ray
	glm::vec3 rxOrigin = P + dPdx;
	glm::vec3 rxDirection = D + dDdx;

	float d = -dot(n, p);
	float tx = (-dot(n, rxOrigin) - d) / dot(n, rxDirection);
	glm::vec3 px = rxOrigin + tx * rxDirection;

	glm::vec3 dpdx = px - p;
	return dpdx;
}

bool SolveLinearSystem2x2(glm::mat2 A, glm::vec2 B, glm::vec2& x);

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

		if(attrib == Attribute::Depth)
		{
			float dt;
			if (m_newTransfer) dt = NewDTDX(ray.origin, ray.direction, ray.t, sphere.C, sphere.r, glm::vec3(0.0f), dDdx);
			else dt = OriginalDTDX(glm::vec3(0.0f), dDdx, ray.t, ray.direction, n);
			dst.set(glm::vec3(dt), x, y);
			continue;
		}
		
		// first test: transfer ray for dOdX
		glm::vec3 dPdx;
		if (m_newTransfer) dPdx = NewTransfer(ray.origin, ray.direction, ray.t, sphere.C, sphere.r, glm::vec3(0.0f), dDdx);
		else dPdx = OriginalTransfer(glm::vec3(0.0f), dDdx, ray.t, ray.direction, n);
		
		if(attrib == Attribute::Position)
		{
			dst.set(dPdx, x, y);
			continue;
		}
		// TODO calc dNdx

		if(attrib == Attribute::Texcoord)
		{
			const glm::vec2 uv = sphere.calcUV(n);
			const auto pi = glm::pi<float>();
				
			int dim[2];
			if (abs(n.x) > abs(n.y) && abs(n.x) > abs(n.z)) {
				dim[0] = 1; dim[1] = 2;
			}
			else if (abs(n.y) > abs(n.z)) {
				dim[0] = 0; dim[1] = 2;
			}
			else {
				dim[0] = 0; dim[1] = 1;
			}

			// change of position per texture coordinate direction (UV-sphere)
			float uOffset = pi;
			const glm::vec3 dpdu = glm::vec3(
				-sin(pi * uv.y) * 2.0f * pi * sin(2.0f * pi * uv.x + uOffset),
				0.0f,
				sin(pi * uv.y) * 2.0f * pi * cos(2.0f * pi * uv.x + uOffset)) * sphere.r;
			const glm::vec3 dpdv = glm::vec3(
				pi * cos(pi * uv.y) * cos(2.0f * pi * uv.x + uOffset),
				-pi * sin(pi * uv.y),
				pi * cos(pi * uv.y) * sin(2.0f * pi * uv.x + uOffset)) * sphere.r;

			glm::mat2 A;
			A[0][0] = dpdu[dim[0]]; A[0][1] = dpdv[dim[0]];
			A[1][0] = dpdu[dim[1]]; A[1][1] = dpdv[dim[1]];

			glm::vec2 Bx = glm::vec2(dPdx[dim[0]], dPdx[dim[1]]);

			glm::vec2 dUVdx;
			if (!SolveLinearSystem2x2(A, Bx, dUVdx)) dUVdx = glm::vec2(0.0);

			dst.set(glm::vec3(dUVdx.x, dUVdx.y, 0.0f), x, y);
			continue;
		}
	}
}

bool SolveLinearSystem2x2(glm::mat2 A, glm::vec2 B, glm::vec2& x) {
	float det = A[0][0] * A[1][1] - A[0][1] * A[1][0];
	if (abs(det) < 1e-10f)
		return false;
	x[0] = (A[1][1] * B[0] - A[0][1] * B[1]) / det;
	x[1] = (A[0][0] * B[1] - A[1][0] * B[0]) / det;
	if (isnan(x[0]) || isnan(x[1]))
		return false;
	return true;
}