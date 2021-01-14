#pragma once
#include "GBufferRenderer.h"
#include "RenderAttributes.h"
#include "Scissor.h"

class RayDiffRenderer
{
public:
	RayDiffRenderer() = default;
	void update(const Pixels<GBuffer>& gbuffer, const Pixels<glm::vec3>& src, Pixels<glm::vec3>& dst, Scissor s, Attribute attrib);
	void setNewTransfer(bool v) { m_newTransfer = v; }
	bool getNewTransfer() const { return m_newTransfer; }
private:
	bool m_newTransfer = true;
};