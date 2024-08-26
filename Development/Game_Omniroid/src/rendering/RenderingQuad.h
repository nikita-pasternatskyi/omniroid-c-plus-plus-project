#pragma once
#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"
#include "Quad.h"
class RenderingQuad
{
public:
	RenderingQuad();
	Quad quad;
	VertexArray Vao;
	VertexBuffer Vbo;
	IndexBuffer Ibo;

	void Bind();
};