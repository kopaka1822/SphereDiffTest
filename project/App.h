#pragma once
#include <optional>

#include "../framework/Window.h"
#include <script/ScriptEngine.h>


#include "AttributeRenderer.h"
#include "GBufferRenderer.h"
#include "RealDiffRenderer.h"

enum class DisplayMode
{
	Reference,
	Diff
};

class App final : public script::ScriptObject
{
	App();
public:

	static App& get();

	Attribute getAttribute() const { return m_attrib; }
	void setAttribute(Attribute a) { m_attrib = a; }
	
	// returns true as long as the application should be executed
	bool run();
	void close();
private:
	void updateTitle(glm::vec3 color);
	Window m_wnd;
	std::optional<GBufferRenderer> m_gbuffer;
	AttributeRenderer m_attribRenderer;
	RealDiffRenderer m_refDiff;
	Attribute m_attrib;
	Pixels<glm::vec3> m_pixels;
	DisplayMode m_mode;
};
