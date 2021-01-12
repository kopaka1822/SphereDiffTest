#include "AttributeRenderer.h"

void AttributeRenderer::update(const Pixels<GBuffer>& src)
{
	if(src.getWidth() != m_pixels.getWidth() || src.getHeight() != m_pixels.getHeight())
	{
		m_pixels = Pixels<glm::vec3>(src.getWidth(), src.getHeight());
	}

	auto s = src.begin();
	auto d = m_pixels.begin();

	while(s != src.end())
	{
		*d = glm::vec3(s->ray.direction.y * 0.5 + 0.5); //* 0.5f + glm::vec3(0.5f);


		
		++s;
		++d;
	}
}
