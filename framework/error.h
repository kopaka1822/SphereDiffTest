#pragma once
#include <glad/glad.h>
#include <string>
#include <stdexcept>

/// \brief checks for an error. WARNING this is a hard sync with the gpu
/// \param where where did the error occur
inline void glCheckError(const std::string& where = "")
{
	auto err = glGetError();
	if (err == GL_NO_ERROR)
		return;

	const char* desc = "unknown error";
	const char* detailed = "";
	switch (err)
	{
	case GL_INVALID_ENUM:
		desc = "GL_INVALID_ENUM";
		detailed = "An unacceptable value is specified for an enumerated argument. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_INVALID_VALUE:
		desc = "GL_INVALID_VALUE";
		detailed = "A numeric argument is out of range. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_INVALID_OPERATION:
		desc = "GL_INVALID_OPERATION";
		detailed = "The specified operation is not allowed in the current state. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_INVALID_FRAMEBUFFER_OPERATION:
		desc = "GL_INVALID_FRAMEBUFFER_OPERATION";
		detailed = "The framebuffer object is not complete. The offending command is ignored and has no other side effect than to set the error flag.";
		break;
	case GL_OUT_OF_MEMORY:
		desc = "GL_OUT_OF_MEMORY";
		detailed = "There is not enough memory left to execute the command. The state of the GL is undefined, except for the state of the error flags, after this error is recorded.";
		break;
	}

	std::string msg = "";
	if (where.length())
		msg += "(" + where + ") ";
	msg += desc;
	msg += "\n";
	msg += detailed;
	throw std::runtime_error(msg);
}

/// \brief checks for an error only in debug mode (faster in release)
/// \param where where did the error occur
inline void glDebugError(const std::string& where = "")
{
#ifdef _DEBUG
	glCheckError(where);
#endif
}

/// \brief in debug mode: tests that expression is true and breaks the programm otherwise
#ifdef _DEBUG
#define dassert(expression) if(!(expression)) __debugbreak()
#else
#define dassert(expression) 
#endif