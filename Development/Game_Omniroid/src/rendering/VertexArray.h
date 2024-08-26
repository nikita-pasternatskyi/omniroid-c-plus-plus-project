#pragma once
#include "VertexBuffer.h"
#include "VertexBufferLayout.h"
class VertexArray
{
public:
	VertexArray();

	void AddBuffer(const VertexBufferLayout& layout);

	void Bind() const;
	void Unbind() const;

private:
	unsigned int _id;
};

