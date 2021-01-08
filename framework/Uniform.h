#pragma once
#include <glad/glad.h>
#include "glmmath.h"

template<class T>
class Uniform
{
	// single type
	void _set(const GLfloat& value) const
	{
		glUniform1f(m_location, value);
	}
	void _set(const GLint& value) const
	{
		glUniform1i(m_location, value);
	}
	void _set(const GLuint& value) const
	{
		glUniform1ui(m_location, value);
	}

	// simple arrays
	void _set(const std::vector<GLfloat>& value) const
	{
		glUniform1fv(m_location, value.size(), value.data());
	}
	void _set(const std::vector<GLint>& value) const
	{
		glUniform1iv(m_location, value.size(), value.data());
	}
	void _set(const std::vector<GLuint>& value) const
	{
		glUniform1uiv(m_location, value.size(), value.data());
	}

	// vec2
	void _set(const glm::vec2& value) const
	{
		glUniform2f(m_location, value.x, value.y);
	}
	void _set(const glm::ivec2& value) const
	{
		glUniform2i(m_location, value.x, value.y);
	}
	void _set(const glm::uvec2& value) const
	{
		glUniform2ui(m_location, value.x, value.y);
	}

	// vec3
	void _set(const glm::vec3& value) const
	{
		glUniform3f(m_location, value.x, value.y, value.z);
	}
	void _set(const glm::ivec3& value) const
	{
		glUniform3i(m_location, value.x, value.y, value.z);
	}
	void _set(const glm::uvec3& value) const
	{
		glUniform3ui(m_location, value.x, value.y, value.z);
	}

	// vec4
	void _set(const glm::vec4& value) const
	{
		glUniform4f(m_location, value.x, value.y, value.z, value.w);
	}
	void _set(const glm::ivec4& value) const
	{
		glUniform4i(m_location, value.x, value.y, value.z, value.w);
	}
	void _set(const glm::uvec4& value) const
	{
		glUniform4ui(m_location, value.x, value.y, value.z, value.w);
	}

	// matrices
	void _set(const glm::mat2& value) const
	{
		glUniformMatrix2fv(m_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value));
	}
	void _set(const glm::mat2x3& value) const
	{
		glUniformMatrix2x3fv(m_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value));
	}
	void _set(const glm::mat2x4& value) const
	{
		glUniformMatrix2x4fv(m_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value));
	}
	void _set(const glm::mat3& value) const
	{
		glUniformMatrix3fv(m_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value));
	}
	void _set(const glm::mat3x2& value) const
	{
		glUniformMatrix3x2fv(m_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value));
	}
	void _set(const glm::mat3x4& value) const
	{
		glUniformMatrix3x4fv(m_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value));
	}
	void _set(const glm::mat4& value) const
	{
		glUniformMatrix4fv(m_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value));
	}
	void _set(const glm::mat4x2& value) const
	{
		glUniformMatrix4x2fv(m_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value));
	}
	void _set(const glm::mat4x3& value) const
	{
		glUniformMatrix4x3fv(m_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&value));
	}
public:
	Uniform() = default;
	Uniform(GLuint program, GLint location, const T& value)
		:
	m_program(program), m_location(location), m_value(value)
	{
		set(value);
	}
	Uniform(const Uniform&) = delete;
	Uniform& operator=(const Uniform&) = delete;
	Uniform(Uniform&& o) noexcept
	{
		swap(o);
	}
	Uniform& operator=(Uniform&& o) noexcept
	{
		swap(o);
		return *this;
	}
	void swap(Uniform& o) noexcept
	{
		std::swap(m_program, o.m_program);
		std::swap(m_location, o.m_location);
		std::swap(m_value, o.m_value);
	}

	/// \brief uploads the value to the gpu uniform
	/// \param value new uniform value
	void set(const T& value)
	{
		// was the uniform initialized?
		dassert(m_program);

		// is the program still valid?
		dassert(glIsProgram(m_program));

		glUseProgram(m_program);
		_set(value);
		m_value = value;
	}
	
	/// \returns the last value the uniform was set to
	const T& get() const
	{
		// was the unifrom initialized?
		dassert(m_program);

		return m_value;
	}
private:
	GLuint m_program = 0;
	GLint m_location;
	T m_value;
};
