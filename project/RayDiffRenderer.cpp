#include "RayDiffRenderer.h"

void RayDiffRenderer::update(const Pixels<GBuffer>& src)
{
	if (src.getWidth() != m_pixels.getWidth() || src.getHeight() != m_pixels.getHeight())
	{
		m_pixels = Pixels<glm::vec3>(src.getWidth(), src.getHeight());
	}

	
}
