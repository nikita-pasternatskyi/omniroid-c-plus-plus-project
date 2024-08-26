#pragma once
#include "glm/glm.hpp"
#include "VertexBufferLayout.h"
struct Vertex
{
	glm::vec2 Position;
	glm::vec2 UV;
	int ID;

	static inline void PushLayout(VertexBufferLayout& layout)
	{
		layout.Push<float>(2);
		layout.Push<float>(2);
		layout.Push<int>(1);
	}

	static inline VertexBufferLayout GetVertexBufferLayout()
	{
		VertexBufferLayout layout;
		layout.Push<float>(2);
		layout.Push<float>(2);
		layout.Push<int>(1);
		return layout;
	}
};