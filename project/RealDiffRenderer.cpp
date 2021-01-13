#include "RealDiffRenderer.h"

void RealDiffRenderer::update(const Pixels<GBuffer>& gbuffer, const Pixels<glm::vec3>& src, Pixels<glm::vec3>& dst, Scissor s)
{
	for(int y = s.ystart; y < s.yend; ++y) for(int x = s.xstart; x < s.xend; ++x)
	{
		// calc dx
		auto rightBuf = gbuffer.get(x + 1, y);
		auto midBuff = gbuffer.get(x, y);
		bool rightOk = rightBuf.hit == midBuff.hit;
		
		if (rightOk)
		{
			auto mid = src.get(x, y);
			auto right = src.get(x + 1, y);
			dst.set(right - mid, x, y);
		}
		else dst.set(glm::vec3(0.0f), x, y);
	}
}
