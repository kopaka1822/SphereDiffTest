#pragma once
#include <optional>

#include "../framework/Window.h"
#include <script/ScriptEngine.h>


#include "AttributeRenderer.h"
#include "GBufferRenderer.h"

class App final : public script::ScriptObject
{
	App();
public:

	static App& get();

	// returns true as long as the application should be executed
	bool run();
	void close();
private:
	void updateTitle(glm::vec3 color);
	Window m_wnd;
	std::optional<GBufferRenderer> m_gbuffer;
	AttributeRenderer m_attribRenderer;
};
