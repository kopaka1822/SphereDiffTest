#pragma once
#include "GBufferRenderer.h"

enum class Attribute
{
	Depth,
	Position,
	Normal,
	Texcoord,
};

class AttributeRenderer
{
public:
	AttributeRenderer() = default;
	void update(const Pixels<GBuffer>& src);
	
	const Pixels<glm::vec3>& getPixels() const { return m_pixels; }
	Attribute getAttribute() const { return m_attrib; }
	void setAttribute(Attribute a) { m_attrib = a; }
private:
	Pixels<glm::vec3> m_pixels;
	Attribute m_attrib = Attribute::Texcoord;
};