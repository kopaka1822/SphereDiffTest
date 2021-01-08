#pragma once
#include <glad/glad.h>
#include <vector>
#include "error.h"
#include "type_conversion.h"

template<class TElement, GLenum TBuffer>
class BufferBase
{
	
public:
	BufferBase(){}
	virtual ~BufferBase()
	{
		dispose();
	}
	BufferBase(const BufferBase&) = delete;
	BufferBase& operator=(const BufferBase&) = delete;
	BufferBase(BufferBase&& o) noexcept
	{
		swap(o);
	}
	BufferBase& operator=(BufferBase&& o) noexcept
	{
		swap(o);
		return *this;
	}
	void swap(BufferBase& o) noexcept
	{
		std::swap(m_id, o.m_id);
		std::swap(m_elements, o.m_elements);
	}

	/// \brief creates a new buffer object (deletes the old one) and uploads the elements
	void uploadToGpu()
	{
		// delete old buffer if existing
		dispose();

		// generate and initialize buffer
		glGenBuffers(1, &m_id);
		glBindBuffer(TBuffer, m_id);
		glBufferData(TBuffer, m_elements.size() * sizeof(TElement), m_elements.data(), GL_STATIC_DRAW);

		glDebugError("VertexArrayObject::uploadToGpu");
	}

	/// \brief clears the element array on cpu side
	void clear()
	{
		// resize to zero elements but keep the heap storage
		m_elements.resize(0);
	}

	/// \brief binds the buffer on the gpu
	void bind() const
	{
		// was data uploaded to the gpu?
		dassert(m_id != 0);
		glBindBuffer(TBuffer, m_id);

		glDebugError("VertexArrayObject::bind");
	}

	/// \return number of elements divided by TCount
	size_t getCount() const
	{
		return m_elements.size();
	}

	GLuint getId() const
	{
		return m_id;
	}
private:
	void dispose()
	{
		if(m_id)
		{
			glDeleteBuffers(1, &m_id);
			m_id = 0;
		}
	}

protected:
	void addElement(const TElement& e)
	{
		m_elements.push_back(e);
	}
private:
	GLuint m_id = 0;
	std::vector<TElement> m_elements;
};
