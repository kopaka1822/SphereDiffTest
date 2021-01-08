#pragma once
#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include <unordered_set>
#include <glad/glad.h>
#include "error.h"
#include <unordered_map>
#include "Program.h"

class VertexArrayObject
{
public:
	VertexArrayObject()
	{
		glGenVertexArrays(1, &m_id);
	}
	~VertexArrayObject()
	{
		if(m_id)
		{
			glDeleteVertexArrays(1, &m_id);
			m_id = 0;
		}
	}
	VertexArrayObject(const VertexArrayObject&) = delete;
	VertexArrayObject& operator=(const VertexArrayObject&) = delete;
	VertexArrayObject(VertexArrayObject&& o) noexcept
	{
		swap(o);
	}
	VertexArrayObject& operator=(VertexArrayObject&& o) noexcept
	{
		swap(o);
		return *this;
	}
	void swap(VertexArrayObject& o) noexcept
	{
		std::swap(m_id, o.m_id);
		std::swap(m_vertexCount, o.m_vertexCount);
	}

	/// \brief binds the opengl vertex object
	void bind() const
	{
		glBindVertexArray(m_id);

		glDebugError("VertexArrayObject::bind");
	}

	/// \brief binds a vertex buffer to this vertex array object
	/// \param location in location of the vertex shader
	/// \param buffer vertex buffer
	template<class TElement>
	void attachBuffer(GLuint location, const VertexBuffer<TElement>& buffer)
	{
		// is data in the buffer?
		dassert(buffer.getCount() > 0);
		// is the vertex count equivalent to the last attached buffer?
		dassert(m_vertexCount == 0 || buffer.getCount() == m_vertexCount);
		m_vertexCount = buffer.getCount();

		bind();
		buffer.bind();

		GLenum type = gltype::Type<TElement>;
		GLint count = gltype::NumComponents<TElement>;

		if (type == GL_FLOAT || type == GL_DOUBLE)
			glVertexAttribPointer(location, count, type, GL_FALSE, 0, nullptr);
		else // upload integer as integer
			glVertexAttribIPointer(location, count, type, 0, nullptr);

		glEnableVertexAttribArray(location);

		glDebugError("VertexArrayObject::attachBuffer");
	}

	/// \brief uses glDrawArrays to draw the attached buffers
	/// \param mode draw mode e.g. GL_TRIANGLES, GL_LINES, GL_POINTS...
	void drawArrays(GLenum mode, const Program& program) const
	{
		program.bind();
		bind(); // bind vao
		glDrawArrays(mode, 0, GLsizei(m_vertexCount));

		glDebugError("VertexArrayObject::drawArrays");
	}

	/// \brief uses glDrawElements to draw the attached buffers
	/// \param mode draw mode e.g. GL_TRIANGLES, GL_LINES, GL_POINTS...
	/// \param indices the index buffer used to index the triangles
	void drawElements(GLenum mode, const Program& program, const IndexBuffer& indices) const
	{
		program.bind();
		bind(); // bind vao
		indices.bind();
		glDrawElements(mode, GLsizei(indices.getCount()), GL_UNSIGNED_INT, nullptr);

		glDebugError("VertexArrayObject::drawElements");
	}

private:
	GLuint m_id = 0;
	size_t m_vertexCount = 0;
};
