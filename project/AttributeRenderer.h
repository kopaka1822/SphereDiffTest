#pragma once
#include "GBufferRenderer.h"
#include "RenderAttributes.h"

class AttributeRenderer
{
public:
	AttributeRenderer() = default;
	void update(const Pixels<GBuffer>& src, Attribute attrib);
	
	const Pixels<glm::vec3>& getPixels() const { return m_pixels; }
private:
	Pixels<glm::vec3> m_pixels;
};