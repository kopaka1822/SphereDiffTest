#pragma once
#include <algorithm>
#include "Shader.h"
#include "error.h"
#include "Uniform.h"

class Program
{
public:
	Program()
		:
	m_id(glCreateProgram())
	{}
	~Program()
	{
		if(m_id)
		{
			glDeleteProgram(m_id);
			m_id = 0;
		}
	}
	Program(const Program&) = delete;
	Program& operator=(const Program&) = delete;
	Program(Program&& o) noexcept
	{
		swap(o);
	}
	Program& operator=(Program&& o) noexcept
	{
		swap(o);
		return *this;
	}
	void swap(Program& o) noexcept
	{
		std::swap(m_id, o.m_id);
		std::swap(m_attachments, o.m_attachments);
	}

	/// \attachs a shader to the program
	/// \param shader loaded shader
	/// \return reference to self
	Program& attach(const Shader& shader)
	{
		// shader already attached?
		dassert(std::find(m_attachments.begin(), m_attachments.end(), shader.getId()) == m_attachments.end());

		glAttachShader(m_id, shader.getId());

		m_attachments.push_back(shader.getId());
		return *this;
	}

	/// \brief links all shaders
	/// \return reference to self
	Program& link()
	{
		// Link all attached shaders (if possible)
		glLinkProgram(m_id);

		for (auto id : m_attachments)
			glDetachShader(m_id, id);
		m_attachments.clear();

		GLint isLinked = GL_FALSE;
		glGetProgramiv(m_id, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE)
		{
			// Read out the error message.
			GLint length = 0;
			glGetProgramiv(m_id, GL_INFO_LOG_LENGTH, &length);
			std::string errorLog;
			errorLog.resize(length);
			glGetProgramInfoLog(m_id, length, &length, &errorLog[0]);
			errorLog = std::string("Linker error: ") + errorLog;
			throw std::runtime_error(errorLog.c_str());
		}
		return *this;
	}
	
	/// \brief binds the program on the gpu
	void bind() const
	{
		glUseProgram(m_id);

		glDebugError("Program::bind");
	}


	/// \brief tries to find the uniform with the given name (e.g. uniform float x)
	/// \tparam T the type of the uniform. Valid types are: float, int, uint, vec2 - vec4, ivec1 - ivec4, uvec1 - uvec4, mat2, mat3, mat4
	/// \param name the name which is used in the shader
	/// \param value initialization value of the uniform
	/// \throw runtime error if uniform was not found
	/// \return the located uniform
	template <class T>
	Uniform<T> getUniform(const std::string& name, const T& value) const
	{
		bind();
		auto location = glGetUniformLocation(m_id, name.c_str());
		
		glDebugError("Program::getUniform");
		 
		if (location == -1)
			throw std::runtime_error("could not find uniform " + name);
		return Uniform<T>(m_id, location, value);
	}

	/// \brief tries to find the attribute with the given name (e.g. in vec3 position)
	/// \param name attribute name
	/// \throw runtime error if the attribute could not be found
	/// \return the shader location id of the attribute (used for VertexArrayObject)
	GLint getAttribute(const std::string& name) const
	{
		auto location = glGetAttribLocation(m_id, name.c_str());

		glDebugError("Program::getAttribute");

		if (location == -1)
			throw std::runtime_error("could not find attribute " + name);
		return location;
	}
private:
	GLuint m_id = 0;
	std::vector<GLuint> m_attachments;
};
