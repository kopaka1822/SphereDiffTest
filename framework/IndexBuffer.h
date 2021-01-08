#pragma once
#include "BufferBase.h"

class IndexBuffer : public BufferBase<uint32_t, GL_ELEMENT_ARRAY_BUFFER>
{
public:
	/// \brief adds a new index to the buffer
	void addIndex(uint32_t index)
	{
		addElement(index);
	}
};