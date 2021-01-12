#include "AttributeRenderer.h"

void AttributeRenderer::update(const Pixels<GBuffer>& src)
{
	if(src.getWidth() != m_pixels.getWidth() || src.getHeight() != m_pixels.getHeight())
	{
		m_pixels = Pixels<glm::vec3>(src.getWidth(), src.getHeight());
	}

	auto s = src.begin();
	auto d = m_pixels.begin();

	glm::vec3 minValue = glm::vec3(std::numeric_limits<float>::max());
	glm::vec3 maxValue = glm::vec3(std::numeric_limits<float>::min());
	
	while(s != src.end())
	{
		*d = glm::vec3(0.0f);
		switch(s->hit.index())
		{
		case 0: // miss
			
			break;

		case 1: // sphere
			*d = glm::vec3(s->ray.t);
			break;
		}
		//*d = glm::vec3(s->ray.direction.y * 0.5 + 0.5); //* 0.5f + glm::vec3(0.5f);

		minValue = glm::min(minValue, *d);
		maxValue = glm::max(maxValue, *d);
		
		++s;
		++d;
	}

	// scale data based on min and max
	float offset = std::min({ minValue.x, minValue.y, minValue.z });
	float scale = 1.0f / (std::max({ maxValue.x, maxValue.y, maxValue.z }) - offset);

	for(auto& p : m_pixels)
	{
		p = (p - offset) * scale;
	}
}
