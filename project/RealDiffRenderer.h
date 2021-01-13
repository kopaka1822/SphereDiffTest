#pragma once
#include "GBufferRenderer.h"
#include "RenderAttributes.h"
#include "Scissor.h"

class RealDiffRenderer
{
public:
	RealDiffRenderer() = default;
	void update(const Pixels<GBuffer>& gbuffer, const Pixels<glm::vec3>& src, Pixels<glm::vec3>& dst, Scissor s);
};