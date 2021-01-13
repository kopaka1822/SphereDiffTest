#include "RealDiffRenderer.h"

void RealDiffRenderer::update(const Pixels<glm::vec3>& src, Pixels<glm::vec3>& dst, Scissor s)
{
	for(int y = s.ystart; y < s.yend; ++y) for(int x = s.xstart; x < s.xend; ++x)
	{
		// calc dx
		auto left = src.get(x - 1, y);
		auto right = src.get(x + 1, y);
		dst.set((right - left) * 0.5f, x, y);
	}
}
