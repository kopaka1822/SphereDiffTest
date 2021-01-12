#pragma once
#include <string>
#include <vector>
#include <algorithm>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <functional>
#include "Program.h"
#include <memory>

#include "Pixels.h"

// windows likes to define some stuff
#undef min
#undef max
#undef DELETE

class Window
{
public:
	// glfw wrapper
	enum Button
	{
		LEFT = GLFW_MOUSE_BUTTON_1,
		RIGHT = GLFW_MOUSE_BUTTON_2,
		MIDDLE = GLFW_MOUSE_BUTTON_3,
		BUTTON4 = GLFW_MOUSE_BUTTON_4,
		BUTTON5 = GLFW_MOUSE_BUTTON_5,
		BUTTON6 = GLFW_MOUSE_BUTTON_6,
		BUTTON7 = GLFW_MOUSE_BUTTON_7,
		BUTTON8 = GLFW_MOUSE_BUTTON_8,
		SIZE
	};

	enum class Key
	{
		UNKNOWN = GLFW_KEY_UNKNOWN,
		SPACE = GLFW_KEY_SPACE,
		APOSTROPHE = GLFW_KEY_APOSTROPHE,
		COMMA = GLFW_KEY_COMMA,
		MINUS = GLFW_KEY_MINUS,
		PERIOD = GLFW_KEY_PERIOD,
		SLASH = GLFW_KEY_SLASH,
		KEY_0 = GLFW_KEY_0,
		KEY_1 = GLFW_KEY_1,
		KEY_2 = GLFW_KEY_2,
		KEY_3 = GLFW_KEY_3,
		KEY_4 = GLFW_KEY_4,
		KEY_5 = GLFW_KEY_5,
		KEY_6 = GLFW_KEY_6,
		KEY_7 = GLFW_KEY_7,
		KEY_8 = GLFW_KEY_8,
		KEY_9 = GLFW_KEY_9,
		SEMICOLON = GLFW_KEY_SEMICOLON,
		EQUAL = GLFW_KEY_EQUAL,
		A = GLFW_KEY_A,
		B = GLFW_KEY_B,
		C = GLFW_KEY_C,
		D = GLFW_KEY_D,
		E = GLFW_KEY_E,
		F = GLFW_KEY_F,
		G = GLFW_KEY_G,
		H = GLFW_KEY_H,
		I = GLFW_KEY_I,
		J = GLFW_KEY_J,
		K = GLFW_KEY_K,
		L = GLFW_KEY_L,
		M = GLFW_KEY_M,
		N = GLFW_KEY_N,
		O = GLFW_KEY_O,
		P = GLFW_KEY_P,
		Q = GLFW_KEY_Q,
		R = GLFW_KEY_R,
		S = GLFW_KEY_S,
		T = GLFW_KEY_T,
		U = GLFW_KEY_U,
		V = GLFW_KEY_V,
		W = GLFW_KEY_W,
		X = GLFW_KEY_X,
		Y = GLFW_KEY_Y,
		Z = GLFW_KEY_Z,
		LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
		BACKSLASH = GLFW_KEY_BACKSLASH,
		RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
		GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
		WORLD_1 = GLFW_KEY_WORLD_1,
		WORLD_2 = GLFW_KEY_WORLD_2,
		ESCAPE = GLFW_KEY_ESCAPE,
		ENTER = GLFW_KEY_ENTER,
		TAB = GLFW_KEY_TAB,
		BACKSPACE = GLFW_KEY_BACKSPACE,
		INSERT = GLFW_KEY_INSERT,
		DELETE = GLFW_KEY_DELETE,
		RIGHT = GLFW_KEY_RIGHT,
		LEFT = GLFW_KEY_LEFT,
		DOWN = GLFW_KEY_DOWN,
		UP = GLFW_KEY_UP,
		PAGE_UP = GLFW_KEY_PAGE_UP,
		PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
		HOME = GLFW_KEY_HOME,
		END = GLFW_KEY_END,
		CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
		SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
		NUM_LOCK = GLFW_KEY_NUM_LOCK,
		PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
		PAUSE = GLFW_KEY_PAUSE,
		F1 = GLFW_KEY_F1,
		F2 = GLFW_KEY_F2,
		F3 = GLFW_KEY_F3,
		F4 = GLFW_KEY_F4,
		F5 = GLFW_KEY_F5,
		F6 = GLFW_KEY_F6,
		F7 = GLFW_KEY_F7,
		F8 = GLFW_KEY_F8,
		F9 = GLFW_KEY_F9,
		F10 = GLFW_KEY_F10,
		F11 = GLFW_KEY_F11,
		F12 = GLFW_KEY_F12,
		F13 = GLFW_KEY_F13,
		F14 = GLFW_KEY_F14,
		F15 = GLFW_KEY_F15,
		F16 = GLFW_KEY_F16,
		F17 = GLFW_KEY_F17,
		F18 = GLFW_KEY_F18,
		F19 = GLFW_KEY_F19,
		F20 = GLFW_KEY_F20,
		F21 = GLFW_KEY_F21,
		F22 = GLFW_KEY_F22,
		F23 = GLFW_KEY_F23,
		F24 = GLFW_KEY_F24,
		F25 = GLFW_KEY_F25,
		KP_0 = GLFW_KEY_KP_0,
		KP_1 = GLFW_KEY_KP_1,
		KP_2 = GLFW_KEY_KP_2,
		KP_3 = GLFW_KEY_KP_3,
		KP_4 = GLFW_KEY_KP_4,
		KP_5 = GLFW_KEY_KP_5,
		KP_6 = GLFW_KEY_KP_6,
		KP_7 = GLFW_KEY_KP_7,
		KP_8 = GLFW_KEY_KP_8,
		KP_9 = GLFW_KEY_KP_9,
		KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
		KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
		KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
		KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
		KP_ADD = GLFW_KEY_KP_ADD,
		KP_ENTER = GLFW_KEY_KP_ENTER,
		KP_EQUAL = GLFW_KEY_KP_EQUAL,
		LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
		LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
		LEFT_ALT = GLFW_KEY_LEFT_ALT,
		LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
		RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
		RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
		RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
		RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
		MENU = GLFW_KEY_MENU,
		SIZE
	};
public:
	/// \brief creates a new window
	/// \param width initial client width in pixels
	/// \param height initial client height in pixels
	/// \param title initial window title
	Window(size_t width, size_t height, const std::string& title);
	~Window();

	/// \return true if the window should remain open
	bool isOpen() const { return m_open; }

	/// \brief handles all window, mouse and key events and causes the key, mouse-callback functions to be called
	void handleEvents();

	/// \brief uploads pixel data to gpu
	void swapBuffer(const Pixels<glm::vec3>& pixels) const;
	
	/// \return window client width in pixels
	size_t getWidth() const { return m_width; }

	/// \return window client height in pixels
	size_t getHeight() const { return m_height; }

	/// \return mouse coordinate in pixels
	size_t getMouseX() const { return std::min(m_width - 1, m_mouseX); }
	
	/// \return mouse coordinate in pixels
	size_t getMouseY() const { return std::min(m_height - 1, m_mouseY); }
	
	/// \return mouse coordinate between [-1,1]
	float getMouseXNorm() const { return float(getMouseX()) / (getWidth() - 1) * 2.0f - 1.0f; }
	
	/// \return mouse coordinate between [-1,1]
	float getMouseYNorm() const { return float(getMouseY()) / (getHeight() - 1) * 2.0f - 1.0f; }

	/// \brief sets callback for mouse button up events
	/// \param cb callback Button = mouse button, first float = mouse x [-1,1], second float = mouse y [-1,1]
	void setMouseUpCallback(std::function<void(Button, float, float)> cb) { m_onMouseUp = cb; }

	/// \brief sets callback for mouse button down events
	/// \param cb callback: Button = mouse button, first float = mouse x [-1,1], second float = mouse y [-1,1]
	void setMouseDownCallback(std::function<void(Button, float, float)> cb) { m_onMouseDown = cb; }

	/// \brief sets callback for mouse button down events
	/// \param cb callback: first float = mouse x [-1,1], second float = mouse y [-1,1], third and fourth float = mouse dx and dy
	void setMouseMoveCallback(std::function<void(float, float, float, float)> cb) { m_onMouseMove = cb; }

	/// \brief sets callback for key up events
	/// \param cb callback: Key = keyboard key
	void setKeyUpCallback(std::function<void(Key)> cb) { m_onKeyUp = cb; }

	/// \brief sets callback for key down events
	/// \param cb callback: Key = keyboard key
	void setKeyDownCallback(std::function<void(Key)> cb) { m_onKeyDown = cb; }

	/// \brief sets callback for window size change
	/// \param cb callback: first int = window width, second int = window height
	void setSizeChangeCallback(std::function<void(int, int)> cb) { m_onSizeChange = cb; }

	/// \brief sets the window title bar
	/// \param title window title
	void setTitle(const std::string& title);

	/// \brief closes the window
	void close();
private:

#ifdef WINDOW_PUT_PIXEL
	/// \brief converts floating point color into a uint32
	/// \return converted color
	static uint8_t convertToBits(float r);

	void initShader();
#endif

	void resize(size_t width, size_t height);
private:
	struct GLFWwindow* m_handle = nullptr;
	bool m_open = true;
	size_t m_width = 0;
	size_t m_height = 0;
	
	std::unique_ptr<Program> m_program;
	uint32_t m_vao = 0;
	uint32_t m_texture = 0;

	size_t m_mouseX = 0;
	size_t m_mouseY = 0;

	std::function<void(Key)> m_onKeyDown;
	std::function<void(Key)> m_onKeyUp;
	std::function<void(Button, float, float)> m_onMouseDown;
	std::function<void(Button, float, float)> m_onMouseUp;
	std::function<void(float, float, float, float)> m_onMouseMove;
	std::function<void(int, int)> m_onSizeChange;
};
