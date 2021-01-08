#pragma once
#include "BufferBase.h"

template<class TElement>
class VertexBuffer : public BufferBase<TElement, GL_ARRAY_BUFFER>
{
public:
	void addVertex(const TElement& only)
	{
		this->addElement(only);
	}
};