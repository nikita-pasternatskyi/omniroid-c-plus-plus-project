#pragma once
#include "../debug/DebugLogger.h"
#include "glad/glad.h"
#include <vector>

struct VertexBufferElement
{
public:
	unsigned int Type;
	unsigned int Count;
	bool Normalized;
	
	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return sizeof(GLfloat);
			case GL_UNSIGNED_INT:	return sizeof(GLuint);
			case GL_UNSIGNED_BYTE:	return sizeof(GLbyte);
			case GL_INT:			return sizeof(GLint);
		}
		ASSERT(false);
		return 0;
	}
};

struct VertexBufferLayout
{
public:
	VertexBufferLayout()
		: _stride(0) {}

	template<typename T>
	void Push(unsigned int count)
	{
	}

	template<>
	void Push<float>(unsigned int count)
	{
		_elements.push_back(VertexBufferElement{ GL_FLOAT, count, GL_FALSE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_FLOAT);
	}

	template<>
	void Push<int>(unsigned int count)
	{
		_elements.push_back(VertexBufferElement{ GL_INT, count, GL_FALSE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_INT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		_elements.push_back(VertexBufferElement{ GL_UNSIGNED_INT, count, GL_FALSE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		_elements.push_back(VertexBufferElement{ GL_UNSIGNED_BYTE, count, GL_TRUE });
		_stride += count * VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE);
	}

	inline const std::vector<VertexBufferElement> GetElements() const { return _elements; }
	inline unsigned int GetStride() const { return _stride; }

private:
	std::vector<VertexBufferElement> _elements;
	unsigned int _stride;
};
